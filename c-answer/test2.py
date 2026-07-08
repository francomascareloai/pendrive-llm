import base64, io, json, re, sys, time
sys.stdout.reconfigure(encoding='utf-8')
from PIL import Image, ImageDraw, ImageFont
import requests

API_KEY = 'sk-JmSrv7f3EpRpMGgqg6tSiPahwggDue2yqpZSSJnQ8KU'
font = ImageFont.load_default()

def make_img(lines, options, h=180):
    img = Image.new('RGB', (1100, h), (240, 240, 240))
    draw = ImageDraw.Draw(img)
    y = 18
    for line in lines:
        draw.text((20, y), line, fill=(0,0,0), font=font); y += 23
    draw.text((20, y+8), options, fill=(0,0,0), font=font)
    buf = io.BytesIO()
    img.save(buf, format='JPEG', quality=85)
    return base64.b64encode(buf.getvalue()).decode()

questions = [
    {'code': ['int i=0;', 'i = i++ + ++i;', 'printf("%d", i);'],
     'opts': 'L:A(0) L:B(1) L:C(2) L:D(UB)', 'answer': 'D', 'note': 'seq point UB'},
    {'code': ['int a[5]; int *p = a;', 'printf("%zu %zu",', '  sizeof(a), sizeof(p));'],
     'opts': 'L:A(4 4) L:B(20 4) L:C(20 8) L:D(4 8)', 'answer': 'C', 'note': 'sizeof array vs ptr 64-bit'},
    {'code': ['void (*f(int))(void);'],
     'opts': 'L:A(ret void*) L:B(ret ptr p/ func) L:C(ret int) L:D(error)',
     'answer': 'B', 'note': 'complex decl spiral'},
    {'code': ['int a=0, b=0;', 'if (a++ && b++) ;', 'printf("%d %d", a, b);'],
     'opts': 'L:A(0 0) L:B(1 0) L:C(1 1) L:D(0 1)', 'answer': 'B', 'note': 'short-circuit'},
    {'code': ['int f() {', '  static int x = 5;', '  return x--;', '}', 'int r = f() + f();', 'printf("%d", r);'],
     'opts': 'L:A(9) L:B(10) L:C(8) L:D(7)', 'answer': 'A', 'note': 'static var post-dec'},
    {'code': ['int a[]={10,20,30};', 'int *p[]={a, a+1, a+2};', 'int **pp = p;', 'printf("%d", (*pp)[1]);'],
     'opts': 'L:A(10) L:B(20) L:C(30) L:D(err)', 'answer': 'B', 'note': 'ptr to ptr array'},
    {'code': ['const int x = 5;', 'int *p = (int*)&x;', '*p = 10;', 'printf("%d", x);'],
     'opts': 'L:A(5) L:B(10) L:C(UB) L:D(err)', 'answer': 'C', 'note': 'const cast UB'},
    {'code': ['char s[] = "abc";', 'char t[] = "abc";', 'printf("%d", s == t);'],
     'opts': 'L:A(0) L:B(1) L:C(-1) L:D(err)', 'answer': 'A', 'note': 'array comparison'},
]

models = ['umans-flash', 'umans-qwen3.6-35b-a3b']
results = {m: {'ok': 0, 'fail': 0, 'n': 0} for m in models}

prompt = 'C code. Output ONLY L:A, L:B, L:C, or L:D.'

for q in questions:
    b64 = make_img(q['code'], q['opts'])
    code_str = '; '.join(q['code'])
    print(f'\n[{q["note"]}] {code_str[:60]}...  Ans={q["answer"]}', flush=True)
    for m in models:
        try:
            t0 = time.time()
            resp = requests.post('https://api.code.umans.ai/v1/chat/completions',
                headers={'Authorization': f'Bearer {API_KEY}', 'Content-Type': 'application/json'},
                json={'model': m, 'messages': [{'role': 'user', 'content': [
                    {'type': 'text', 'text': prompt},
                    {'type': 'image_url', 'image_url': {'url': f'data:image/jpeg;base64,{b64}'}}
                ]}], 'max_tokens': 1000},
                timeout=180)
            t = time.time() - t0
            d = resp.json()
            msg = d['choices'][0]['message']
            txt = (msg.get('content') or msg.get('reasoning') or msg.get('reasoning_content') or '').strip()
            matches = list(re.finditer(r'L:(?:Q(\d+):)?([A-Za-z0-9])', txt))
            ans = matches[-1].group(2).upper() if matches else None
            if ans is None:
                # Try extracting bare letter
                txt_clean = txt.strip().rstrip('.')
                if len(txt_clean) == 1 and txt_clean in 'ABCD':
                    ans = txt_clean
            ok = ans == q['answer']
            results[m]['n'] += 1
            if ok: results[m]['ok'] += 1
            else: results[m]['fail'] += 1
            icon = '✅' if ok else '❌'
            print(f'  {m:28s} {t:3.0f}s  {str(ans):6s} {icon}', flush=True)
        except Exception as e:
            print(f'  {m:28s} ERR {str(e)[:40]}', flush=True)

print(f'\n{"="*60}')
for m, r in results.items():
    pct = r['ok']/r['n']*100 if r['n'] else 0
    print(f'{m:30s} {r["ok"]}/{r["n"]} ({pct:.0f}%)')

