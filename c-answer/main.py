import base64
import ctypes
import ctypes.wintypes
import hashlib
import json
import logging
import os
import re
import sys
import threading
import time
import tkinter as tk
from io import BytesIO
from queue import Empty, Queue

import mss
import requests
from PIL import Image, ImageFilter, ImageEnhance, ImageDraw
import pystray
from pystray import MenuItem as item

try:
    # Força modo escuro para menus ANTES de qualquer UI (Tkinter) carregar.
    # As funções SetPreferredAppMode (ordinal 135) e FlushMenuThemes
    # (ordinal 136) não são exportadas por nome — só por ordinal.
    # getattr(uxtheme, str(135)) NÃO funciona; é preciso uxtheme[135].
    uxtheme = ctypes.windll.LoadLibrary("uxtheme.dll")
    SetPreferredAppMode = uxtheme[135]
    SetPreferredAppMode.argtypes = [ctypes.c_int]
    SetPreferredAppMode.restype = ctypes.c_int
    SetPreferredAppMode(2)  # 2 = ForceDark
    FlushMenuThemes = uxtheme[136]
    FlushMenuThemes.argtypes = []
    FlushMenuThemes.restype = None
    FlushMenuThemes()
except Exception as e:
    logging.debug(f"Dark mode init falhou: {e}")

# ─── Config ─────────────────────────────────────────────────────────────────

if getattr(sys, "frozen", False):
    BASE_DIR = os.path.dirname(sys.executable)
else:
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))

CONFIG_FILE = os.path.join(BASE_DIR, "config.json")


def load_config():
    try:
        with open(CONFIG_FILE, "r") as f:
            return json.load(f)
    except FileNotFoundError:
        return None
    except json.JSONDecodeError as e:
        ctypes.windll.user32.MessageBoxW(
            0, f"Erro de sintaxe no config.json:\n{e}", "c-answer - Erro", 0x10
        )
        sys.exit(1)
    except Exception as e:
        ctypes.windll.user32.MessageBoxW(
            0, f"Erro ao ler config.json:\n{e}", "c-answer - Erro", 0x10
        )
        sys.exit(1)


config = load_config()
if config is None:
    ctypes.windll.user32.MessageBoxW(
        0,
        "config.json não encontrado.\nCopie o arquivo para o mesmo diretório do executável.",
        "c-answer",
        0x10,
    )
    sys.exit(1)
API_KEY = config.get("api_key", "")
API_URL = config.get("api_url", "https://openrouter.ai/api/v1/chat/completions")
MODEL = config.get("model", "google/gemini-2.5-flash-001")
EXTRA_HEADERS = config.get("extra_headers", {})
_fb = config.get("fallback", {})
FALLBACK_API_URL = _fb.get("api_url", "")
FALLBACK_API_KEY = _fb.get("api_key", "")
FALLBACK_MODEL = _fb.get("model", "")
FALLBACK_HEADERS = _fb.get("extra_headers", {})
CAPTURE_INTERVAL = config.get("capture_interval", 10)
MIN_API_INTERVAL = config.get("min_api_interval", 20)
GRACE_PERIOD = config.get("grace_period", 12)
JPEG_QUALITY = config.get("jpeg_quality", 85)
CAPTURE_SCALE = config.get("capture_scale", 0.65)
SHARPEN = config.get("sharpen", True)
MAX_TEXT_CHARS = config.get("max_text_chars", 200)
OPACITY = config.get("opacity", 0.85)
VERBOSE_LOG = config.get("verbose_log", False)

LOG_FILE = os.path.join(BASE_DIR, "c-answer.log")
try:
    logging.basicConfig(
        level=logging.DEBUG if VERBOSE_LOG else logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        handlers=[logging.FileHandler(LOG_FILE)],
    )
except (OSError, PermissionError):
    logging.basicConfig(
        level=logging.DEBUG if VERBOSE_LOG else logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        stream=sys.stdout,
    )
