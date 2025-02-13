// Example2_4.cpp : A bouncing ball

// #include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>
#include <stdio.h>

#define PI 3.1415926535898

// Posicion x y y de la pelota
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
GLfloat RadiusOfBall = 10.;

// Funcion para dibujar la pelota, centrada en el origen y con color blanco
void draw_ball()
{
  glColor3f(1.0, 1.0, 1.0);
  MyCircle2f(0., 0., RadiusOfBall);
}

// Funcion para mostrar en la ventana
void Display(void)
{
  // Intercambia los buffers
  glutSwapBuffers();

  // Limpia todos los pixeles con el color de limpieza especificado
  glClear(GL_COLOR_BUFFER_BIT);

  // Logica del movimiento de la pelota

  // Movimiento de la pelota en el eje x
  xpos = xpos + xdir * speed;

  // Movimiento de la pelota en el eje y
  ypos = ypos + ydir * speed;

  // Experimental, aumentar la velocidad de la pelota
  speed += 0.001;

  // Si toca la pared izquierda, cambia la direccion de la pelota hacia la derecha
  if (xpos >= xsize - RadiusOfBall)
  {
    xdir = -1;
    // Imprimomes en consola que choco con la pared izquierda
    printf("Choco con la pared izquierda\n");
    printf("la velocidad actual es: %f\n", speed);
  }

  // Si toca la pared derecha, cambia la direccion de la pelota hacia la izquierda
  else if (xpos <= RadiusOfBall)
    xdir = 1;

  // Si toca el techo, cambia la direccion de la pelota hacia abajo
  if (ypos >= ysize - RadiusOfBall)
    ydir = -1;

  // Si toca el suelo, cambia la direccion de la pelota hacia arriba
  else if (ypos <= RadiusOfBall)
    ydir = 1;

  // Aplicamos la transformacion de la pelota
  T[12] = xpos;
  T[13] = ypos;
  glLoadMatrixf(T);

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

  // Dibujar la pelota
  draw_ball();
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

// Funcion para inicializar la ventana y parametros de la pelota
void init(void)
{
  // Pone el color de limpieza en negro?
  glClearColor(0.0, 0.0, 0.0, 1.0);
  // Inicializa la posicion en el centro y parametros de la pelota
  xpos = 170;
  ypos = 240;
  xdir = 1;
  ydir = 1;
  sx = 1.;
  sy = 1.;
  squash = 0.9;
  rot = 0;
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