#include <bits/stdc++.h>
#include <GL/glut.h>
#include <ctime>
using namespace std;

#define Z0COLOR 255, 0, 0
#define Z1COLOR 0, 255, 0
#define Z2COLOR 0, 0, 255
#define Z3COLOR 255, 255, 0
#define Z4COLOR 255, 0, 255
#define Z5COLOR 0, 255, 255
#define Z6COLOR 105, 105, 105
#define Z7COLOR 255, 255, 255


#define Width 512
#define Height 512

int maxY = Height/2-1 ;
int minY = -Height/2 ;  
int maxX = Width/2-1 ;
int minX = -Width/2 ;

int lineRight = 100;
int lineTop = 100;
int lineLeft = -100;
int lineBottom = -100;


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

//draws the grid
void draw_grid(void){
    glBegin(GL_LINES);
    glVertex2i(lineLeft,minY);
    glVertex2i(lineLeft,maxY);
    glVertex2i(lineRight,minY);
    glVertex2i(lineRight,maxY);
    glVertex2i(minX,lineTop);
    glVertex2i(maxX,lineTop);
    glVertex2i(minX,lineBottom);
    glVertex2i(maxX,lineBottom);

    glEnd();
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

void draw8way(int x0, int y0, int x1, int y1)
{
    int zone = findZone(x0, y0, x1, y1);
    switch (zone)
    {
    case 0:
        drawLine0(x0, y0, x1, y1, 0);
        /* code */
        break;

    case 1:
        drawLine0(y0, x0, y1, x1, 1);
        /* code */
        break;

    case 2:
        drawLine0(y0, -x0, y1, -x1, 2);
        /* code */
        break;

    case 3:
        drawLine0(-x0, y0, -x1, y1, 3);
        /* code */
        break;

    case 4:
        drawLine0(-x0, -y0, -x1, -y1, 4);
        /* code */
        break;

    case 5:
        drawLine0(-y0, -x0, -y1, -x1, 5);
        /* code */
        break;

    case 6:
        drawLine0(-y0, x0, -y1, x1, 6);
        /* code */
        break;

    case 7:
        drawLine0(x0, -y0, x1, -y1, 7);
        /* code */
        break;
    default:
        break;
    }

}



double findTopT(int x0, int y0, int x1, int y1)
{
    return double(double(lineTop - y0) / double(y1 - y0));
}

double findBottomT(int x0, int y0, int x1, int y1)
{
    return double(double(y0 - lineBottom) / double(y0 - y1));
}


double findRightT(int x0, int y0, int x1, int y1)
{
    return double(double(lineRight - x0) / double(x1 -x0));
}

double findLeftT(int x0, int y0, int x1, int y1)
{
    return double(double(x0 - lineLeft) / double(x0 - x1));
}

bool isTopEntering(int x0, int y0, int x1, int y1)
{
    if ((y1 - y0) >= 0)
    {
        return false;
    }

    return true;
}

bool isBottomEntering(int x0, int y0, int x1, int y1)
{
    if ((y0 - y1) >= 0)
    {
        return false;
    }

    return true;
}

bool isRightEntering(int x0, int y0, int x1, int y1)
{
    if ((x1 - x0) >= 0)
    {
        return false;
    }

    return true;
}

bool isLeftEntering(int x0, int y0, int x1, int y1)
{
    if ((x0 - x1) >= 0)
    {
        return false;
    }

    return true;
}


struct point {
    double x;
    double y;
};


point findLineEndpoints(int x0, int y0, int x1, int y1, double t)
{
    point p;

    p.x = (x0 + t*(x1-x0));
    p.y = (y0 + t*(y1-y0));

    return p;

}

void findClippedLine(int x0, int y0, int x1, int y1)
{

    glPointSize(5);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_POINTS);
    glVertex2i(x0, y0);
    glVertex2i(x1, y1);
    glEnd();


    glPointSize(1);
    glColor3f(0, 0.0, 1.0);
    draw8way(x0, y0, x1, y1);

    vector<double> entering;
    vector<double> leaving;

    double tEMax = -10000;
    double tLMin = 10000;

    double topT = findTopT(x0, y0, x1, y1);
    double bottomT = findBottomT(x0, y0, x1, y1);
    double rightT = findRightT(x0, y0, x1, y1);
    double leftT = findLeftT(x0, y0, x1, y1);

    
    bool topEntering = isTopEntering(x0, y0, x1, y1);
    bool bottomEntering = isBottomEntering(x0, y0, x1, y1);
    bool rightEntering = isRightEntering(x0, y0, x1, y1);
    bool leftEntering = isLeftEntering(x0, y0, x1, y1);
    // cout<<topT<<endl;

    // cout<<bottomT<<endl;
    // cout<<rightT<<endl;
    // cout<<leftT<<endl;


    if(topEntering)
    {
        tEMax = max(tEMax, topT);
        
    }
    else{
        tLMin = min(tLMin, topT);
    }

    if(bottomEntering)
    {
        tEMax = max(tEMax, bottomT);
    }
    else{
        tLMin = min(tLMin, bottomT);
    }

    if(rightEntering)
    {
        tEMax = max(tEMax, rightT);
    }
    else{
        tLMin = min(tLMin, rightT);
    }

    if(leftEntering)
    {
        tEMax = max(tEMax, leftT);
    }
    else{
        tLMin = min(tLMin, leftT);
    }



    cout<<tEMax<<endl;
    cout<<tLMin<<endl;
    // cout<<entering[0]<<endl;

    // cout<<entering[entering.size()-1]<<endl;

    // // cout<<leaving[0]<<endl;

    // // cout<<leaving[leaving.size() -1]<<endl;

    // for(double e: entering)
    // {
    //     cout<<"AWOO"<<e<<endl;
    // }

    // for(double e: leaving)
    // {
    //     cout<<"AWOO"<<e<<endl;
    // }
    // // double tEMax = entering[0];
    // // double tLMin = leaving[0];

    if(tEMax > tLMin)
    {
        cout<<"Doesn't enter Zone"<<endl;
    }
    else
    {
        point startPoint = findLineEndpoints(x0, y0, x1, y1, tEMax);
        point endPoint = findLineEndpoints(x0, y0, x1, y1, tLMin);


        glPointSize(5);
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_POINTS);
        glVertex2i(startPoint.x, startPoint.y);
        glVertex2i(endPoint.x, endPoint.y);
        glEnd();

        glPointSize(1);
        glColor3f(1.0, 0.0, 0.0);

        draw8way(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }


}

void drawEnds(int x0, int y0, int x1, int y1)
{
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2i(x0 , y0);
    glVertex2i(x1, y1);
    glEnd();
}
    


void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    draw_grid();

    int x_input_0;
    int y_input_0;
    int x_input_1;
    int y_input_1;

    cin>>x_input_0;
    cin>>y_input_0;
    cin>>x_input_1;
    cin>>y_input_1;

    glBegin(GL_POINTS);
    drawEnds(x_input_0, y_input_0, -x_input_1, -y_input_1);
    findClippedLine(x_input_0, y_input_0, x_input_1, y_input_1);
    
    glFlush();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(10, 10);
    // glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // sets the display mode

    glutCreateWindow("Line Clipping");
    myInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    // glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