logging.getLogger("urllib3").setLevel(logging.WARNING)
logging.getLogger("PIL").setLevel(logging.WARNING)
logging.getLogger("mss").setLevel(logging.WARNING)

# ─── Queues & State ─────────────────────────────────────────────────────────

out_q = Queue()
cmd_q = Queue()
_last_hash = ""
_last_api_call = 0
stop = threading.Event()
_lock = threading.Lock()

_pending_change = False  # screen changed, waiting grace period
_pending_change_time = 0

# ─── Prompts ─────────────────────────────────────────────────────────────────

AUTO_PROMPT = (
    "You are analyzing a C programming exam on screen. Identify the question.\n"
    "Be concise and accurate. Trace pointers, memory, precedence carefully.\n\n"
    "STRICT RULES:\n"
    "1. Respond in EXACTLY ONE of these formats — no other text before or after:\n"
    "  L:<letter>              -> for MCQ (A, B, C, D...)\n"
    "  L:Q<num>:<letter>       -> if question number is visible\n"
    "  T:<short answer>        -> for written/assertions (max 10 words)\n"
    "2. If no C question is visible, respond with exactly: ?\n"
    "3. Do NOT explain your reasoning. Do NOT add any text outside the format.\n\n"
    "Examples:\n"
    "  L:B\n"
    "  L:Q3:C\n"
    "  T:int x=5;\n"
    "  T:I e II sao verdadeiras\n"
    "  ?"
)

MULTI_PROMPT = (
    "You are analyzing a LONG C programming question shown across MULTIPLE "
    "screenshots (scrolled). Combine all parts.\n\n"
    "STRICT RULES:\n"
    "1. Respond in EXACTLY ONE of these formats — no other text:\n"
    "  L:<letter>              -> for MCQ\n"
    "  L:Q<num>:<letter>       -> with question number\n"
    "  T:<short answer>        -> for written (max 5 words)\n"
    "2. If unsure, respond with exactly: ?\n"
    "3. Do NOT add any explanation."
)

# ─── Capture ─────────────────────────────────────────────────────────────────

_monitor_phys = None


def get_monitor_phys():
    global _monitor_phys
    if _monitor_phys is None:
        with mss.MSS() as sct:
            m = sct.monitors[1]
            _monitor_phys = (m["width"], m["height"])
    return _monitor_phys


def capture_full():
    with mss.MSS() as sct:
        m = sct.monitors[1]
        raw = sct.grab(m)
        return Image.frombytes("RGB", (raw.width, raw.height), raw.rgb)


def capture_region(bbox):
    x1, y1, x2, y2 = bbox
    with mss.MSS() as sct:
        r = {"left": x1, "top": y1, "width": x2 - x1, "height": y2 - y1}
        raw = sct.grab(r)
        return Image.frombytes("RGB", (raw.width, raw.height), raw.rgb)


try:
    _RESAMPLE = Image.Resampling.LANCZOS
except AttributeError:
    _RESAMPLE = Image.LANCZOS


def process_image(img):
    w, h = img.size
    if CAPTURE_SCALE < 1.0:
        img = img.resize((int(w * CAPTURE_SCALE), int(h * CAPTURE_SCALE)), _RESAMPLE)
    if SHARPEN:
        img = img.filter(ImageFilter.SHARPEN)
        img = ImageEnhance.Contrast(img).enhance(1.12)
    return img


def img_to_b64(img):
    buf = BytesIO()
    img.save(buf, format="JPEG", quality=JPEG_QUALITY)
    return base64.b64encode(buf.getvalue()).decode()


# ─── LLM ─────────────────────────────────────────────────────────────────────


