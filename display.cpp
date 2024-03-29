#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

#include "board.h"

Board myBoard;
int width = 800;
int height = 800;
double fov = 45;
double aspect = 1;
double zNear = 1;
double zFar = 1000;
double lookX = 0;
double lookY = 0;
double lookZ = 130;
double lookTheta = 0;

void init();
void initLight();
void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void special(int, int, int);
void timer(int);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    init();
    glutMainLoop();
    return 0;
}

void initLight()
{
    GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat matDiffuse[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat matSpecular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat matShininess[] = {50.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess); 
}

void init()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Snake");

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);
    initLight();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    myBoard.draw3D();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    aspect = (double)w/h;
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        myBoard.setDirection(UP);
        break;
    case GLUT_KEY_DOWN:
        myBoard.setDirection(DOWN);
        break;
    case GLUT_KEY_LEFT:
        myBoard.setDirection(LEFT);
        break;
    case GLUT_KEY_RIGHT:
        myBoard.setDirection(RIGHT);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case 32:
        myBoard.togglePause();
        break;
    case 'r':
        if (myBoard.isStopped())
            myBoard.restart();
        break;
    default:
        break;
    }
}

void timer(int value)
{
    if (value && !myBoard.isStopped() && !myBoard.isPaused()) {
        myBoard.move();
    }
    glutTimerFunc(myBoard.getPeriod(), timer, 1);
    glutPostRedisplay();
}
