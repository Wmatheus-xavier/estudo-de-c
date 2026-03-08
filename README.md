# estudo-em-casa
Cubo 3D em ASCII no Terminal 🧊

Este projeto é uma renderização clássica de um cubo 3D giratório diretamente no terminal, usando apenas caracteres ASCII. Ele foi escrito em C e utiliza conceitos fundamentais de computação gráfica, como matrizes de rotação matemática, projeção de 3D para 2D e Z-Buffer (buffer de profundidade) para lidar com a oclusão das faces.
📝 Funcionalidades

    Renderização 3D: Calcula a posição de pontos no espaço tridimensional.

    Projeção de Perspectiva: Converte as coordenadas 3D para o plano 2D da sua tela.

    Z-Buffer (Buffer de Profundidade): Garante que as faces da frente do cubo cubram corretamente as faces de trás.

    Animação Contínua: Rotação suave nos eixos X, Y e Z usando a biblioteca <math.h>.

🛠️ Como Compilar e Executar

Para rodar este projeto, você precisará de um compilador C (como o GCC) instalado no seu sistema.

    Clone o repositório ou baixe o arquivo fonte (cubo.c).

    Abra o terminal na pasta onde o arquivo está salvo.

    Compile o código passando a flag -lm para linkar a biblioteca matemática:

Bash

gcc cubo.c -o cubo -lm

    Execute o programa:

Bash

./cubo

Dica: Para parar a execução, pressione Ctrl + C no terminal.
⚙️ Customização

Você pode brincar com o código alterando algumas variáveis globais no início do arquivo para criar efeitos diferentes:

    tamanhoCubo = 20;: Altera o tamanho do cubo.

    larguraTela = 100, alturaTela = 44: Ajuste de acordo com o tamanho do seu terminal para não quebrar a imagem.

    incrementoSuperficie = 0.6;: Altera a densidade dos caracteres. Valores menores preenchem mais as faces, mas exigem mais processamento.

    anguloX += 0.05; (no loop principal): Altere esses valores numéricos no final do main() para deixar o cubo girando mais rápido, mais devagar ou focar a rotação em um eixo específico.

💻 Tecnologias Utilizadas

    Linguagem C (Standard Library, Math, String, Unistd)
