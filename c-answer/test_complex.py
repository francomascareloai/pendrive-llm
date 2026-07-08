import base64, io, json, re, sys, time
sys.stdout.reconfigure(encoding='utf-8')
from PIL import Image, ImageDraw, ImageFont
import requests

with open('config.json') as f:
    config = json.load(f)

API_KEY = config['api_key']
API_URL = config['api_url']
font = ImageFont.load_default()

def make_img(code, options):
    img = Image.new('RGB', (1000, 150), (240, 240, 240))
    draw = ImageDraw.Draw(img)
    draw.text((20, 15), code, fill=(0,0,0), font=font)
    draw.text((20, 90), options, fill=(0,0,0), font=font)
    buf = io.BytesIO()
    img.save(buf, format='JPEG', quality=85)
    return base64.b64encode(buf.getvalue()).decode()

questions = [
    {
        'code': 'int a[]={10,20,30,40,50};\nint *p=a+3; p[-1]+=5;\nprintf("%d",a[2]);',
        'opts': 'L:A(20) L:B(25) L:C(30) L:D(35)',
        'answer': 'D',
        'note': 'pointer arith + precedence'
    },
    {
        'code': 'struct X { char c; int i; };\nprintf("%zu", sizeof(struct X));',
        'opts': 'L:A(5) L:B(6) L:C(8) L:D(12)',
        'answer': 'C',
        'note': 'struct padding (32-bit int)'
    }
]

prompt_template = (
    'C code question shown in screenshot.\n'
    'Respond EXACTLY with L:A, L:B, L:C, or L:D.\n'
    'No explanation. No extra text.\n'
    'Examples: L:B   L:D'
)

models = [config['model']]

for q in questions:
    print(f'\n{"="*60}', flush=True)
    print(f'Q: {q["code"]}  |  {q["opts"]}  |  Correct: {q["answer"]} ({q["note"]})', flush=True)
    b64 = make_img(q['code'], q['opts'])
    
    for m in models:
        try:
            t0 = time.time()
            resp = requests.post(API_URL,
                headers={'Authorization': f'Bearer {API_KEY}', 'Content-Type': 'application/json'},
                json={'model': m, 'messages': [{'role': 'user', 'content': [
                    {'type': 'text', 'text': prompt_template},
                    {'type': 'image_url', 'image_url': {'url': f'data:image/jpeg;base64,{b64}'}}
                ]}], 'max_tokens': 500},
                timeout=180)
            t = time.time() - t0
            d = resp.json()
            if 'error' in d:
                print(f'  {m:20s} ERROR: {d["error"]}', flush=True)
                continue
            msg = d['choices'][0]['message']
            txt = (msg.get('content') or msg.get('reasoning') or msg.get('reasoning_content') or '').strip()
            matches = list(re.finditer(r'L:(?:Q(\d+):)?([A-Za-z0-9])', txt))
            answer = matches[-1].group(2).upper() if matches else f'?({txt[:30]})'
            correct = '✅' if answer == q['answer'] else '❌'
            print(f'  {m:20s} {resp.status_code} {t:3.0f}s  {answer:8s} {correct}', flush=True)
        except Exception as e:
            print(f'  {m:20s} ERROR: {str(e)[:50]}', flush=True)

