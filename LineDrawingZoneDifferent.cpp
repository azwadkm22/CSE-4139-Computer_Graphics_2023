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
    glVertex2i(x, y);

    glEnd();
    glPopMatrix();

}


void drawLine0(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
    // printf("Zone 0");
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

void drawLine1(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
    // printf("Zone 1");
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = dy - 2*dx;
    int dn = -2*dx;
    int dne = 2*(dy - dx);

    while(y <= y1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d > 0)
        {
            y++;
            d += dn;
        }
        else
        {
            x++;
            y++;
            d += dne;
        }
    }
}

void drawLine2(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
    // printf("Zone 2");
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = -2*dx - dy;
    int dn = -2*dx;
    int dnw = -2*(dy + dx);

    while(y < y1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d < 0)
        {
            y++;
            d += dn;
        }
        else
        {
            x--;
            y++;
            d += dnw;
        }
    }
}

void drawLine3(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
        // printf("Zone 3");
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = -2*dx + dy;
    int dw = -2*dy;
    int dnw = -2*(dy + dx);

    while(x > x1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d > 0)
        {
            x--;
            d += dw;
        }
        else
        {
            x--;
            y++;
            d += dnw;
        }
    }
}

void drawLine4(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
        // printf("Zone 4");
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = -2*dy + dx;
    int dw = -2*dy;
    int dsw = -2*(dy - dx);

    while(x > x1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d < 0)
        {
            x--;
            d += dw;
        }
        else
        {
            x--;
            y--;
            d += dsw;
        }
    }
}

void drawLine5(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
        // printf("Zone 5");
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = 2*dx - dy;
    int ds = 2*dx;
    int dsw = -2*(dy - dx);

    while( y >= y1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d > 0)
        {
            y--;
            d += ds;
        }
        else
        {
            x--;
            y--;
            d += dsw;
        }
    }
}

void drawLine6(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
        // printf("Zone 6");
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = 2*dx + dy;
    int ds = 2*dx;
    int dse = 2*(dx + dy);

    while(y > y1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d < 0)
        {
            y--;
            d += ds;
        }
        else
        {
            x++;
            y--;
            d += dse;
        }
    }
}

void drawLine7(int x0, int y0, int x1, int y1, int zone, int r, int g, int b)
{
    // printf("Zone 7");
    int dx = x1-x0;
    int dy = y1-y0;

    int x = x0, y = y0;
    int d = 2*dx - dy;
    int de = 2*dy;
    int dse = 2*(dy + dx);

    while(x < x1)
    {
        drawPixel(x, y, zone, r, g, b);
        if(d > 0)
        {
            x++;
            d += de;
        }
        else
        {
            x++;
            y--;
            d += dse;
        }
    }
}



void findZoneAndDraw(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    if(abs(dx) >= abs(dy))
    {
        if(dx >= 0 && dy >= 0)
            drawLine0(x0, y0, x1, y1, 0, Z0COLOR);
        else if(dx>=0 && dy < 0)
            drawLine7(x0, y0, x1, y1, 7, Z7COLOR);
        else if(dx<0 && dy>=0)
            drawLine3(x0, y0, x1, y1, 3, Z3COLOR);
        else drawLine4(x0, y0, x1, y1, 4, Z4COLOR);
    }
    else {
        if(dx >= 0 && dy >= 0)
            drawLine1(x0, y0, x1, y1, 1, Z1COLOR);
        else if(dx>=0 && dy < 0)
            drawLine6(x0, y0, x1, y1, 6, Z6COLOR);
        else if(dx<0 && dy>=0)
            drawLine2(x0, y0, x1, y1, 2, Z2COLOR);
        else drawLine5(x0, y0, x1, y1, 5, Z5COLOR);
    }
}


void drawLine(void)
{
    int x0 =0, y0 = 0, x1 = 100, y1 = 0;
    // JOB
    int r = 200;
    for(int i=0; i<360; i++)
    {
        int x = r*cos(i);
        int y = r* sin(i);
        findZoneAndDraw(0, 0, x, y);
    }

}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxis();
    glEnd();
    glBegin(GL_POINTS);

    // Call Functions here

    drawLine();
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

    glutCreateWindow("LineDrawingZoneDifferent");
    myInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}