def ask_llm(
    images_b64,
    prompt,
    retry=2,
    model_override=None,
    url_override=None,
    key_override=None,
    headers_override=None,
):
    api_url = url_override or API_URL
    api_key = key_override or API_KEY
    extra_headers = headers_override if headers_override is not None else EXTRA_HEADERS

    if (not api_key) or (api_key.startswith("sk-") and len(api_key) < 20):
        return "KEY?"

    content = [{"type": "text", "text": prompt}]
    for b64 in images_b64:
        content.append(
            {"type": "image_url", "image_url": {"url": f"data:image/jpeg;base64,{b64}"}}
        )

    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json",
        "HTTP-Referer": "https://github.com/c-answer",
        "X-Title": "c-answer",
    }
    headers.update(extra_headers)

    payload = {
        "model": model_override if model_override else MODEL,
        "messages": [{"role": "user", "content": content}],
        "max_tokens": 1000,
    }

    for attempt in range(retry + 1):
        try:
            resp = requests.post(api_url, headers=headers, json=payload, timeout=60)
            resp.raise_for_status()
            data = resp.json()
            msg = data["choices"][0]["message"]
            txt = (
                msg.get("content")
                or msg.get("reasoning")
                or msg.get("reasoning_content")
                or ""
            ).strip()
            logging.debug(f"LLM raw: {txt!r}")
            return txt
        except requests.exceptions.HTTPError as e:
            code = e.response.status_code
            logging.warning(f"LLM HTTP {code} (attempt {attempt + 1})")
            if code == 401:
                return "KEY?"
            if code not in (429, 500, 502, 503, 504):
                return f"Erro HTTP {code}"
            if attempt < retry:
                out_q.put(("status", f"Erro {code}... retentando"))
                delay = (3 * (attempt + 1)) if code == 429 else 2
                time.sleep(delay)
                continue
        except Exception as e:
            logging.error(f"LLM Network Error (attempt {attempt + 1}): {e}")
            if attempt < retry:
                out_q.put(("status", f"Rede falhou... retentando"))
                time.sleep(2)
                continue
    return "Erro de Conexao/API"


# ─── Answer Parser ──────────────────────────────────────────────────────────


def parse_answer(raw):
    if not raw:
        return ("none", "")
    # Strip the "N: " counter prefix prepended in process_llm_bg so the
    # sentinel checks below (KEY?, ?, Erro) still match.
    raw = re.sub(r"^\d+:\s*", "", raw, count=1)
    if raw == "KEY?":
        return ("key", "")
    if "Erro " in raw:
        return ("text", raw[:40])

    # Strip thinking block to search only the actual answer.
    # Models that emit <think>...</think> put the real answer after the
    # closing tag. Only use the after-tag content when non-empty,
    # otherwise search the full response so answers aren't lost.
    clean = raw
    pos = raw.rfind("</think>")
    if pos >= 0:
        after = raw[pos + len("</think>") :].strip()
        if after:
            clean = after

    # Flexible: find LAST L: or T: in clean response
    matches = list(re.finditer(r"L:(?:Q(\d+):)?([A-Za-z0-9])", clean))
    if matches:
        m = matches[-1]
        qnum, letter = m.group(1), m.group(2).upper()
        return ("mcq", f"Q{qnum}:{letter}" if qnum else letter)

    matches = list(re.finditer(r"T:([^\n]*)", clean))
    if matches:
        m = matches[-1]
        text = m.group(1).strip()[:MAX_TEXT_CHARS]
        return (
            "text",
            text + ("…" if len(m.group(1).strip()) > MAX_TEXT_CHARS else ""),
        )

    if clean.strip() in ("?", "?."):
        return ("none", "")

    # Fallback: just use first 8 chars if nothing structured found
    short = raw[:8].replace("\n", " ")
    logging.warning(f"Unstructured response: {raw!r}")
    return ("text", short + ("…" if len(raw) > 8 else ""))


# ─── Capture Thread ─────────────────────────────────────────────────────────

_ans_counter = 0
_current_req_id = 0
_req_lock = threading.Lock()


