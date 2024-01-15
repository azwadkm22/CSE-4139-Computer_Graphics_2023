#include <bits/stdc++.h>
#include <GL/glut.h>
#include <stdlib.h>
using namespace std;


#define Width 640
#define Height 480
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

void draw4way(int x, int y, int shiftx, int shifty)
{

    drawPixel(x + shiftx, y + shifty, BLUE);
    drawPixel(x + shiftx, -y + shifty, BLUE);
    drawPixel(-x + shiftx, y + shifty, BLUE);
    drawPixel(-x + shiftx, -y + shifty, BLUE);

}



void drawEllipse(int x, int y, int a, int b)
{
    int shiftx = x;
    int shifty = y;
    x = 0;
    y = b;
    int d = 4 * (b*b - a*a*b) + a*a;
    int delE = 4*b*b * (2*x + 3);
    int delSE = 4*b*b * (2*x + 3) + 4*a*a * (2 - 2*y);
 

    draw4way(x, y, shiftx, shifty);

    while(2*b*b*(x + 1) < a*a*(2*y - 1)){
        if(d < 0){
            d += 4*b*b*(2*x + 3);
            x++;
        }
        else{
            d += 4*b*b * (2*x + 3) + 4*a*a * (2 - 2*y);
            x++;
            y--;
        }
        draw4way(x, y, shiftx, shifty);
    }

    while(y > 0){
        if(d < 0){
            d += 4*b*b * (2*x + 3) + 4*a*a * (2 - 2*y);
            x++;
            y--;
        }
        else{
            d += 4*a*a * (2 - 2*y);
            y--;
        }
        draw4way(x, y, shiftx, shifty);
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
float y = -50;
int r = 100;
int y_max = Height/2;
int y_min = -1*Height/2 + 3;
float dy = 1.5;
int a = r;
int b = r;

double cons = 2*r*r;

int bol = 0;
int change = 2;
float lim = r/2;
int turn = 0;
void bouncingEllipse()
{
    bol = bol;
    float g = 0.1;

    if(bol == 1)
    {   
        a += change;
        b = sqrt(2*r*r - a*a);
        
        y = y_min + b + 0.5;
        
        if(a == lim || b == lim)
        {
            change *= -1;
        }
        if(a - b <= 0.1)
        {
            bol = 0;
        }
        
    }
    else
    {

        dy = dy - g;
        y = y + dy;
        if(y + r > y_max)
        {
            bol = 1;
            dy = dy *-1;
            g = g *-1;
            y = y_min + b + 0.5;
            if(change < 0)
            {
                change *= -1;
            }
        }
        if( y - r < y_min)
        {
            bol = 1;
            dy = dy*-1;
            g = g *-1;
            y = y_min + b + 0.5;
            if(change < 0)
            {
                change *= -1;
            }
        }
    }
    drawEllipse(0, y, a, b);
}
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    char c;
    scanf("%c", &c);
    bouncingEllipse();
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

    glutCreateWindow("Bouncing Ellipse");
    myInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}