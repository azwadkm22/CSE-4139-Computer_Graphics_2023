#include <bits/stdc++.h>
#include <GL/glut.h>
#include <stdlib.h>


#define Width 640
#define Height 480

#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255

static void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Width/2, Width/2 ,  -Height/2, Height/2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawPixel(int x, int y, int r, int g, int b) {
    glColor3ub(r, g, b);
    glPushMatrix();
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glPopMatrix();
}


void draw8way(int x, int y, int r, int g, int b, int shiftx, int shifty) {
    drawPixel(x + shiftx, y + shifty, r, g, b);
    drawPixel(x + shiftx, -y + shifty, r, g, b);
    drawPixel(-x + shiftx, y + shifty, r, g, b);
    drawPixel(-x + shiftx, -y + shifty, r, g, b);

    drawPixel(y + shiftx, x + shifty, r, g, b);
    drawPixel(-y + shiftx, x + shifty, r, g, b);
    drawPixel(y + shiftx, -x + shifty, r, g, b);
    drawPixel(-y + shiftx, -x + shifty, r, g, b);
}

void drawCircle(int r, int x, int y)
{
    int shiftx = x, shifty = y;
    x = 0; 
    y = r;
    int d = 5 - 4 * r;
    draw8way(x, y, GREEN, shiftx, shifty);
    while(x <= y) {
        if(d < 0) {
            d += 4 * (2 * x + 3);
            x++;
        } else {
            d += 4 * (2 * x - 2 * y + 5);
            x++;
            y--;
        }
        draw8way(x, y, GREEN, shiftx, shifty);
    }
}

static void drawAxis(void)
{
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glBegin(GL_LINES);

    glVertex2i(-Width/2, 0);
    glVertex2i(Width/2, 0);
    glVertex2i(0, Height/2);
    glVertex2i(0, -Height/2);

    glEnd();
    glPopMatrix();

}

float x =0; 
float y =0;
int r = 60;
int x_max = Width/2;
int x_min = -1*Width/2;
int y_max = Height/2;
int y_min = -1*Height/2;

float dx = 5;
float dy = 1.5;

void drawMovingCircle()
{
    x = x + dx;
    y = y + dy;

    if(x +r > x_max)
    {
        dx = dx*-1;
    }
    if(x - r <x_min)
    {
        dx = dx*-1;
    }
    if(y + r > y_max)
    {
        dy = dy *-1;
    }
    if( y - r < y_min)
    {
        dy = dy*-1;
    }
    drawCircle(r, x, y);
}
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxis();
    drawMovingCircle();
    glutSwapBuffers();
}

static void idle(void) {
    glutPostRedisplay();
}

void myInit() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Moving Circle");
    myInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}