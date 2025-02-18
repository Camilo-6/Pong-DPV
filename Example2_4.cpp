// Example2_4.cpp : A bouncing ball

// #include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>
#include <stdio.h>

// numeros aleatorios
#include <cstdlib>
// tiempo
#include <ctime>

#define PI 3.1415926535898

// Posicion x y y de la pelota, son el centro de la pelota
double xpos, ypos;
// Direccion en x y y de la pelota
double ydir, xdir;
// Factores de escala en x y y
double sx, sy, squash;
// Rotacion
double rot, rdir;
// Velocidad de la pelota
double speed = 3.0;
// Tamanio x del juego
int xsize = 640;
// Tamanio y del juego
int ysize = 480;
// Posicion x y y del rectangulo izquierdo, son el centro del rectangulo
double rectIzqXpos, rectIzqYpos;
// Direccion en x del rectangulo izquierdo
double rectIzqXdir;
// Posicion x y y del rectangulo derecho, son el centro del rectangulo
double rectDerXpos, rectDerYpos;
// Direccion en x del rectangulo derecho
double rectDerXdir;
// Velocidad de los rectangulos
double rectSpeed = 2.0;

// Matrices de transformacion
GLfloat T1[16] = {1., 0., 0., 0.,
                  0., 1., 0., 0.,
                  0., 0., 1., 0.,
                  0., 0., 0., 1.};
GLfloat S[16] = {1., 0., 0., 0.,
                 0., 1., 0., 0.,
                 0., 0., 1., 0.,
                 0., 0., 0., 1.};
GLfloat T[16] = {1., 0., 0., 0.,
                 0., 1., 0., 0.,
                 0., 0., 1., 0.,
                 0., 0., 0., 1.};

#define PI 3.1415926535898

// Numero de puntos para dibujar un circulo
GLint circle_points = 100;

// Funcion para dibujar un circulo, centrado en (centerx, centery) y radio radius
void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius)
{
  GLint i;
  GLdouble angle;
  glBegin(GL_POLYGON);
  for (i = 0; i < circle_points; i++)
  {
    angle = 2 * PI * i / circle_points;
    glVertex2f(centerx + radius * cos(angle), centery + radius * sin(angle));
  }
  glEnd();
}

// Radio de la pelota
GLfloat RadiusOfBall = 10.0;

// Funcion para dibujar la pelota, con color blanco
void draw_ball()
{
  glColor3f(1.0, 1.0, 1.0);
  MyCircle2f(0.0, 0.0, RadiusOfBall);
}

// Alto y ancho del rectangulo
GLfloat Alto = 60.0;
GLfloat Ancho = 10.0;

// Funcion para dibujar un rectangulo blanco
void draw_rectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  GLfloat hwidth = width / 2.0;
  GLfloat hheight = height / 2.0;
  glVertex2f(x - hwidth, y - hheight); // Esquina inferior izquierda
  glVertex2f(x + hwidth, y - hheight); // Esquina inferior derecha
  glVertex2f(x + hwidth, y + hheight); // Esquina superior derecha
  glVertex2f(x - hwidth, y + hheight); // Esquina superior izquierda
  glEnd();
}

// Funcion para mover al circulo
void moverCirculo()
{
  // Movimiento de la pelota en el eje x
  xpos = xpos + xdir * speed;

  // Movimiento de la pelota en el eje y
  ypos = ypos + ydir * speed;

  // Experimental, aumentar la velocidad de la pelota
  speed += 0.001;
}

// Funcion para detectar colisiones entre el circulo y las paredes
void colisionesCirculoParedes()
{
  // Si toca la pared izquierda, cambia la direccion de la pelota hacia la derecha
  if (xpos >= xsize - RadiusOfBall)
  {
    xdir = -1;
    // Imprimimos en consola que choco con la pared izquierda
    printf("Choco con la pared izquierda\n");
  }

  // Si toca la pared derecha, cambia la direccion de la pelota hacia la izquierda
  else if (xpos <= RadiusOfBall)
  {
    xdir = 1;
    // Imprimimos en consola que choco con la pared izquierda
    printf("Choco con la pared izquierda\n");
  }

  // Si toca el techo, cambia la direccion de la pelota hacia abajo
  if (ypos >= ysize - RadiusOfBall)
    ydir = -1;

  // Si toca el suelo, cambia la direccion de la pelota hacia arriba
  else if (ypos <= RadiusOfBall)
    ydir = 1;
}

// Funcion para detectar colisiones entre el circulo y el rectangulo izquierdo
void colisionesCirculoRectIzq()
{
  // Obtenemos las esquinas del circulo
  GLfloat x1 = xpos - RadiusOfBall; // Esquina inferior izquierda
  GLfloat x2 = xpos + RadiusOfBall; // Esquina inferior derecha
  GLfloat y1 = ypos - RadiusOfBall; // Esquina superior izquierda
  GLfloat y2 = ypos + RadiusOfBall; // Esquina superior derecha

  // Obtenemos las esquinas del rectangulo
  GLfloat rectIzqX1 = rectIzqXpos - Ancho / 2.0; // Esquina inferior izquierda
  GLfloat rectIzqX2 = rectIzqXpos + Ancho / 2.0; // Esquina inferior derecha
  GLfloat rectIzqY1 = rectIzqYpos - Alto / 2.0;  // Esquina superior izquierda
  GLfloat rectIzqY2 = rectIzqYpos + Alto / 2.0;  // Esquina superior derecha

  // Revisamos si hay colision
  if (!(x1 < rectIzqX2 and x2 > rectIzqX1 and y1 < rectIzqY2 and y2 > rectIzqY1))
  {
    // Si no hay colision, no hacemos nada
    return;
  }

  // Detectamos con que lado del rectangulo choco
  // Calculamos penetracion en x
  GLfloat dx = 0.0;
  GLfloat aux1 = x2 - rectIzqX1;
  GLfloat aux2 = rectIzqX2 - x1;
  dx = (aux1 < aux2) ? aux1 : aux2;
  // Calculamos penetracion en y
  GLfloat dy = 0.0;
  aux1 = y2 - rectIzqY1;
  aux2 = rectIzqY2 - y1;
  dy = (aux1 < aux2) ? aux1 : aux2;
  // TODO
}