def process_llm_bg(req_id, images_b64, prompt, is_multi=False):
    global _ans_counter
    ans = ask_llm(images_b64, prompt)
    if ans.startswith("Erro") or ans == "KEY?":
        # Don't burn a fallback call if a newer capture already superseded us.
        with _req_lock:
            if req_id != _current_req_id:
                return
        if FALLBACK_API_URL and FALLBACK_API_KEY and FALLBACK_MODEL:
            out_q.put(("status", f"{ans}... Fallback NeuralWatt!"))
            ans = ask_llm(
                images_b64,
                prompt,
                retry=1,
                model_override=FALLBACK_MODEL,
                url_override=FALLBACK_API_URL,
                key_override=FALLBACK_API_KEY,
                headers_override=FALLBACK_HEADERS,
            )
        else:
            out_q.put(("status", f"{ans}... sem fallback!"))

    with _req_lock:
        if req_id != _current_req_id:
            logging.info(f"Discarding obsolete result for req {req_id}")
            return
        if not is_multi:
            _ans_counter += 1
            logging.info(f"[bg] {ans}")
            out_q.put(("answer", f"{_ans_counter}: {ans}"))
        else:
            logging.info(f"[multi bg] {ans}")
            out_q.put(("answer", ans))


def capture_loop():
    global \
        _last_hash, \
        _last_api_call, \
        _pending_change, \
        _pending_change_time, \
        _ans_counter, \
        _current_req_id
    multi_batch = []
    multi_mode = False

    while not stop.is_set():
        # ── Process commands (manual always works) ──
        try:
            cmd = cmd_q.get_nowait()
            action = cmd["action"]

            try:
                if action == "capture_now":
                    _pending_change = False
                    b64 = img_to_b64(process_image(capture_full()))
                    with _req_lock:
                        _current_req_id += 1
                        req = _current_req_id
                    out_q.put(("status", "Enviando imagem..."))
                    threading.Thread(
                        target=process_llm_bg,
                        args=(req, [b64], AUTO_PROMPT),
                        daemon=True,
                    ).start()

                elif action == "capture_region":
                    _pending_change = False
                    b64 = img_to_b64(process_image(capture_region(cmd["bbox"])))
                    with _req_lock:
                        _current_req_id += 1
                        req = _current_req_id
                    out_q.put(("status", "Enviando regiao..."))
                    threading.Thread(
                        target=process_llm_bg,
                        args=(req, [b64], AUTO_PROMPT),
                        daemon=True,
                    ).start()

                elif action == "multi_add":
                    if len(multi_batch) >= 10:
                        logging.warning("[multi] batch limit reached (10)")
                        out_q.put(("status", "limite (10)"))
                        continue
                    b64 = img_to_b64(process_image(capture_full()))
                    multi_batch.append(b64)
                    multi_mode = True
                    out_q.put(("multi_count", len(multi_batch)))
                    logging.info(f"[multi] +1 ({len(multi_batch)})")

                elif action == "multi_send":
                    _pending_change = False
                    if multi_batch:
                        with _req_lock:
                            _current_req_id += 1
                            req = _current_req_id
                        out_q.put(("status", "Enviando lote..."))
                        threading.Thread(
                            target=process_llm_bg,
                            args=(req, multi_batch, MULTI_PROMPT, True),
                            daemon=True,
                        ).start()
                    else:
                        out_q.put(("status", "empty"))
                    multi_batch = []
                    multi_mode = False

                elif action == "multi_clear":
                    multi_batch = []
                    multi_mode = False
                    out_q.put(("multi_count", 0))

            except Exception as e:
                logging.error(f"Erro processando comando {action}: {e}")
                out_q.put(("answer", "Erro Interno"))

        except Empty:
            pass

        # ── Auto periodic capture (paused during multi-mode) ──
        if not multi_mode:
            now = time.time()

            # Check for screen changes
            try:
                img = capture_full()
                proc = process_image(img)
                h = hashlib.md5(proc.tobytes()).hexdigest()

                with _lock:
                    changed = h != _last_hash
                    if changed:
                        _last_hash = h

                if changed:
                    # Screen changed → start grace period (don't clear yet)
                    _pending_change = True
                    _pending_change_time = now
                    logging.debug(
                        f"screen changed, grace until {now + GRACE_PERIOD:.0f}"
                    )

            except Exception as e:
                logging.error(f"capture: {e}")

            # Handle pending change (grace period expired)
            if _pending_change and (now - _pending_change_time >= GRACE_PERIOD):
                _pending_change = False
                out_q.put(("clear", ""))

                with _lock:
                    elapsed = now - _last_api_call
                    can_send = elapsed >= MIN_API_INTERVAL

                if can_send:
                    try:
                        b64 = img_to_b64(process_image(capture_full()))
                        with _req_lock:
                            _current_req_id += 1
                            req = _current_req_id
                        threading.Thread(
                            target=process_llm_bg,
                            args=(req, [b64], AUTO_PROMPT),
                            daemon=True,
                        ).start()
                        with _lock:
                            _last_api_call = time.time()
                    except Exception as e:
                        logging.error(f"auto: {e}")

        stop.wait(max(CAPTURE_INTERVAL, 1))


