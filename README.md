# Projetos Gráficos em C

Coleção de programas em C que desenham gráficos 3D, tanto no terminal quanto em janela com OpenGL.

## Arquivos do projeto

| Arquivo | Descrição | Tipo |
|---|---|---|
| `cubo3d.c` | Cubo 3D girando, desenhado com caracteres ASCII no terminal | Terminal |
| `openclaude.c` | Versão refeita do cubo, com outros caracteres nas faces | Terminal |
| `heli.c` | Hélice cilíndrica 3D com OpenGL e GLUT | Janela gráfica |
| `helice-3d.c` | Mesma hélice, com código totalmente comentado | Janela gráfica |

---

## Parte 1: Cubo no terminal (`cubo3d.c` e `openclaude.c`)

Esses dois programas usam apenas a biblioteca padrão do C e a biblioteca de matemática. Não precisam de nenhuma instalação extra além do compilador.

### O que instalar

**Linux (Ubuntu/Debian):**

```bash
sudo apt update
sudo apt install build-essential
```

**macOS:**

```bash
xcode-select --install
```

**Windows:**

Instale o [MSYS2](https://www.msys2.org/) e, no terminal MSYS2 UCRT64, rode:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

Outra opção é usar o WSL (Ubuntu dentro do Windows) e seguir os passos do Linux.

### Como compilar

```bash
gcc cubo3d.c -o cubo3d -lm
```

```bash
gcc openclaude.c -o openclaude -lm
```

A flag `-lm` liga a biblioteca de matemática (`sin`, `cos`). Sem ela, aparece o erro `undefined reference to 'sin'`.

### Como executar

```bash
./cubo3d
```

```bash
./openclaude
```

Para sair, aperte `Ctrl + C`.

> **Dica:** deixe o terminal com pelo menos 100 colunas e 44 linhas, senão o cubo fica cortado.

> **Atenção (Windows):** esses programas usam `unistd.h` e `usleep`, que existem no Linux, no macOS, no WSL e no MSYS2, mas não no CMD/PowerShell puro com MSVC.

---

## Parte 2: Hélice 3D com OpenGL (`heli.c` e `helice-3d.c`)

Esses programas abrem uma janela gráfica e precisam das bibliotecas **OpenGL**, **GLU** e **GLUT (freeglut)**.

### O que instalar

**Linux (Ubuntu/Debian):**

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev mesa-common-dev
```

**Linux (Fedora):**

```bash
sudo dnf install gcc freeglut-devel mesa-libGLU-devel
```

**Linux (Arch):**

```bash
sudo pacman -S base-devel freeglut glu
```

**macOS:**

```bash
xcode-select --install
```

No macOS o GLUT já vem com o sistema (`-framework GLUT`).

**Windows (MSYS2):**

Abra o terminal **MSYS2 UCRT64** e rode:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut
```

### Como compilar

**Linux:**

```bash
gcc heli.c -o heli -lglut -lGLU -lGL -lm
```

```bash
gcc helice-3d.c -o helice-3d -lglut -lGLU -lGL -lm
```

**macOS:**

```bash
gcc heli.c -o heli -framework OpenGL -framework GLUT -Wno-deprecated
```

```bash
gcc helice-3d.c -o helice-3d -framework OpenGL -framework GLUT -Wno-deprecated
```

**Windows (MSYS2):**

```bash
gcc heli.c -o heli.exe -lfreeglut -lglu32 -lopengl32 -lm
```

```bash
gcc helice-3d.c -o helice-3d.exe -lfreeglut -lglu32 -lopengl32 -lm
```

### Como executar

**Linux / macOS:**

```bash
./heli
```

```bash
./helice-3d
```

**Windows:**

```bash
./heli.exe
```

### Controles

| Ação | Função |
|---|---|
| Arrastar com o botão esquerdo do mouse | Rotaciona a câmera |
| Scroll do mouse | Zoom in / zoom out |
| Tecla `A` | Liga/desliga a rotação automática |
| Tecla `ESC` ou `Q` | Sai do programa |

---

## Explicação das flags do GCC

| Flag | Significado |
|---|---|
| `-o nome` | Define o nome do executável gerado |
| `-lm` | Liga a biblioteca de matemática |
| `-lglut` | Liga o GLUT (janela e eventos) |
| `-lGLU` | Liga o GLU (`gluPerspective`) |
| `-lGL` | Liga o OpenGL |
| `-framework` | Equivalente às flags acima no macOS |

---

## Problemas comuns

**`undefined reference to 'sin'`**
Faltou a flag `-lm` no final do comando.

**`fatal error: GL/glut.h: No such file or directory`**
O freeglut não está instalado. Rode o comando de instalação da Parte 2.

**`implicit declaration of function 'exit'`**
Os arquivos `heli.c` e `helice-3d.c` usam `exit(0)` sem incluir `<stdlib.h>`. Adicione esta linha no topo do arquivo:

```c
#include <stdlib.h>
```

**`cannot open display` / janela não abre no WSL**
Use o WSL2 com WSLg (Windows 11) ou instale um servidor X.

**Cubo cortado ou bagunçado no terminal**
Aumente a janela do terminal para pelo menos 100x44 caracteres.

---

## Executando tudo de uma vez (opcional)

Crie um arquivo `Makefile` na mesma pasta:

```makefile
CC = gcc

all: cubo3d openclaude heli helice-3d

cubo3d: cubo3d.c
	$(CC) cubo3d.c -o cubo3d -lm

openclaude: openclaude.c
	$(CC) openclaude.c -o openclaude -lm

heli: heli.c
	$(CC) heli.c -o heli -lglut -lGLU -lGL -lm

helice-3d: helice-3d.c
	$(CC) helice-3d.c -o helice-3d -lglut -lGLU -lGL -lm

clean:
	rm -f cubo3d openclaude heli helice-3d
```

Depois é só rodar:

```bash
make
```

E para apagar os executáveis:

```bash
make clean
```

---

## Licença

Uso livre para estudo e aprendizado.
