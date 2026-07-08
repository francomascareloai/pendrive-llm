import base64, io, json, re, sys, time
sys.stdout.reconfigure(encoding='utf-8')
from PIL import Image, ImageDraw, ImageFont
import requests

API_KEY = 'sk-JmSrv7f3EpRpMGgqg6tSiPahwggDue2yqpZSSJnQ8KU'
font = ImageFont.load_default()

def make_img(lines, options, h=200):
    img = Image.new('RGB', (1100, h), (240, 240, 240))
    draw = ImageDraw.Draw(img)
    y = 15
    for line in lines:
        draw.text((20, y), line, fill=(0,0,0), font=font)
        y += 22
    draw.text((20, y + 10), options, fill=(0,0,0), font=font)
    buf = io.BytesIO()
    img.save(buf, format='JPEG', quality=85)
    return base64.b64encode(buf.getvalue()).decode()

questions = [
    {   # Sequence point violation (UB) - but compilers do something
        'code': ['int i=0;', 'i = i++ + ++i;', 'printf("%d", i);'],
        'opts': 'L:A(0) L:B(1) L:C(2) L:D(UB/qualquer)',
        'answer': 'D',
        'note': 'seq point UB'
    },
    {   # sizeof array vs pointer
        'code': ['int a[5]; int *p = a;', 'printf("%zu %zu",', '  sizeof(a), sizeof(p));'],
        'opts': 'L:A(4 4) L:B(20 4) L:C(20 8) L:D(4 8)',
        'answer': 'C',
        'note': 'sizeof array vs pointer (64-bit)'
    },
    {   # Complex declaration - signal
        'code': ['void (*f(int))(void);', '// f is a function that...'],
        'opts': 'L:A(retorna void) L:B(retorna ponteiro p/ func) L:C(retorna int) L:D(syntax error)',
        'answer': 'B',
        'note': 'complex declaration spiral rule'
    },
    {   # Short-circuit + side effects
        'code': ['int a=0, b=0;', 'if (a++ && b++) ;', 'printf("%d %d", a, b);'],
        'opts': 'L:A(0 0) L:B(1 0) L:C(1 1) L:D(0 1)',
        'answer': 'B',
        'note': 'short-circuit eval'
    },
    {   # Static variable
        'code': ['int f() {', '  static int x = 5;', '  return x--;', '}', 'int r = f() + f();', 'printf("%d", r);'],
        'opts': 'L:A(9) L:B(10) L:C(8) L:D(7)',
        'answer': 'A',
        'note': 'static var + post-decrement'
    },
    {   # Double pointer
        'code': ['int a[]={10,20,30};', 'int *p[]={a, a+1, a+2};', 'int **pp = p;', 'printf("%d", (*pp)[1]);'],
        'opts': 'L:A(10) L:B(20) L:C(30) L:D(erro)',
        'answer': 'B',
        'note': 'pointer to pointer array'
    },
]

models = ['umans-flash', 'umans-qwen3.6-35b-a3b']

results = {m: {'correct': 0, 'wrong': 0, 'total': 0} for m in models}
results['umans-kimi-k2.7'] = {'correct': 0, 'wrong': 0, 'total': 0}

for q in questions:
    print(f'\n{"="*70}', flush=True)
    for l in q['code']: print(f'  {l}')
    print(f'  {q["opts"]}', flush=True)
    print(f'  Correct: {q["answer"]} ({q["note"]})', flush=True)
    b64 = make_img(q['code'], q['opts'])
    
    for m in models:
        try:
            t0 = time.time()
            resp = requests.post('https://api.code.umans.ai/v1/chat/completions',
                headers={'Authorization': f'Bearer {API_KEY}', 'Content-Type': 'application/json'},
                json={'model': m, 'messages': [{'role': 'user', 'content': [
                    {'type': 'text', 'text': 'C code. Output ONLY L:A, L:B, L:C, or L:D.'},
                    {'type': 'image_url', 'image_url': {'url': f'data:image/jpeg;base64,{b64}'}}
                ]}], 'max_tokens': 200},
                timeout=180)
            t = time.time() - t0
            d = resp.json()
            msg = d['choices'][0]['message']
            txt = (msg.get('content') or msg.get('reasoning') or msg.get('reasoning_content') or '').strip()
            matches = list(re.finditer(r'L:(?:Q(\d+):)?([A-Za-z0-9])', txt))
            answer = matches[-1].group(2).upper() if matches else f'?({txt[:20]})'
            correct = answer == q['answer']
            results[m]['total'] += 1
            if correct:
                results[m]['correct'] += 1
            else:
                results[m]['wrong'] += 1
            icon = '✅' if correct else '❌'
            print(f'  {m:30s} {t:3.0f}s  {str(answer):8s} {icon}', flush=True)
        except Exception as e:
            print(f'  {m:30s} ERROR: {str(e)[:50]}', flush=True)

# Summary
print(f'\n{"="*70}', flush=True)
print(f'RESUMO:', flush=True)
for m, r in results.items():
    pct = (r['correct']/r['total']*100) if r['total'] else 0
    print(f'  {m:30s} {r["correct"]}/{r["total"]} ({pct:.0f}%)', flush=True)

