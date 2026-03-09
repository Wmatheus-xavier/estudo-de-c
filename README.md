.

💻 Terminal-Driven Study: Foco Total no CLI
Este repositório/guia é dedicado a criar e manter o ambiente de estudos definitivo em casa, rodando 100% dentro do terminal.

A filosofia é simples: o mouse é uma distração, e interfaces gráficas (GUIs) roubam seu foco. Aqui, construímos um ecossistema minimalista, rápido e à prova de procrastinação para ler, codar, fazer anotações e gerenciar o tempo.

🎯 Por que estudar no Terminal?
Zero Distrações: Sem notificações do sistema, sem abas de YouTube abertas ao lado do seu texto.

Velocidade: Navegação puramente baseada no teclado.

Baixo Consumo: Roda em qualquer máquina, do seu PC principal a um Raspberry Pi ou um notebook antigo.

Imersão (Flow): Estilo hacker. Tela preta, texto claro, concentração máxima.

🧰 O Arsenal (Ferramentas Essenciais)
Para dominar os estudos via CLI, você precisa de um conjunto de ferramentas que substituam os aplicativos convencionais:

Multiplexador de Terminal (Tmux ou Zellij): O coração do setup. Permite dividir o terminal em várias janelas (ex: uma para anotações, uma para o código, uma para o timer).

Editor de Texto (Neovim, Vim ou Nano): Para escrever resumos, documentação em Markdown e programar.

Gerenciador de Tarefas (Taskwarrior): Controle o que você precisa estudar, crie projetos e defina prazos sem sair da linha de comando.

Timer Pomodoro (pomodoro-cli ou termdown): Essencial para manter ciclos de estudo intenso (ex: 50 minutos de foco, 10 de descanso).

Leitor de PDF/Epub (Zathura ou pdftotext): Se você usa uma interface gráfica minimalista (como i3wm/Sway), o Zathura é navegado via teclado (estilo Vim). Se quer puramente texto, converta os livros.

Música de Fundo (Cmus ou ncspot): Tocar lo-fi ou ruído branco direto do terminal para não ter que abrir o Spotify no navegador.

⚙️ Fluxo de Trabalho (O Setup Ideal)
Um exemplo de como organizar sua tela durante uma sessão de 2 horas de estudo:

Abra seu terminal em tela cheia (F11).

Inicie uma sessão no Tmux: tmux new -s estudos.

Divida a tela:

Lado Esquerdo (60%): Seu editor de texto (nvim resumos.md) para anotações e leitura.

Lado Direito - Topo (20%): Linha de comando livre para testar códigos, compilar ou rodar o task para ver a lista de afazeres.

Lado Direito - Fundo (20%): O timer rodando (termdown 50m).

🛡️ Dicas Hardcore contra Procrastinação
Bloqueie a internet no /etc/hosts: Antes de começar, redirecione redes sociais para 127.0.0.1.

Use Aliases: Crie atalhos no seu .bashrc ou .zshrc. Exemplo: digite focar e o sistema limpa a tela, abre o Tmux, liga o Pomodoro e abre suas anotações do dia.
