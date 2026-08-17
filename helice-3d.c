/* ============================================================================
 * HELICE_3D.C
 * ----------------------------------------------------------------------------
 * Renderizacao de uma Helice Cilindrica 3D usando OpenGL Legacy + GLUT.
 *
 * Equacoes parametricas da helice cilindrica:
 *
 *      x(t) = r * cos(t)
 *      y(t) = r * sin(t)
 *      z(t) = c * t
 *
 * onde:
 *      r -> raio da helice (distancia radial ao eixo Z)
 *      c -> passo vertical (controla o "espacamento" entre as voltas)
 *      t -> parametro que varre o angulo (e portanto a altura, via z = c*t)
 *
 * Controles:
 *      Mouse (botao esquerdo + arrastar) -> rotaciona a camera manualmente
 *      Tecla 'a'                          -> liga/desliga a rotacao automatica
 *      Tecla ESC ou 'q'                   -> encerra o programa
 *
 * Autor: Gerado como exemplo didatico de Computacao Grafica em C/OpenGL.
 * ==========================================================================*/

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

#define HELIX_RADIUS        1.5f      //   r: raio da helice
#define HELIX_PITCH         0.35f     //   c: passo vertical (altura por radiano)
#define HELIX_TURNS         6.0f      //   numero de voltas completas da helice  
#define HELIX_STEP          0.01f     //   passo (delta t) do loop -> fluidez!   
/* Um passo pequeno (0.01 rad) gera milhares de vertices ao longo da helice,
 * eliminando qualquer aparencia de "quinas" na curva.                        */

#define AXIS_LENGTH         5.0f      /* comprimento visual dos eixos X,Y,Z    */

#define LINE_WIDTH_HELIX    2.5f      /* espessura da linha da helice          */
#define LINE_WIDTH_AXIS     2.0f      /* espessura da linha dos eixos          */

#define AUTO_ROTATE_SPEED   0.4f      /* graus por frame na rotacao automatica */


static float g_rotX = 25.0f;   /* rotacao acumulada em torno do eixo X (pitch) */
static float g_rotY = -35.0f;  /* rotacao acumulada em torno do eixo Y (yaw)   */
static float g_zoom = -12.0f;  /* distancia da camera (translacao em Z)        */

static bool  g_autoRotate = true;   /* liga/desliga a rotacao automatica        */
static bool  g_mouseDown  = false;
static int   g_lastMouseX = 0;
static int   g_lastMouseY = 0;

static int   g_windowWidth  = 900;
static int   g_windowHeight = 700;

/* ============================================================================
 * FUNCAO: initGL
 * Configura o estado inicial do OpenGL: cor de fundo, profundidade,
 * antialiasing de linhas (GL_LINE_SMOOTH) e blending (necessario para o
 * antialiasing funcionar corretamente).
 * ==========================================================================*/
