import base64, io, json, re, sys, time
sys.stdout.reconfigure(encoding='utf-8')
from PIL import Image, ImageDraw, ImageFont
import requests

API_KEY = 'sk-JmSrv7f3EpRpMGgqg6tSiPahwggDue2yqpZSSJnQ8KU'

def test_model(model, prompt, img_bytes=None):
    content = [{'type': 'text', 'text': prompt}]
    if img_bytes:
        content.append({'type': 'image_url', 'image_url': {'url': f'data:image/jpeg;base64,{img_bytes}'}})
    t0 = time.time()
    resp = requests.post('https://api.code.umans.ai/v1/chat/completions',
        headers={'Authorization': f'Bearer {API_KEY}', 'Content-Type': 'application/json'},
        json={'model': model, 'messages': [{'role': 'user', 'content': content}], 'max_tokens': 1000},
        timeout=180)
    t = time.time() - t0
    d = resp.json()
    msg = d['choices'][0]['message']
    txt = (msg.get('content') or msg.get('reasoning') or msg.get('reasoning_content') or '').strip()
    return txt, t, resp.status_code

# Create test image with tricky C question
img = Image.new('RGB', (900, 120), (240, 240, 240))
draw = ImageDraw.Draw(img)
font = ImageFont.load_default()
# Question with pointer arithmetic + precedence
draw.text((20, 20), 'int a[]={10,20,30,40,50}; int *p=a+3; p[-1] += 5;', fill=(0,0,0), font=font)
draw.text((20, 50), 'printf("%d", a[2]); // A)20 B)25 C)30 D)35', fill=(0,0,0), font=font)

buf = io.BytesIO()
img.save(buf, format='JPEG', quality=85)
b64 = base64.b64encode(buf.getvalue()).decode()

prompt = ('C code. What is the output?\n'
          'Respond ONLY with L:A, L:B, L:C, or L:D.\n'
          'No explanation.')

models = ['umans-kimi-k2.7', 'umans-flash', 'umans-qwen3.6-35b-a3b']
for m in models:
    print(f'\n=== {m} ===', flush=True)
    try:
        txt, elapsed, code = test_model(m, prompt, b64)
        print(f'HTTP {code} ({elapsed:.0f}s)', flush=True)
        matches = list(re.finditer(r'L:(?:Q(\d+):)?([A-Za-z0-9])', txt))
        if matches:
            print(f'>>> L:{matches[-1].group(2).upper()}', flush=True)
        else:
            print(f'>>> sem formato: {txt[:120]}', flush=True)
    except Exception as e:
        print(f'ERRO: {e}', flush=True)

