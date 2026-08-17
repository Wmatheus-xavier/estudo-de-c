#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#if defined(_WIN32)
#include <windows.h>  
#include <GL/glut.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989
#endif

#define RAIO_HELICE             1.5f
#define PASSO_HELICE            0.35f
#define VOLTAS_HELICE           6.0f
#define INCREMENTO_HELICE       0.01f
#define COMPRIMENTO_EIXO        5.0f
#define LARGURA_LINHA_HELICE    2.5f
#define LARGURA_LINHA_EIXOS     2.0f
#define VELOCIDADE_ROTACAO_AUTO 0.4f

static float g_rotacaoX = 25.0f;
static float g_rotacaoY = -35.0f;
static float g_zoom     = -12.0f;

static bool  g_rotacaoAutomatica = true;
static bool  g_mousePressionado  = false;
static int   g_ultimoMouseX      = 0;
static int   g_ultimoMouseY      = 0;

static int   g_larguraJanela = 900;
static int   g_alturaJanela  = 700;

static void inicializarGL(void)
{
    glClearColor(0.08f, 0.09f, 0.11f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

static void desenharEixos(float comprimento)
{
    glLineWidth(LARGURA_LINHA_EIXOS);

    glBegin(GL_LINES);

    glColor3f(1.0f, 0.15f, 0.15f);
    glVertex3f(-comprimento, 0.0f, 0.0f);
    glVertex3f( comprimento, 0.0f, 0.0f);

    glColor3f(0.15f, 1.0f, 0.15f);
    glVertex3f(0.0f, -comprimento, 0.0f);
    glVertex3f(0.0f,  comprimento, 0.0f);

    glColor3f(0.25f, 0.45f, 1.0f);
    glVertex3f(0.0f, 0.0f, -comprimento);
    glVertex3f(0.0f, 0.0f,  comprimento);

    glEnd();

    float ponta = comprimento;
    float tamanhoSeta = 0.15f;

    glBegin(GL_LINES);

    glColor3f(1.0f, 0.15f, 0.15f);
    glVertex3f(ponta, 0, 0); glVertex3f(ponta - tamanhoSeta, tamanhoSeta, 0);
    glVertex3f(ponta, 0, 0); glVertex3f(ponta - tamanhoSeta, -tamanhoSeta, 0);

    glColor3f(0.15f, 1.0f, 0.15f);
    glVertex3f(0, ponta, 0); glVertex3f(tamanhoSeta, ponta - tamanhoSeta, 0);
    glVertex3f(0, ponta, 0); glVertex3f(-tamanhoSeta, ponta - tamanhoSeta, 0);

    glColor3f(0.25f, 0.45f, 1.0f);
    glVertex3f(0, 0, ponta); glVertex3f(tamanhoSeta, 0, ponta - tamanhoSeta);
    glVertex3f(0, 0, ponta); glVertex3f(-tamanhoSeta, 0, ponta - tamanhoSeta);
    
    glEnd();
}

static void desenharHelice(void)
{
    const float tMaximo = VOLTAS_HELICE * 2.0f * (float)M_PI;
    const float deslocamentoZ = (PASSO_HELICE * tMaximo) * 0.5f;

    glLineWidth(LARGURA_LINHA_HELICE);

    glBegin(GL_LINE_STRIP);
    for (float t = 0.0f; t <= tMaximo; t += INCREMENTO_HELICE)
    {
        float x = RAIO_HELICE * cosf(t);
        float y = RAIO_HELICE * sinf(t);
        float z = PASSO_HELICE * t - deslocamentoZ;

        float progresso = t / tMaximo;
        float vermelho = 1.0f - progresso;
        float verde = progresso;
        float azul = 1.0f;

        glColor3f(vermelho, verde, azul);
        glVertex3f(x, y, z);
    }
    glEnd();
}

static void desenharGradeChao(void)
{
    glLineWidth(1.0f);
    glColor4f(0.35f, 0.37f, 0.40f, 0.35f);

    const float extensao = COMPRIMENTO_EIXO;
    const float passo = 0.5f;

    glBegin(GL_LINES);
    for (float v = -extensao; v <= extensao; v += passo)
    {
        glVertex3f(v, -extensao, -3.5f);
        glVertex3f(v,  extensao, -3.5f);

        glVertex3f(-extensao, v, -3.5f);
        glVertex3f( extensao, v, -3.5f);
    }
    glEnd();
}

static void exibir(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, g_zoom);

    glRotatef(g_rotacaoX, 1.0f, 0.0f, 0.0f);
    glRotatef(g_rotacaoY, 0.0f, 1.0f, 0.0f);

    desenharGradeChao();
    desenharEixos(COMPRIMENTO_EIXO);
    desenharHelice();

    glutSwapBuffers();
}

static void redimensionar(int largura, int altura)
{
    if (altura == 0) altura = 1;

    g_larguraJanela = largura;
    g_alturaJanela = altura;

    glViewport(0, 0, largura, altura);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float proporcao = (float)largura / (float)altura;
    gluPerspective(45.0, proporcao, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void temporizador(int valor)
{
    (void)valor;

    if (g_rotacaoAutomatica)
    {
        g_rotacaoY += VELOCIDADE_ROTACAO_AUTO;
        if (g_rotacaoY > 360.0f) g_rotacaoY -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, temporizador, 0);
}

static void botaoMouse(int botao, int estado, int x, int y)
{
    if (botao == GLUT_LEFT_BUTTON)
    {
        g_mousePressionado = (estado == GLUT_DOWN);
        g_ultimoMouseX = x;
        g_ultimoMouseY = y;
    }
    else if (botao == 3)
    {
        g_zoom += 0.5f;
    }
    else if (botao == 4)
    {
        g_zoom -= 0.5f;
    }
}

static void movimentoMouse(int x, int y)
{
    if (g_mousePressionado)
    {
        int dx = x - g_ultimoMouseX;
        int dy = y - g_ultimoMouseY;

        g_rotacaoY += dx * 0.4f;
        g_rotacaoX += dy * 0.4f;

        g_ultimoMouseX = x;
        g_ultimoMouseY = y;
    }
}

static void teclado(unsigned char tecla, int x, int y)
{
    (void)x; (void)y;

    switch (tecla)
    {
        case 'a':
        case 'A':
            g_rotacaoAutomatica = !g_rotacaoAutomatica;
            break;

        case 27:
        case 'q':
        case 'Q':
            exit(0);
            break;

        default:
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_larguraJanela, g_alturaJanela);
    glutCreateWindow("Helice Cilindrica 3D - x(t)=r*cos(t), y(t)=r*sin(t), z(t)=c*t");

    inicializarGL();

    glutDisplayFunc(exibir);
    glutReshapeFunc(redimensionar);
    glutMouseFunc(botaoMouse);
    glutMotionFunc(movimentoMouse);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, temporizador, 0);

    printf(" Helice Cilindrica 3D\n");
    printf("---------------------------------------------------------\n");
    printf(" Mouse (arrastar botao esquerdo) : rotacionar camera\n");
    printf(" Scroll do mouse                 : zoom in/out\n");
    printf(" Tecla 'a'                       : liga/desliga rotacao auto\n");
    printf(" Tecla ESC ou 'q'                : sair\n");

    glutMainLoop();

    return 0;
}