static void initGL(void)
{
    /* Fundo escuro tipo "cinza chumbo" para dar destaque a helice colorida */
    glClearColor(0.08f, 0.09f, 0.11f, 1.0f);

    glEnable(GL_DEPTH_TEST);   /* Teste de profundidade para o 3D correto     */
    glShadeModel(GL_SMOOTH);   /* Sombreamento suave (interpolacao de cores)  */

    /* --- Antialiasing de linhas: deixa as curvas suaves e sem serrilhado --- */
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    /* Blending necessario para o LINE_SMOOTH funcionar com transparencia
     * correta nas bordas anti-aliased das linhas.                           */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Pontos tambem podem ficar suaves (usado nas "juntas" da helice) */
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

 // Desenha os tres eixos cartesianos cruzando a origem, seguindo o padrao
 // classico de cores: X = Vermelho, Y = Verde, Z = Azul.
 // Tambem desenha pequenas "pontas de seta" para indicar o sentido positivo.

static void drawAxes(float length)
{
    glLineWidth(LINE_WIDTH_AXIS);

    glBegin(GL_LINES);

    // Eixo X - Vermelho 
    glColor3f(1.0f, 0.15f, 0.15f);
    glVertex3f(-length, 0.0f, 0.0f);
    glVertex3f( length, 0.0f, 0.0f);

    // Eixo Y - Verde 
    glColor3f(0.15f, 1.0f, 0.15f);
    glVertex3f(0.0f, -length, 0.0f);
    glVertex3f(0.0f,  length, 0.0f);

    // Eixo Z - Azul
    glColor3f(0.25f, 0.45f, 1.0f);
    glVertex3f(0.0f, 0.0f, -length);
    glVertex3f(0.0f, 0.0f,  length);

    glEnd();

    /* Pequenas pontas de seta (cones simples feitos com linhas) indicando
     * o sentido positivo de cada eixo, para melhor leitura da orientacao. */
    float tip = length;
    float s = 0.15f; /* tamanho da "seta" */

    glBegin(GL_LINES);
    /* Seta do eixo X (vermelho) */
    glColor3f(1.0f, 0.15f, 0.15f);
    glVertex3f(tip, 0, 0); glVertex3f(tip - s, s, 0);
    glVertex3f(tip, 0, 0); glVertex3f(tip - s, -s, 0);

    /* Seta do eixo Y (verde) */
    glColor3f(0.15f, 1.0f, 0.15f);
    glVertex3f(0, tip, 0); glVertex3f(s, tip - s, 0);
    glVertex3f(0, tip, 0); glVertex3f(-s, tip - s, 0);

    /* Seta do eixo Z (azul) */
    glColor3f(0.25f, 0.45f, 1.0f);
    glVertex3f(0, 0, tip); glVertex3f(s, 0, tip - s);
    glVertex3f(0, 0, tip); glVertex3f(-s, 0, tip - s);
    glEnd();
}


 // Aqui esta o coracao matematico do programa: percorremos o parametro "t"
 // de 0 ate (HELIX_TURNS * 2*PI) com um passo muito pequeno (HELIX_STEP),
// calculando a cada iteracao a posicao (x, y, z) via as equacoes
// parametricas da helice cilindrica. Como o passo e pequeno, milhares de
 // segmentos de reta minusculos sao gerados, e com GL_LINE_SMOOTH ativo o
 // resultado visual e uma curva perfeitamente lisa.

 // Um leve gradiente de cor (de magenta para ciano) e aplicado ao longo do
 // comprimento da helice para reforcar a percepcao de profundidade/altura.
static void drawHelix(void)
{
    const float tMax = HELIX_TURNS * 2.0f * (float)M_PI;

    /* Centraliza a helice verticalmente em torno da origem */
    const float zOffset = (HELIX_PITCH * tMax) * 0.5f;

    glLineWidth(LINE_WIDTH_HELIX);

    glBegin(GL_LINE_STRIP);
    for (float t = 0.0f; t <= tMax; t += HELIX_STEP)
    {
        /* ---- MATEMATICA DA HELICE APLICADA AQUI ---- */
        float x = HELIX_RADIUS * cosf(t);   /* x(t) = r * cos(t) */
        float y = HELIX_RADIUS * sinf(t);   /* y(t) = r * sin(t) */
        float z = HELIX_PITCH  * t - zOffset; /* z(t) = c * t (centralizado) */
        /* ---------------------------------------------- */

        /* Gradiente de cor suave ao longo do parametro t: de magenta a ciano.
         * Isso da um efeito visual "vibrante" e ajuda a perceber a
         * progressao ao longo da helice.                                    */
        float progress = t / tMax; /* varia de 0.0 a 1.0 */
        float red   = 1.0f - progress;        /* comeca em 1, termina em 0   */
        float green = progress;               /* comeca em 0, termina em 1   */
        float blue  = 1.0f;                   /* canal azul sempre no maximo */

        glColor3f(red, green, blue);
        glVertex3f(x, y, z);
    }
    glEnd();
}

/* ============================================================================
 * FUNCAO: drawGroundGrid (opcional, puramente estetico)
 * Desenha uma grade discreta no plano XY para dar mais referencia espacial
 * a cena, sem poluir visualmente o resultado.
 * ==========================================================================*/
static void drawGroundGrid(void)
{
    glLineWidth(1.0f);
    glColor4f(0.35f, 0.37f, 0.40f, 0.35f);

    const float extent = AXIS_LENGTH;
    const float step   = 0.5f;

    glBegin(GL_LINES);
    for (float v = -extent; v <= extent; v += step)
    {
        glVertex3f(v, -extent, -3.5f);
        glVertex3f(v,  extent, -3.5f);

        glVertex3f(-extent, v, -3.5f);
        glVertex3f( extent, v, -3.5f);
    }
    glEnd();
}

/* ============================================================================
 * FUNCAO: display
 * Callback principal de renderizacao do GLUT. Configura a camera (view),
 * aplica as rotacoes acumuladas e desenha a cena.
 * ==========================================================================*/
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    /* Afasta a camera da origem para enxergarmos toda a cena */
    glTranslatef(0.0f, 0.0f, g_zoom);

    /* Aplica as rotacoes da camera (manual via mouse e/ou automatica) */
    glRotatef(g_rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(g_rotY, 0.0f, 1.0f, 0.0f);

    drawGroundGrid();
    drawAxes(AXIS_LENGTH);
    drawHelix();

    glutSwapBuffers();
}

/* ============================================================================
 * FUNCAO: reshape
 * Callback chamado quando a janela e redimensionada. Ajusta o viewport e
 * a matriz de projecao em perspectiva.
 * ==========================================================================*/
static void reshape(int width, int height)
{
    if (height == 0) height = 1;

    g_windowWidth  = width;
    g_windowHeight = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* ============================================================================
 * FUNCAO: timer
 * Callback de temporizacao usado para animar a rotacao automatica da cena
 * e manter uma taxa de atualizacao constante (~60 FPS).
 * ==========================================================================*/
static void timer(int value)
{
    (void)value;

    if (g_autoRotate)
    {
        g_rotY += AUTO_ROTATE_SPEED;
        if (g_rotY > 360.0f) g_rotY -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); /* ~60 FPS (16 ms por frame) */
}

/* ============================================================================
 * FUNCOES DE INTERACAO: MOUSE E TECLADO
 * ==========================================================================*/
static void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        g_mouseDown = (state == GLUT_DOWN);
        g_lastMouseX = x;
        g_lastMouseY = y;
    }
    else if (button == 3) /* Scroll para cima -> zoom in */
    {
        g_zoom += 0.5f;
    }
    else if (button == 4) /* Scroll para baixo -> zoom out */
    {
        g_zoom -= 0.5f;
    }
}