# ─── Region Selector ────────────────────────────────────────────────────────


def select_region():
    sel = None
    win = tk.Toplevel()
    win.overrideredirect(True)
    win.attributes("-alpha", 0.25, "-topmost", True)
    win.state("zoomed")
    win.configure(bg="black")

    c = tk.Canvas(win, cursor="crosshair", highlightthickness=0)
    c.pack(fill="both", expand=True)

    sx = sy = 0
    rect = None

    def press(e):
        nonlocal sx, sy, rect
        sx, sy = e.x, e.y
        if rect:
            c.delete(rect)
        rect = c.create_rectangle(e.x, e.y, e.x, e.y, outline="#00ff41", width=2)

    def drag(e):
        if rect:
            c.coords(rect, sx, sy, max(e.x, 0), max(e.y, 0))

    def release(e):
        nonlocal sel
        ow, oh = win.winfo_width(), win.winfo_height()
        cx1 = max(0, min(sx, ow))
        cy1 = max(0, min(sy, oh))
        cx2 = max(0, min(e.x, ow))
        cy2 = max(0, min(e.y, oh))
        x1, x2 = (cx1, cx2) if cx1 < cx2 else (cx2, cx1)
        y1, y2 = (cy1, cy2) if cy1 < cy2 else (cy2, cy1)
        pw, ph = get_monitor_phys()
        xs1 = int((x1 / ow) * pw) if ow else 0
        xs2 = int((x2 / ow) * pw) if ow else 0
        ys1 = int((y1 / oh) * ph) if oh else 0
        ys2 = int((y2 / oh) * ph) if oh else 0
        if (xs2 - xs1) > 30 and (ys2 - ys1) > 30:
            sel = (xs1, ys1, xs2, ys2)
        win.destroy()

    win.bind("<Escape>", lambda e: win.destroy())
    c.bind("<Button-1>", press)
    c.bind("<B1-Motion>", drag)
    c.bind("<ButtonRelease-1>", release)

    win.wait_window()
    return sel


# ─── Overlay (discreto, estilo barra de tarefas) ────────────────────────────

TASKBAR_BG = "#1e1e1e"
MCQ_COLOR = "#3a6a3a"
TEXT_COLOR = "#5a5a5a"


