#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

float A, B, C;

float larguraCubo = 20;
int larguradatela = 100, alturadatela = 44; // Ajustado para terminais padrão

// Corrigido: Agora o tamanho dos buffers segue as variáveis da tela
float bufferProfundidade[100 * 44]; 
char bufferTela[100 * 44];          

int caracterefundo = ' ';
int distanciacam = 100;
float k1 = 40;

float velocidadeRotacao = 0.6; 

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) -
           k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) +
           k * sin(A) * sin(C) +
           i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) +
           k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) +
           k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) -
           j * sin(A) * cos(B) +
           i * sin(B);
}

void calcularParaSuperficie(float cubox, float cuboy, float cuboz, int ch) {
    x = calculateX(cubox, cuboy, cuboz);
    y = calculateY(cubox, cuboy, cuboz);
    z = calculateZ(cubox, cuboy, cuboz) + distanciacam;

    ooz = 1 / z;

    xp = (int)(larguradatela / 2 + k1 * ooz * x * 2); 
    yp = (int)(alturadatela / 2 + k1 * ooz * y);

    idx = xp + yp * larguradatela;
    
    // Trava de segurança essencial para evitar Falha de Segmentação
    if (idx >= 0 && idx < larguradatela * alturadatela) {
        if (ooz > bufferProfundidade[idx]) {
            bufferProfundidade[idx] = ooz;
            bufferTela[idx] = ch;
        }
    }
}

int main() {
    printf("\x1b[2J"); 
    
    while (1) {
        memset(bufferTela, caracterefundo, larguradatela * alturadatela);
        memset(bufferProfundidade, 0, larguradatela * alturadatela * 4); 

        // Renderizando as 6 faces do cubo
        for (float cubox = -larguraCubo; cubox < larguraCubo; cubox += velocidadeRotacao) {
            for (float cuboy = -larguraCubo; cuboy < larguraCubo; cuboy += velocidadeRotacao) {
                calcularParaSuperficie(cubox, cuboy, -larguraCubo, '.');
                calcularParaSuperficie(larguraCubo, cuboy, cubox, '$');
                calcularParaSuperficie(-larguraCubo, cuboy, -cubox, '*');
                calcularParaSuperficie(-cubox, cuboy, larguraCubo, '#');
                calcularParaSuperficie(cubox, -larguraCubo, -cuboy, '!');
                calcularParaSuperficie(cubox, larguraCubo, cuboy, '+');
            }
        }

        printf("\x1b[H"); 
        
        for (int k = 0; k < larguradatela * alturadatela; k++) {
            putchar(k % larguradatela ? bufferTela[k] : 10);
        }

        A += 0.05; 
        B += 0.05;
        C += 0.01; // Adicionei rotação no eixo C para ficar mais dinâmico
        
        usleep(15000); 
    }
    return 0;
}