// Funcion para mostrar en la ventana
void Display(void)
{
  // Intercambia los buffers
  glutSwapBuffers();

  // Limpia todos los pixeles con el color de limpieza especificado
  glClear(GL_COLOR_BUFFER_BIT);

  // Movemos la pelota
  moverCirculo();

  // Detectar colisiones con los rectangulos, usaremos el circulo como un cuadrado por simplicidad
  colisionesCirculoRectIzq();
  // rectangulo derecho

  // Detectar colisiones con las paredes
  colisionesCirculoParedes();

  /*
  // 160 is max X value in our world
  // Define X position of the ball to be at center of window
  xpos = 80.;

  // Shape has hit the ground! Stop moving and start squashing down and then back up
  if (ypos == RadiusOfBall && ydir == -1)
  {
    sy = sy * squash;

    if (sy < 0.8)
      // reached maximum suqash, now unsquash back up
      squash = 1.1;
    else if (sy > 1.)
    {
      // reset squash parameters and bounce ball back upwards
      sy = 1.;
      squash = 0.9;
      ydir = 1;
    }
    sx = 1. / sy;
  }
  // 120 is max Y value in our world
  // set Y position to increment 1.5 times the direction of the bounce
  else
  {
    ypos = ypos + ydir * 1.5 - (1. - sy) * RadiusOfBall;
    // If ball touches the top, change direction of ball downwards
    if (ypos == 120 - RadiusOfBall)
      ydir = -1;
    // If ball touches the bottom, change direction of ball upwards
    else if (ypos < RadiusOfBall)
      ydir = 1;
  }

  /*  //reset transformation state
    glLoadIdentity();

    // apply translation
    glTranslatef(xpos,ypos, 0.);

    // Translate ball back to center
    glTranslatef(0.,-RadiusOfBall, 0.);
    // Scale the ball about its bottom
    glScalef(sx,sy, 1.);
    // Translate ball up so bottom is at the origin
    glTranslatef(0.,RadiusOfBall, 0.);
    // draw the ball
    draw_ball();
  */

  /*
  // Translate the bouncing ball to its new position
  T[12] = xpos;
  T[13] = ypos;
  glLoadMatrixf(T);

  // Mueve la pelota a su nueva posicion?
  T1[13] = -RadiusOfBall;
  // Translate ball back to center
  glMultMatrixf(T1);
  S[0] = sx;
  S[5] = sy;
  // Scale the ball about its bottom
  glMultMatrixf(S);
  T1[13] = RadiusOfBall;
  // Translate ball up so bottom is at the origin
  glMultMatrixf(T1);
  */

  // Dibujar rectangulos
  // Guardar la matriz acutal
  glPushMatrix();
  // Dibujar los rectangulos
  draw_rectangle(rectIzqXpos, rectIzqYpos, Ancho, Alto);
  draw_rectangle(rectDerXpos, rectDerYpos, Ancho, Alto);
  // Restaurar la matriz
  glPopMatrix();

  // Dibujar la pelota
  // Guardar la matriz acutal
  glPushMatrix();
  T[12] = xpos;
  T[13] = ypos;
  // Aplicar transformacion
  glLoadMatrixf(T);
  // Dibujar la pelota
  draw_ball();
  // Restaurar la matriz
  glPopMatrix();

  // Actualizar la ventana
  glutPostRedisplay();
}

// Funcion para cambiar el tamano de la ventana y mantener un sistema de coordenadas constante de 640x480
void reshape(int w, int h)
{
  // On reshape and on startup, keep the viewport to be the entire size of the window
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Keep our logical coordinate system constant
  // Si cambiamos xsize o ysize, hay que cambiar estos valores
  gluOrtho2D(0.0, 640.0, 0.0, 480.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Funcion para inicializar la ventana, parametros de la pelota y de los rectangulos
void init(void)
{
  // Pone el color de limpieza en negro?
  glClearColor(0.0, 0.0, 0.0, 1.0);
  // Inicializa la posicion y parametros de la pelota
  xpos = xsize / 2;
  srand(time(0));
  ypos = rand() % (ysize - (int)RadiusOfBall * 2) + RadiusOfBall;
  printf("%f\n", ypos);
  xdir = (rand() % 2 == 0) ? 1 : -1;
  ydir = (rand() % 2 == 0) ? 1 : -1;
  sx = 1.;
  sy = 1.;
  squash = 0.9;
  rot = 0;
  // Inicializa la posicion de los rectangulos
  rectIzqXpos = 70;
  rectIzqYpos = 240;
  rectDerXpos = 570;
  rectDerYpos = 240;
}

// Funcion para que todo funcione
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  // Inicializa una ventana de 640x480
  glutInitWindowSize(xsize, ysize);
  glutCreateWindow("Pong?");
  init();
  // Registra las funciones de display y reshape
  glutDisplayFunc(Display);
  glutReshapeFunc(reshape);
  // Loop para correr
  glutMainLoop();

  return 1;
}