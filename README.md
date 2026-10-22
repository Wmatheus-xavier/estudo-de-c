# Projetos 3D em C: Cubo no Terminal e Hélice OpenGL

Este repositório contém as instruções para compilar e executar dois tipos de projetos gráficos em C: um renderizador de cubo 3D feito puramente no terminal (ASCII) e renderizadores de uma hélice cilíndrica 3D utilizando a biblioteca gráfica OpenGL/GLUT.

---

## 🛠️ 1. Dependências e Instalação

Para rodar o cubo no terminal, você precisa apenas do compilador GCC padrão. Porém, para compilar os códigos da hélice (`heli.c` e `helice-3d.c`), será necessário instalar as bibliotecas do **OpenGL** e do **GLUT** no seu sistema.

### No Arch Linux / Manjaro
Abra o terminal (como o Kitty) e instale os pacotes básicos de compilação e o FreeGLUT:
```
sudo pacman -S gcc make freeglut glu mesa
```
No Ubuntu / Debian / Mint 
```
sudo apt update
sudo apt install build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
```
No Windows (MSYS2 / MinGW)

Se estiver usando o MSYS2, instale o GCC e o FreeGLUT com:
```
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut
```

2. Compilando e Rodando o Cubo 3D (Terminal)

O arquivo codigo do cubo 3d.c não precisa de interface gráfica, mas exige a biblioteca matemática padrão do C (math.h).

Arquivo: codigo do cubo 3d.c

Como compilar:
Como o nome do arquivo tem espaços, coloque-o entre aspas e use a flag -lm no final para linkar a matemática.
```

gcc "codigo do cubo 3d.c" -o cubo -lm

```

Como executar:
```
./cubo
```


(Para parar a animação no terminal, pressione Ctrl + C)


3. Compilando e Rodando a Hélice 3D (OpenGL/GLUT)

Os arquivos heli.c e helice-3d.c geram uma janela gráfica interativa. Eles precisam ser linkados com as bibliotecas do OpenGL (-lGL), do GLU (-lGLU), do GLUT (-lglut) e de matemática (-lm).

Arquivos: heli.c ou helice-3d.c (use o mesmo comando para ambos, apenas trocando o nome do arquivo fonte e de saída).

Como compilar no Linux:
```

gcc heli.c -o heli -lGL -lGLU -lglut -lm
```
(Se for compilar o outro arquivo: gcc helice-3d.c -o helice -lGL -lGLU -lglut -lm)

Como compilar no Windows (MinGW):

```
gcc heli.c -o heli.exe -lopengl32 -lglu32 -lfreeglut -lm
```
Como executar:

```
./heli
```

(No Windows, digite apenas heli ou heli.exe)
🎮 Controles da Hélice:

  Mouse (Botão Esquerdo): Segure e arraste para girar a câmera livremente.

  Scroll do Mouse: Aumenta ou diminui o Zoom.

  Tecla a ou A: Pausa / Retoma a rotação automática.

  Tecla q, Q ou ESC: Fecha a janela.

 Solução de Problemas Comuns

  Erro undefined reference to sin ou cos: Você esqueceu de colocar a flag -lm no final do comando de compilação.

  Erro GL/glut.h: No such file or directory: O pacote do FreeGLUT não está instalado corretamente no seu sistema. Volte ao passo 1 e certifique-se de instalar as dependências.

  Falha de Segmentação (Core Dumped) no Cubo: Verifique se o tamanho do seu terminal é pelo menos 100x44 (largura x altura). Se a janela do terminal (ex: Kitty) estiver muito pequena, o desenho pode quebrar.
