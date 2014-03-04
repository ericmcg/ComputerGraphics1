//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//
// "Drop!" v. 1.4
//
// by Christopher Rasmussen
//
// CISC 440/640, September, 2008
// updated February, 2012
//
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "Sprite.hh"

#include <math.h>
#include <string.h>
#include <sys/time.h>

#include <GL/glut.h>

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// numTextures = 4

void initialize_random();
double uniform_random(double, double);
int uniform_random_int(int, int);
void draw_string(float, float, float, char *);

bool checkCollision(float, float);

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#define MAX2(A, B)          (((A) > (B)) ? (A) : (B))

#define STATE_NORMAL    1
#define STATE_EXPLODING 2
#define STATE_GONE      3

// these values depend on the order in load_sprites()

#define TEXTURE_TIEFIGHTER  1
#define TEXTURE_SAUCER      2
#define TEXTURE_SHOT        3
#define TEXTURE_EXPLOSION   4


#define SPEED_STEP_INCREASE 1
#define SPEED_STEP_DECREASE SPEED_STEP_INCREASE
#define TARGET_FPS 60
#define PI 3.14159

#define MAX_SPEED 3


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

class Ship 
{
public:

  int state;                             // alive or exploding (regenerates after death)
  float x, y;                            // position
  int theta;                           //angle of the sh
  int speed;				 //current speed

  float w, h;                            // dimensions

  bool turn_left, turn_right, boost;        // state of user key input 

  Ship();
  void update();
  void draw();
  void draw_shot();

};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

int win_w = 400;
int win_h = 500;

//Asteroids **asteroids;    // array of asteroids
Ship *ship;           // user-controllable ship

char *score_string;   // level, lives, score go here
int level = 1;
int num_destroyed = 0;

bool textureFlag = true;

int numObjects = 8;   
int numGunners = 5;

int msDelay = 2;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

Ship::Ship()
{
  state = STATE_NORMAL; 
  x = win_w/2; 
  y = win_h/2; 
  w = 20; 
  h = 30; 
  theta = 0;
  speed = 0;

  turn_left = turn_right = boost= false;
}

//----------------------------------------------------------------------------

void Ship::update()
{
  // user input

  if (turn_left)
    theta = (theta + (90/TARGET_FPS)) % 360 ; //rotate ship
  if (turn_right) 
    theta = (theta - (90/TARGET_FPS)) % 360 ; //rotate ship


  
  
  if (boost)
  {
    printf("The speed is: %d" , speed);
    speed += SPEED_STEP_INCREASE;

    if (speed > MAX_SPEED)
       speed = MAX_SPEED;
  }

  else
  {
    speed -= SPEED_STEP_DECREASE;
    if (speed < 0){
      speed = 0;
    }
  }

  x -= sin(theta*PI/180) * speed;
  y += cos(theta*PI/180) * speed;

  //fly to the other side of the screen if we overshoot
  if (x > win_w)
    x -= win_w;

  else if (x < 0)
    x += win_w;

  //fly to the other side of the screen if we overshoot
  if (y > win_h)
    y-= win_h;

  else if (y < 0)
    y += win_h;



  // ship collision
  //check for collisions with asteroids
}

//----------------------------------------------------------------------------

// do shots


//----------------------------------------------------------------------------

// non-texture stuff--only divided up because of 
// glEnable/glDisable(GL_TEXTURE_2D) division in display

void Ship::draw()
{


    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(theta,0,0,1);


 
    glBegin(GL_POLYGON);       
    glColor3f(1,1,1);
    glVertex2f(0, h);
    glVertex2f(w/2, 0);
    glVertex2f(-w/2, 0);


    glEnd();
    glPopMatrix();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


// MONO characters are all same width: 104.76 pixels

void draw_string(float x, float y, float char_width, char *text)
{
  char *p;
   
  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef(char_width / 104.76, char_width / 104.76, 1);
  for (p = text; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
  // proportional: looks better, but characters are different widths
  //    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// set seed based on time so no two games are alike

void initialize_random()
{
  struct timeval tp;

  gettimeofday(&tp, NULL);
  srand48(tp.tv_sec);
}

//----------------------------------------------------------------------------

// random float in range [lower, upper]

double uniform_random(double lower, double upper)
{
  double result;
  double range_size;

  range_size = upper - lower;
  result = range_size * drand48();
  result += lower;

  return result;
}

//----------------------------------------------------------------------------

// random int in range [lower, upper]

int uniform_random_int(int lower, int upper)
{
  return (int) round(uniform_random(lower, upper));
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

// has a key been released?

void keyboardup(unsigned char key, int x, int y)
{
  if (key == 'a') 
    ship->turn_left = false;
  else if (key == 'd') 
    ship->turn_right = false;
  else if (key == 'w')
    ship->boost = false;
}

//----------------------------------------------------------------------------

// has a key been pressed?

void keyboard(unsigned char key, int x, int y)
{
  if (key == 'q')
    exit(1);

  if (key == 'a') 
    ship->turn_left = true;
  
  if (key == 'd')
    ship->turn_right = true;

  if (key == 'w')
    ship->boost = true;
}

//----------------------------------------------------------------------------

// no other events

//void idle(int value)
void idle()
{
  // ship
  ship->update();

  glutPostRedisplay();

  //  glutTimerFunc(msDelay, idle, 0);
}

//----------------------------------------------------------------------------

void display() 
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  

  // falling objects

 // for (i = 0; i < numObjects; i++) 
 //   dropper[i]->draw();

  // shot

  //ship->draw_shot();

  // ship
  
  ship->draw();
				
  // sample text
  
  sprintf(score_string, "Level %i   Ships lost %i", 0, 11);
  draw_string(5, 5, 12, score_string);

  // finish up

  glutSwapBuffers();
}

//----------------------------------------------------------------------------

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0, win_w, 0, win_h);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

//----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(win_w, win_h);
  glutCreateWindow("Asteroids");
  init(); 			            // set OpenGL states, variables

  // game objects

  initialize_random();
	
  //int numTextures = 4;
  //char **texfilename = (char **) malloc(numTextures * sizeof(char *));
  //for (int i = 0; i < numTextures; i++) 
  //  texfilename[i] = (char *) malloc(256 * sizeof(char));

  //sprintf(texfilename[0], "tiefighter.ppm");
  //sprintf(texfilename[1], "saucer.ppm");
  //sprintf(texfilename[2], "fireball.ppm");
  //sprintf(texfilename[3], "explosion.ppm");

  //load_sprites(numTextures, texfilename);

  ship = new Ship;

  //dropper = (Dropper **) malloc(numObjects * sizeof(Dropper *));
  /*
  for (int i = 0; i < numObjects; i++) 
    dropper[i] = new Dropper(uniform_random(0, win_w), uniform_random(win_h/2, win_h), 
			     uniform_random(-180, 180), i < numGunners, 
			     uniform_random_int(TEXTURE_TIEFIGHTER, TEXTURE_SAUCER));

  */
  score_string = (char *) malloc(512 * sizeof(char));   

  // register callback routines

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardup);
  glutIdleFunc(idle);
  //  glutTimerFunc(msDelay, idle, 0);

  glutMainLoop(); 		    // enter event-driven loop

  return 1;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
