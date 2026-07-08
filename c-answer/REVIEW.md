# Code Review - c-answer

## CRITICAL

| Line | Issue | Fix |
|------|-------|-----|
| 29 | `config.json` ausente causa crash sem mensagem | Tratar FileNotFoundError |
| 47 | Pendrive pode ser read-only → log falha | Fallback para stdout |

## WARNING

| Line | Issue | Fix |
|------|-------|-----|
| 138 | Precedência de operadores ambígua | Adicionar parênteses |
| 177 | Retry em 500/503 sem delay | Adicionar sleep |
| 195 | `re.search` em T: pega primeiro match | Usar `finditer` pegar último |

## SUGGESTION

| Line | Issue |
|------|-------|
| 148 | OpenRouter recomenda headers HTTP-Referer e X-Title |
| 339 | Coordenadas negativas no drag do region selector |
| 235 | multi_batch sem limite de tamanho |
