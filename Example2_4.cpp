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

// Posicion x y y del circulo, son el centro del circulo
double xpos, ypos;
// Direccion en x y y del circulo
double ydir, xdir;
// Factores de escala en x y y
double sx, sy, squash;
// Rotacion
double rot, rdir;
// Velocidades del circulo
double speed, inicialSpeed = 3.0;
// Tamanio x del juego
int xsize = 640;
// Tamanio y del juego
int ysize = 480;
// Posicion x y y del rectangulo izquierdo, son el centro del rectangulo
double rectIzqXpos, rectIzqYpos;
// Direccion en y del rectangulo izquierdo
double rectIzqYdir;
// Posicion x y y del rectangulo derecho, son el centro del rectangulo
double rectDerXpos, rectDerYpos;
// Direccion en y del rectangulo derecho
double rectDerYdir;
// Booleano para saber si el rectangulo izquierdo intenta ir arriba
bool rectIzqArriba = false;
// Booleano para saber si el rectangulo izquierdo intenta ir abajo
bool rectIzqAbajo = false;
// Booleano para saber si el rectangulo derecho intenta ir arriba
bool rectDerArriba = false;
// Booleano para saber si el rectangulo derecho intenta ir abajo
bool rectDerAbajo = false;
// Velocidad de los rectangulos, procurar que sea menor o igual a la velocidad del circulo para evitar colisiones extranias
double rectSpeed = 3.0;
// Puntaje de los jugadores
int puntIzq, puntDer = 0;

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

// Radio del circulo
GLfloat RadiusOfBall = 10.0;

// Funcion para dibujar el circulo, con color blanco
void draw_ball()
{
  glColor3f(1.0, 1.0, 1.0);
  MyCircle2f(0.0, 0.0, RadiusOfBall);
}

// Alto y ancho de los rectangulos
GLfloat Alto = 70.0;
GLfloat Ancho = 10.0;

// Funcion para dibujar un rectangulo, con color blanco
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

// Funcion para reiniciar la posicion, velocidad y direccion del circulo
void reiniciarCirculo()
{
  xpos = xsize / 2;
  srand(time(0));
  ypos = rand() % (ysize - (int)RadiusOfBall * 2) + RadiusOfBall;
  xdir = (rand() % 2 == 0) ? 1 : -1;
  ydir = (rand() % 2 == 0) ? 1 : -1;
  speed = inicialSpeed;
}

// Funcion para mover al circulo
void moverCirculo()
{
  // Movimiento en el eje x
  xpos = xpos + xdir * speed;

  // Movimiento en el eje y
  ypos = ypos + ydir * speed;

  // Experimental, aumentar la velocidad del circulo
  speed += 0.001;
}

// Funcion para reiniciar la posicion y direccion de los rectangulos
void reiniciarRectangulos()
{
  rectIzqXpos = 70;
  rectIzqYpos = 240;
  rectIzqYdir = 0;
  rectDerXpos = 570;
  rectDerYpos = 240;
  rectDerYdir = 0;
}

// Funcion para mover un rectangulo
void moverRectangulo(double &rectYpos, double rectYdir, double rectSpeed)
{
  // Movimiento en el eje y
  rectYpos = rectYpos + rectYdir * rectSpeed;

  // Si avanza arriba del techo, lo movemos abajo
  if (rectYpos > ysize - Alto / 2.0)
  {
    rectYpos = ysize - Alto / 2.0;
  }
  // Si avanza abajo del suelo, lo movemos arriba
  else if (rectYpos < Alto / 2.0)
  {
    rectYpos = Alto / 2.0;
  }
}

// Funcion para detectar colisiones entre el circulo y las paredes
void colisionesCirculoParedes()
{
  // Si toca la pared derecha
  if (xpos >= xsize - RadiusOfBall)
  {
    // Aumentamos el puntaje del jugador de la izquierda
    printf("Punto para el jugador de la izquierda\n");
    puntIzq++;
    printf("Puntaje: %d - %d\n", puntIzq, puntDer);
    // Reiniciamos el circulo
    reiniciarCirculo();
  }
  // Si toca la pared izquierda
  else if (xpos <= RadiusOfBall)
  {
    // Aumentamos el puntaje del jugador de la derecha
    printf("Punto para el jugador de la derecha\n");
    puntDer++;
    printf("Puntaje: %d - %d\n", puntIzq, puntDer);
    // Reiniciamos el circulo
    reiniciarCirculo();
  }
  // Si toca el techo, cambia la direccion del circulo hacia abajo
  if (ypos >= ysize - RadiusOfBall)
  {
    ydir = -ydir;
    // Ajustamos la posicion del circulo para que no se quede pegada al techo
    ypos = ysize - RadiusOfBall;
  }
  // Si toca el suelo, cambia la direccion del circulo hacia arriba
  else if (ypos <= RadiusOfBall)
  {
    ydir = -ydir;
    // Ajustamos la posicion del circulo para que no se quede pegada al suelo
    ypos = RadiusOfBall;
  }
}

