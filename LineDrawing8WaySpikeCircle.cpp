#include <bits/stdc++.h>
#include <GL/glut.h>
#include <stdlib.h>

#define Width 512
#define Height 512


#define WHITE 255, 255, 255

#define Z0COLOR 255, 0, 0
#define Z1COLOR 0, 255, 0
#define Z2COLOR 0, 0, 255
#define Z3COLOR 255, 255, 0
#define Z4COLOR 255, 0, 255
#define Z5COLOR 0, 255, 255
#define Z6COLOR 105, 105, 105
#define Z7COLOR 255, 255, 255



static void reshape(int width, int height) {
    // const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    // project = array, jetate display = oitao array
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    // glOrtho(-Width/2, Width/2 ,  -Height/2, Height/2, -1, 1);
    glOrtho(-Width/2, Width/2 ,  -Height/2, Height/2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


static void drawAxis(void)
{
    glColor3ub(WHITE);
    glPushMatrix();
    glBegin(GL_LINES);

    glVertex2i(-Width/2, 0);
    glVertex2i(Width/2, 0);
    glVertex2i(0, Height/2);
    glVertex2i(0, -Height/2);
}


void drawPixel(int x, int y, int zone, int r, int g, int b)
{
    glPushMatrix();
    glBegin(GL_POINTS);

    glColor3ub(r, g, b);
    // glVertex2i(x, y);

    switch(zone)
    {
        case 0:
            glVertex2i(x, y);
            break;
        case 1:
            glVertex2i(y, x);
            break;
        case 2:
            glVertex2i(-y, x);
            break;
        case 3:
            glVertex2i(-x, y);
            break;
        case 4:
            glVertex2i(-x, -y);
            break;
        case 5:
            glVertex2i(-y, -x);
            break;
        case 6:
            glVertex2i(y, -x);
            break;
        case 7:
            glVertex2i(x, -y);
            break;

    }

    glEnd();
    glPopMatrix();

}



int findZone(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    if(abs(dx) >= abs(dy))
    {
        if(dx >= 0 && dy >= 0)
            return 0;
        else if(dx>=0 && dy < 0)
            return 7;
        else if(dx<0 && dy>=0)
            return 3;
        else return 4;
    }
    else {
        if(dx >= 0 && dy >= 0)
            return 1;
        else if(dx>=0 && dy < 0)
            return 6;
        else if(dx<0 && dy>=0)
            return 2;
        else return 5;
    }
}


void drawLine0(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = 2*dy - dx;
    int de = 2*dy;
    int dne = 2*(dy - dx);

    while(x <= x1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d < 0)
        {
            x++;
            d += de;
        }
        else
        {
            x++;
            y++;
            d += dne;
        }
    }
}


void draw8way(int x0, int y0, int x1, int y1)
{
    int zone = findZone(x0, y0, x1, y1);
    switch (zone)
    {
    case 0:
        drawLine0(x0, y0, x1, y1, 0, Z0COLOR);
        /* code */
        break;

    case 1:
        drawLine0(y0, x0, y1, x1, 1, Z1COLOR);
        /* code */
        break;

    case 2:
        drawLine0(y0, -x0, y1, -x1, 2, Z2COLOR);
        /* code */
        break;

    case 3:
        drawLine0(-x0, y0, -x1, y1, 3, Z3COLOR);
        /* code */
        break;

    case 4:
        drawLine0(-x0, -y0, -x1, -y1, 4, Z4COLOR);
        /* code */
        break;

    case 5:
        drawLine0(-y0, -x0, -y1, -x1, 5, Z5COLOR);
        /* code */
        break;

    case 6:
        drawLine0(-y0, x0, -y1, x1, 6, Z6COLOR);
        /* code */
        break;

    case 7:
        drawLine0(x0, -y0, x1, -y1, 7, Z7COLOR);
        /* code */
        break;
    default:
        break;
    }

}

void drawSpikeCircle()
{
    int x0 =0, y0 = 0, x1 = 100, y1 = 0;
    // JOB
    int r = 200;
    for(int i=0; i<360; i++)
    {
        int x = r*cos(i);
        int y = r* sin(i);
        draw8way(0, 0, x, y);
    }

}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxis();
    glEnd();
    glBegin(GL_POINTS);

    // Call Functions here
    drawSpikeCircle();
    
    glEnd();
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

    glutCreateWindow("Draw8WaySpikeCircle");
    myInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