static void mouseMotion(int x, int y)
{
    if (g_mouseDown)
    {
        int dx = x - g_lastMouseX;
        int dy = y - g_lastMouseY;

        g_rotY += dx * 0.4f;
        g_rotX += dy * 0.4f;

        g_lastMouseX = x;
        g_lastMouseY = y;
    }
}

static void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;

    switch (key)
    {
        case 'a':
        case 'A':
            g_autoRotate = !g_autoRotate; /* Liga/desliga rotacao automatica */
            break;

        case 27:  /* ESC */
        case 'q':
        case 'Q':
            exit(0);
            break;

        default:
            break;
    }
}

/* ============================================================================
 * FUNCAO: main
 * Ponto de entrada: inicializa o GLUT, cria a janela com buffer duplo e
 * suporte a RGBA + Depth, registra os callbacks e inicia o loop principal.
 * ==========================================================================*/
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    /* Double buffer (evita flicker) + RGBA + Depth buffer para o 3D */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_windowWidth, g_windowHeight);
    glutCreateWindow("Helice Cilindrica 3D - x(t)=r*cos(t), y(t)=r*sin(t), z(t)=c*t");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    printf(" Helice Cilindrica 3D\n");
    printf("---------------------------------------------------------\n");
    printf(" Mouse (arrastar botao esquerdo) : rotacionar camera\n");
    printf(" Scroll do mouse                 : zoom in/out\n");
    printf(" Tecla 'a'                       : liga/desliga rotacao auto\n");
    printf(" Tecla ESC ou 'q'                : sair\n");

    glutMainLoop();

    return 0;
}