// Funcion para detectar colisiones entre el circulo y un rectangulo, usando el circulo como un cuadrado por simplicidad
void colisionesCirculoRect(double rectX, double rectY)
{
  // Obtenemos las esquinas del circulo
  GLfloat x1 = xpos - RadiusOfBall; // Esquina inferior izquierda
  GLfloat x2 = xpos + RadiusOfBall; // Esquina inferior derecha
  GLfloat y1 = ypos - RadiusOfBall; // Esquina superior izquierda
  GLfloat y2 = ypos + RadiusOfBall; // Esquina superior derecha

  // Obtenemos las esquinas del rectangulo
  GLfloat rectX1 = rectX - Ancho / 2.0; // Esquina inferior izquierda
  GLfloat rectX2 = rectX + Ancho / 2.0; // Esquina inferior derecha
  GLfloat rectY1 = rectY - Alto / 2.0;  // Esquina superior izquierda
  GLfloat rectY2 = rectY + Alto / 2.0;  // Esquina superior derecha

  // Revisamos si hay colision
  if (!(x1 < rectX2 and x2 > rectX1 and y1 < rectY2 and y2 > rectY1))
  {
    // Si no hay colision, terminamos
    return;
  }

  // Detectamos de que lado fue la colision
  // Calculamos penetracion en x
  GLfloat dx = 0.0;
  GLfloat aux1 = x2 - rectX1;
  GLfloat aux2 = rectX2 - x1;
  dx = (aux1 < aux2) ? aux1 : aux2;
  // Calculamos penetracion en y
  GLfloat dy = 0.0;
  aux1 = y2 - rectY1;
  aux2 = rectY2 - y1;
  dy = (aux1 < aux2) ? aux1 : aux2;

  // Revisamos la direccion de la colision
  if (dx == dy)
  {
    // Si la colision fue en diagonal, cambiamos la direccion del circulo en x y y
    xdir = -xdir;
    ydir = -ydir;
    // Ajustamos la posicion del circulo para que no se quede pegada al rectangulo
    xpos = (xpos < rectX) ? rectX - Ancho / 2.0 - RadiusOfBall : rectX + Ancho / 2.0 + RadiusOfBall;
    ypos = (ypos < rectY) ? rectY - Alto / 2.0 - RadiusOfBall : rectY + Alto / 2.0 + RadiusOfBall;
  }
  else if (dx < dy)
  {
    // Si la colision fue en x, cambiamos la direccion del circulo en x
    xdir = -xdir;
    // Ajustamos la posicion del circulo para que no se quede pegada al rectangulo
    xpos = (xpos < rectX) ? rectX - Ancho / 2.0 - RadiusOfBall : rectX + Ancho / 2.0 + RadiusOfBall;
  }
  else
  {
    // Si la colision fue en y, cambiamos la direccion del circulo en y
    ydir = -ydir;
    // Ajustamos la posicion del circulo para que no se quede pegada al rectangulo
    ypos = (ypos < rectY) ? rectY - Alto / 2.0 - RadiusOfBall : rectY + Alto / 2.0 + RadiusOfBall;
  }
}

// TODO: intentar mejorar las colisiones entre el circulo y rectangulo para que el circulo ahora si sea un circulo

