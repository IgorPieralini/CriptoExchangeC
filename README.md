# 💸 CriptoExchangeC

Um sistema de exchange de criptomoedas feito em linguagem **C**, utilizando arquivos binários para persistência dos dados.  
Este projeto simula as funcionalidades principais de uma plataforma de criptomoedas, incluindo:

---

## ⚙️ Funcionalidades

- 🔐 Sistema de login
- 💼 Gerenciamento de carteira (saldo em criptomoedas)
- 📈 Compra e venda de moedas
- 📜 Histórico de transações
- 💾 Salvamento de dados em arquivos binários
- 🧮 Interface de terminal simples e funcional

---

## 📂 Organização do Projeto

O sistema é dividido em múltiplos arquivos `.c` e `.h`, para manter a modularização e facilitar a manutenção do código.

---

## ▶️ Como rodar o projeto

1. Clone ou acesse este Replit.
2. Se estiver usando localmente:
   ```bash
   gcc *.c -o main && ./main
   ```
3. No Replit, configure o comando de execução com:
   ```
   gcc *.c -o main && ./main
   ```

---

## 📁 Estrutura esperada dos arquivos

- `main.c`: ponto de entrada da aplicação
- `login.c / login.h`: sistema de autenticação
- `carteira.c / carteira.h`: controle de saldo e criptos
- `transacoes.c / transacoes.h`: compra, venda e histórico
- `utils.c / utils.h`: funções auxiliares diversas

---

## 👨‍💻 Autor

- **Igor Marques Pieralini**  
  RA: 242240034

- **Victor Augusto Caramori André**  
  RA: 242240091