class Overlay:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("")
        self.root.overrideredirect(True)
        self.root.attributes("-alpha", OPACITY, "-topmost", True)

        # Fundo extremamente escuro para combinar com temas escuros e permitir drag
        TASKBAR_BG = "#141414"
        self.root.configure(bg=TASKBAR_BG)
        self.root.resizable(False, False)

        # Variáveis para arrastar a janela
        self._drag_data = {"x": 0, "y": 0}

        self.label = tk.Label(
            self.root,
            text="",
            fg="#7a7a7a",
            bg=TASKBAR_BG,
            font=("Segoe UI", 10, "bold"),
            padx=8,
            pady=2,
            wraplength=450,
            justify="left",
        )
        self.label.pack()

        self.sub = tk.Label(
            self.root,
            text="",
            fg="#555555",
            bg=TASKBAR_BG,
            font=("Segoe UI", 9),
            padx=8,
            pady=0,
        )
        self.sub.pack()

        # Bindings para clicar e arrastar
        for widget in (self.root, self.label, self.sub):
            widget.bind("<ButtonPress-1>", self._start_drag)
            widget.bind("<B1-Motion>", self._on_drag)

        self.root.update_idletasks()
        self._style_hidden()
        self._hotkeys()
        self.label.config(text="?", fg="#888", font=("Consolas", 20, "bold"))
        self.sub.config(text="aguardando")
        self._corner()
        self.root.deiconify()
        self._setup_tray()

    def _create_icon_image(self):
        img = Image.new("RGB", (64, 64), color=(30, 30, 30))
        d = ImageDraw.Draw(img)
        d.rectangle([16, 16, 48, 48], outline=MCQ_COLOR, width=4)
        return img

    def _setup_tray(self):
        menu = pystray.Menu(
            item(
                "Capturar Agora (Ctrl+Shift+F4)",
                lambda *a: self.root.after(0, self._capture),
            ),
            item(
                "Capturar Regiao (Ctrl+Shift+F3)",
                lambda *a: self.root.after(0, self._region),
            ),
            pystray.Menu.SEPARATOR,
            item(
                "Multi: Add (Ctrl+Shift+F8)",
                lambda *a: self.root.after(0, self._multi_add),
            ),
            item(
                "Multi: Enviar (Ctrl+Shift+F9)",
                lambda *a: self.root.after(0, self._multi_send),
            ),
            item(
                "Mostrar/Ocultar (Ctrl+Shift+F2)",
                lambda *a: self.root.after(0, self._toggle_vis),
            ),
            pystray.Menu.SEPARATOR,
            item("Sair (Ctrl+Shift+F12)", lambda *a: self.root.after(0, self.quit)),
        )
        self.tray_icon = pystray.Icon(
            "c-answer", self._create_icon_image(), "c-answer", menu
        )
        threading.Thread(
            target=lambda: self.tray_icon.run(setup=self._apply_dark_tray), daemon=True
        ).start()

    def _apply_dark_tray(self, icon):
        try:
            hwnd = icon._hwnd
            dwm = ctypes.windll.dwmapi
            dwm.DwmSetWindowAttribute.argtypes = [
                ctypes.wintypes.HWND,
                ctypes.c_uint,
                ctypes.c_void_p,
                ctypes.c_uint,
            ]
            dwm.DwmSetWindowAttribute.restype = ctypes.c_long
            # 20 para Win11, 19 para Win10 antigo
            dwm.DwmSetWindowAttribute(hwnd, 20, ctypes.byref(ctypes.c_int(1)), 4)
            dwm.DwmSetWindowAttribute(hwnd, 19, ctypes.byref(ctypes.c_int(1)), 4)
            ux = ctypes.windll.uxtheme
            ux.SetWindowTheme.argtypes = [
                ctypes.wintypes.HWND,
                ctypes.wintypes.LPCWSTR,
                ctypes.wintypes.LPCWSTR,
            ]
            ux.SetWindowTheme.restype = ctypes.c_long
            ux.SetWindowTheme(hwnd, "DarkMode_Explorer", None)
        except Exception as e:
            logging.debug(f"Dark mode dwm falhou: {e}")
        icon.visible = True

    def _style_hidden(self):
        user32 = ctypes.windll.user32
        # restype/argtypes obrigatórios em 64-bit para não truncar HWND
        user32.GetParent.restype = ctypes.wintypes.HWND
        user32.GetWindowLongW.argtypes = [ctypes.wintypes.HWND, ctypes.c_int]
        user32.GetWindowLongW.restype = ctypes.wintypes.LONG
        user32.SetWindowLongW.argtypes = [
            ctypes.wintypes.HWND,
            ctypes.c_int,
            ctypes.wintypes.LONG,
        ]
        user32.SetWindowLongW.restype = ctypes.wintypes.LONG
        hwnd = user32.GetParent(self.root.winfo_id())
        GWL_EXSTYLE = -20
        # Flags:
        # 0x80000 = WS_EX_LAYERED (suporte a alpha)
        # 0x80 = WS_EX_TOOLWINDOW (esconde do taskbar)
        # 0x08000000 = WS_EX_NOACTIVATE (não rouba o foco do browser)
        # Removido: 0x20 (WS_EX_TRANSPARENT) para permitir que o mouse interaja com a janela!
        flags = 0x80000 | 0x80 | 0x08000000
        curr = user32.GetWindowLongW(hwnd, GWL_EXSTYLE)
        user32.SetWindowLongW(hwnd, GWL_EXSTYLE, curr | flags)

    def _start_drag(self, event):
        self._drag_data["x"] = event.x_root - self.root.winfo_x()
        self._drag_data["y"] = event.y_root - self.root.winfo_y()

    def _on_drag(self, event):
        x = event.x_root - self._drag_data["x"]
        y = event.y_root - self._drag_data["y"]
        self.root.geometry(f"+{x}+{y}")

    def _corner(self):
        # Apenas posiciona no canto na primeira vez, se o usuário arrastar não reseta mais.
        if not hasattr(self, "_placed_initially"):
            self.root.update_idletasks()
            w = self.root.winfo_reqwidth()
            h = self.root.winfo_reqheight()
            sw = self.root.winfo_screenwidth()
            sh = self.root.winfo_screenheight()
            x = sw - w - 300
            y = sh - h - 48
            self.root.geometry(f"+{x}+{y}")
            self._placed_initially = True

    def _hotkeys(self):
        user32 = ctypes.windll.user32
        # argtypes/restype obrigatórios em 64-bit: sem eles o ctypes assume
        # c_int (32-bit) para todos os args/retornos, truncando HWND e
        # ponteiros de MSG.
        user32.RegisterHotKey.argtypes = [
            ctypes.wintypes.HWND,
            ctypes.c_int,
            ctypes.c_uint,
            ctypes.c_uint,
        ]
        user32.RegisterHotKey.restype = ctypes.wintypes.BOOL
        user32.UnregisterHotKey.argtypes = [ctypes.wintypes.HWND, ctypes.c_int]
        user32.UnregisterHotKey.restype = ctypes.wintypes.BOOL
        user32.GetMessageW.argtypes = [
            ctypes.POINTER(ctypes.wintypes.MSG),
            ctypes.wintypes.HWND,
            ctypes.c_uint,
            ctypes.c_uint,
        ]
        user32.GetMessageW.restype = ctypes.wintypes.BOOL
        # Chrome usa F-keys SÓ sem modificadores (F1 ajuda, F3 buscar, F5
        # recarregar, F6 foco, F7 caret, F11 fullscreen, F12 DevTools) e
        # Ctrl+Shift SÓ com letras (B, D, G, I, J, N, T, W...). Logo
        # Ctrl+Shift+Fx NÃO conflita com nada no Chrome: o acorde completo é
        # interceptado pelo Windows antes de chegar ao navegador, e as
        # F-keys bare do Chrome continuam funcionando normalmente.
        # F2/F4/F8/F9 nem como bare o Chrome usa; F3/F12 ele usa bare mas o
        # modificador Ctrl+Shift isola totalmente.
        MOD_CTRL, MOD_SHIFT, MOD_NOREPEAT = 0x0002, 0x0004, 0x4000
        MOD = MOD_CTRL | MOD_SHIFT | MOD_NOREPEAT
        binds = {
            1: (MOD, 0x7B),  # Ctrl+Shift+F12  quit
            2: (MOD, 0x72),  # Ctrl+Shift+F3   region
            3: (MOD, 0x73),  # Ctrl+Shift+F4   capture
            4: (MOD, 0x78),  # Ctrl+Shift+F8   multi add
            5: (MOD, 0x79),  # Ctrl+Shift+F9   multi send
            6: (MOD, 0x71),  # Ctrl+Shift+F2   toggle visibility
        }

        def listen():
            # RegisterHotKey com hWnd=NULL posta WM_HOTKEY na fila da thread
            # que chamou RegisterHotKey. Precisa ser a MESMA thread do loop
            # GetMessageW abaixo, senão as mensagens nunca chegam aqui.
            for hid, (mod, vk) in binds.items():
                if not user32.RegisterHotKey(None, hid, mod, vk):
                    logging.warning(
                        f"RegisterHotKey falhou para id {hid} (vk=0x{vk:X})"
                    )
            msg = ctypes.wintypes.MSG()
            while user32.GetMessageW(ctypes.byref(msg), None, 0, 0) > 0:
                if msg.message == 0x0312:
                    if msg.wParam == 1:
                        self.root.after(0, self.quit)
                        break
                    elif msg.wParam == 2:
                        self.root.after(0, self._region)
                    elif msg.wParam == 3:
                        self.root.after(0, self._capture)
                    elif msg.wParam == 4:
                        self.root.after(0, self._multi_add)
                    elif msg.wParam == 5:
                        self.root.after(0, self._multi_send)
                    elif msg.wParam == 6:
                        self.root.after(0, self._toggle_vis)
            for hid in binds:
                user32.UnregisterHotKey(None, hid)

        threading.Thread(target=listen, daemon=True).start()

    def _region(self):
        self.root.withdraw()
        time.sleep(0.15)
        sel = select_region()
        if sel:
            cmd_q.put({"action": "capture_region", "bbox": sel})

    def _capture(self):
        cmd_q.put({"action": "capture_now"})

    def _multi_add(self):
        cmd_q.put({"action": "multi_add"})

    def _multi_send(self):
        cmd_q.put({"action": "multi_send"})

    def _toggle_vis(self):
        if self.root.state() == "withdrawn":
            self.root.deiconify()
        else:
            self.root.withdraw()

    def show(self, tag, data):
        if tag == "answer":
            atype, display = parse_answer(data)
            if atype == "none":
                self.label.config(text="", font=("Segoe UI", 10, "bold"))
                self.sub.config(text="")
                self.root.withdraw()
            elif atype == "key":
                self.label.config(text="KEY?", fg="#a55", font=("Segoe UI", 11, "bold"))
                self.sub.config(text="api_key invalida")
                self._corner()
                self.root.deiconify()
            else:
                # Cores extremamente discretas para não chamar atenção
                fg_color = "#6b6b6b" if atype == "mcq" else "#555555"
                font_choice = (
                    ("Segoe UI", 10, "bold") if atype == "mcq" else ("Segoe UI", 9)
                )
                self.label.config(text=display, fg=fg_color, font=font_choice)
                self.sub.config(text="")
                self._corner()
                self.root.deiconify()
        elif tag == "clear":
            self.label.config(text="")
            self.sub.config(text="")
            self.root.withdraw()
        elif tag == "multi_count":
            if data > 0:
                self.sub.config(text=f"[{data}]")
                self.label.config(text="···")
                self.root.deiconify()
            else:
                self.sub.config(text="")
                self.label.config(text="")
                self.root.withdraw()
        elif tag == "status":
            self.sub.config(text=data)

    def quit(self):
        stop.set()
        if self.tray_icon:
            self.tray_icon.stop()
        self.root.destroy()


# ─── Main ────────────────────────────────────────────────────────────────────


def main():
    ov = Overlay()

    def poll():
        # Drena toda a fila a cada ciclo para não atrasar a exibição
        # quando vários eventos (status + answer) chegam juntos.
        while True:
            try:
                tag, data = out_q.get_nowait()
                ov.show(tag, data)
            except (Empty, ValueError):
                break
        try:
            ov.root.after(250, poll)
        except tk.TclError:
            pass

    ov.root.after(250, poll)
    threading.Thread(target=capture_loop, daemon=True).start()

    try:
        ov.root.mainloop()
    except KeyboardInterrupt:
        ov.quit()


if __name__ == "__main__":
    main()