// Funcion para mostrar en la ventana
void Display(void)
{
  // Intercambia los buffers
  glutSwapBuffers();

  // Limpia todos los pixeles con el color de limpieza especificado
  glClear(GL_COLOR_BUFFER_BIT);

  // Mover los rectangulos
  moverRectangulo(rectIzqYpos, rectIzqYdir, rectSpeed);
  moverRectangulo(rectDerYpos, rectDerYdir, rectSpeed);

  // Movemos el circulo
  moverCirculo();

  // Detectar colisiones con los rectangulos
  colisionesCirculoRect(rectIzqXpos, rectIzqYpos);
  colisionesCirculoRect(rectDerXpos, rectDerYpos);

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

  // Mueve el circulo a su nueva posicion?
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
  // Aplicar transformacion
  glTranslatef(rectIzqXpos, rectIzqYpos, 0);
  // Dibujar el rectangulo
  draw_rectangle(0, 0, Ancho, Alto);
  // Restaurar la matriz
  glPopMatrix();
  // Guardar la matriz acutal
  glPushMatrix();
  // Aplicar transformacion
  glTranslatef(rectDerXpos, rectDerYpos, 0);
  // Dibujar el rectangulo
  draw_rectangle(0, 0, Ancho, Alto);
  // Restaurar la matriz
  glPopMatrix();

  // Dibujar circulo
  // Guardar la matriz acutal
  glPushMatrix();
  // Aplicar transformacion
  T[12] = xpos;
  T[13] = ypos;
  // Cambiar la matriz
  glLoadMatrixf(T);
  // Dibujar el circulo
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

// Funcion para inicializar la ventana, parametros del circulo y de los rectangulos
void init(void)
{
  // Pone el color de limpieza en negro?
  glClearColor(0.0, 0.0, 0.0, 1.0);
  // Inicializa la posicion y parametros del circulo
  reiniciarCirculo();
  sx = 1.;
  sy = 1.;
  squash = 0.9;
  rot = 0;
  // Inicializa la posicion y parametros de los rectangulos
  reiniciarRectangulos();
}

// Funcion para detectar cuando una tecla es presionada
void keyDown(unsigned char key, int x, int y)
{
  switch (key)
  {
  // Cuando se presiona la tecla w
  case 'w':
    // Se pone que se intenta ir arriba rectangulo izquierdo
    rectIzqArriba = true;
    if (rectIzqAbajo)
    {
      // Si tambien se intenta ir abajo, no se mueve
      rectIzqYdir = 0;
    }
    else
    {
      // Si solo intenta ir arriba, se mueve
      rectIzqYdir = 1;
    }
    break;
  // Cuando se presiona la tecla s
  case 's':
    // Se pone que se intenta ir abajo rectangulo izquierdo
    rectIzqAbajo = true;
    if (rectIzqArriba)
    {
      // Si tambien se intenta ir arriba, no se mueve
      rectIzqYdir = 0;
    }
    else
    {
      // Si solo intenta ir abajo, se mueve
      rectIzqYdir = -1;
    }
    break;
  // Cuando se presiona la tecla o
  case 'o':
    // Se pone que se intenta ir arriba rectangulo derecho
    rectDerArriba = true;
    if (rectDerAbajo)
    {
      // Si tambien se intenta ir abajo, no se mueve
      rectDerYdir = 0;
    }
    else
    {
      // Si solo intenta ir arriba, se mueve
      rectDerYdir = 1;
    }
    break;
  // Cuando se presiona la tecla l
  case 'l':
    // Se pone que se intenta ir abajo rectangulo derecho
    rectDerAbajo = true;
    if (rectDerArriba)
    {
      // Si tambien se intenta ir arriba, no se mueve
      rectDerYdir = 0;
    }
    else
    {
      // Si solo intenta ir abajo, se mueve
      rectDerYdir = -1;
    }
    break;
  }
}

// Funcion para detectar cuando una tecla es soltada
void keyUp(unsigned char key, int x, int y)
{
  switch (key)
  {
  // Cuando se suelta la tecla w
  case 'w':
    // Se pone que ya no se intenta ir arriba rectangulo izquierdo
    rectIzqArriba = false;
    if (rectIzqAbajo)
    {
      // Si se esta intentando ir abajo, se mueve
      rectIzqYdir = -1;
    }
    else
    {
      // Si no se esta intentando ir abajo, se deja de mover
      rectIzqYdir = 0;
    }
    break;
  // Cuando se suelta la tecla s
  case 's':
    // Se pone que ya no se intenta ir abajo rectangulo izquierdo
    rectIzqAbajo = false;
    if (rectIzqArriba)
    {
      // Si se esta intentando ir arriba, se mueve
      rectIzqYdir = 1;
    }
    else
    {
      // Si no se esta intentando ir arriba, se deja de mover
      rectIzqYdir = 0;
    }
    break;
  // Cuando se suelta la tecla o
  case 'o':
    // Se pone que ya no se intenta ir arriba rectangulo derecho
    rectDerArriba = false;
    if (rectDerAbajo)
    {
      // Si se esta intentando ir abajo, se mueve
      rectDerYdir = -1;
    }
    else
    {
      // Si no se esta intentando ir abajo, se deja de mover
      rectDerYdir = 0;
    }
    break;
  // Cuando se suelta la tecla l
  case 'l':
    // Se pone que ya no se intenta ir abajo rectangulo derecho
    rectDerAbajo = false;
    if (rectDerArriba)
    {
      // Si se esta intentando ir arriba, se mueve
      rectDerYdir = 1;
    }
    else
    {
      // Si no se esta intentando ir arriba, se deja de mover
      rectDerYdir = 0;
    }
    break;
  }
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
  // Registra la funcion para detectar teclas presionadas
  glutKeyboardFunc(keyDown);
  // Registra la funcion para detectar teclas soltas
  glutKeyboardUpFunc(keyUp);
  // Loop para correr
  glutMainLoop();

  return 0;
}