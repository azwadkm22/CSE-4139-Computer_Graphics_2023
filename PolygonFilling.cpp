#include <bits/stdc++.h>
#include <GL/glut.h>
#include <ctime>
using namespace std;

#define Width 1000
#define Height 1000


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


void drawPixel(int x, int y, int zone)
{
    glPushMatrix();
    glBegin(GL_POINTS);

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


void drawLine0(int x0, int y0, int x1, int y1, int zone)
{
    int dx = x1-x0;
    int dy = y1-y0; 

    int x = x0, y = y0;
    int d = 2*dy - dx;
    int de = 2*dy;
    int dne = 2*(dy - dx);

    while(x <= x1)
    {
        drawPixel(x, y, zone);
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

void drawLine(int x0, int y0, int x1, int y1)
{
    int zone = findZone(x0, y0, x1, y1);
    switch (zone)
    {
    case 0:
        drawLine0(x0, y0, x1, y1, 0);
        break;

    case 1:
        drawLine0(y0, x0, y1, x1, 1);
        break;

    case 2:
        drawLine0(y0, -x0, y1, -x1, 2);
        break;

    case 3:
        drawLine0(-x0, y0, -x1, y1, 3);
        break;

    case 4:
        drawLine0(-x0, -y0, -x1, -y1, 4);
        break;

    case 5:
        drawLine0(-y0, -x0, -y1, -x1, 5);
        break;

    case 6:
        drawLine0(-y0, x0, -y1, x1, 6);
        break;

    case 7:
        drawLine0(x0, -y0, x1, -y1, 7);
        break;

    default:
        break;
    }

}


int D = 400;
int L = 400;

struct point3D {
    double x;
    double y;
    double z;
};

struct point2D {
    double x;
    double y;
};

point3D P3D[8];
point2D P2D[8];

void makeCube(double sideA)
{
    // 1. Generate the P3D Array
    P3D[0].x = sideA; P3D[0].y = sideA; P3D[0].z = sideA - L;
    P3D[1].x = -sideA; P3D[1].y = sideA; P3D[1].z = sideA - L;
    P3D[2].x = sideA; P3D[2].y = -sideA; P3D[2].z = sideA - L;
    P3D[3].x = -sideA; P3D[3].y = -sideA; P3D[3].z = sideA - L;
    P3D[4].x = sideA; P3D[4].y = sideA; P3D[4].z = -sideA - L;
    P3D[5].x = -sideA; P3D[5].y = sideA; P3D[5].z = -sideA - L;
    P3D[6].x = sideA; P3D[6].y = -sideA; P3D[6].z = -sideA - L;
    P3D[7].x = -sideA; P3D[7].y = -sideA; P3D[7].z = -sideA - L;
    

    // 2. Find the P2D projection of the P3D points
    for(int i=0; i<8; i++)
    {
        double Zvalue = P3D[i].z;
        cout<<"Z: "<<Zvalue<<endl;
        double Wvalue = 1-Zvalue/D;
        if(Wvalue == 0)
        {
            Wvalue = 1;
        }
        double xDash = P3D[i].x / (Wvalue);
        double yDash = P3D[i].y / (Wvalue);
        P2D[i].x = xDash;
        P2D[i].y = yDash;
    }

}


void myInit (void){
    glClearColor(0.0, 0.0, 0.0,1.0);
}


static void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Width/2, Width/2 ,  -Height/2, Height/2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void keyboard(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT)    
    {
        L-= 10;
    }
        
    else if(key == GLUT_KEY_RIGHT)
    {
        
        L+=10;
    }
    else if(key == GLUT_KEY_DOWN)
    {
        D-=10;
    }
    else if(key == GLUT_KEY_UP)
    {
        D+=10;
    }

}

static void idle(void)
{
    glutPostRedisplay();
}

bool done = false;

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1, 1, 1);
    drawAxis();


    makeCube(200);
    done = true;
    // for(int i=0; i<7; i++)
    {
        // drawLine(P2D[0].x, P2D[0].y, P2D[2].x, P2D[2].y);
    }

    drawLine(P2D[0].x, P2D[0].y, P2D[1].x, P2D[1].y);
    drawLine(P2D[0].x, P2D[0].y, P2D[2].x, P2D[2].y);
    drawLine(P2D[1].x, P2D[1].y, P2D[3].x, P2D[3].y);
    drawLine(P2D[2].x, P2D[2].y, P2D[3].x, P2D[3].y);

    drawLine(P2D[0].x, P2D[0].y, P2D[4].x, P2D[4].y);
    drawLine(P2D[1].x, P2D[1].y, P2D[5].x, P2D[5].y);
    drawLine(P2D[2].x, P2D[2].y, P2D[6].x, P2D[6].y);
    drawLine(P2D[3].x, P2D[3].y, P2D[7].x, P2D[7].y);

    drawLine(P2D[4].x, P2D[4].y, P2D[5].x, P2D[5].y);
    drawLine(P2D[4].x, P2D[4].y, P2D[6].x, P2D[6].y);
    drawLine(P2D[5].x, P2D[5].y, P2D[7].x, P2D[7].y);
    drawLine(P2D[6].x, P2D[6].y, P2D[7].x, P2D[7].y);

    glutSwapBuffers();
}


int main(int argc, char** argv)
{

    
    glutInit(&argc, argv);

    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // sets the display mode

    glutCreateWindow("Polygon Filling");
    myInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}