#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

#define Width 720
#define Height 720

void drawLine(int, int);
void makeCube(double);
void project(void);
void rotate(void);


static void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Width/2, Width/2 - 1,  -Height/2, Height/2 - 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawAxis(void) 
{
    glColor3ub(100, 100, 100);

    glPushMatrix();
    glBegin(GL_LINES);
    glVertex2i(-Width/2, 0);
    glVertex2i(Width/2, 0);
    glVertex2i( 0, Height/2);
    glVertex2i( 0, -Height/2);

    glEnd();
    glPopMatrix();
}


static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxis();

    glBegin(GL_POINTS);

    makeCube(200);
    rotate();
    project();

    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

static void idle(void) {
    glutPostRedisplay();
}


void myInit() {
    glClearColor(0, 0, 0, 1);

    glEnable(GL_COLOR_MATERIAL);
}

std::pair<int, int> convertToZone0(int x, int y, int zone)
{
    switch (zone) {
        case 0:
            return std::make_pair(x, y);
        case 1: 
            return std::make_pair(y, x);
        case 2: 
            return std::make_pair(y, -x);
        case 3: 
            return std::make_pair(-x, y);
        case 4: 
            return std::make_pair(-x, -y);
        case 5: 
            return std::make_pair(-y, -x);
        case 6: 
            return std::make_pair(-y, x);
        case 7: 
            return std::make_pair(x, -y);
        default:
            assert(false);
    }
    return std::make_pair(x, y);
}


std::pair<int, int> convertToOriginal(int x, int y, int zone)
{
    switch (zone) {
        case 0:
            return std::make_pair(x, y);
        case 1:
            return std::make_pair(y, x);
        case 2:
            return std::make_pair(-y, x);
        case 3:
            return std::make_pair(-x, y);
        case 4:
            return std::make_pair(-x, -y);
        case 5:
            return std::make_pair(-y, -x);
        case 6:
            return std::make_pair(y, -x);
        case 7:
            return std::make_pair(x, -y);
        default:
            assert(false);
    }

    return std::make_pair(x, y);
}

void draw8Way(int x0, int y0, int x1, int y1, int zone) {
    // glColor3ub(255, 255, 255);
    std::pair<int, int> converted = convertToZone0(x0, y0, zone);
    x0 = converted.first;
    y0 = converted.second;

    converted = convertToZone0(x1, y1, zone);
    x1 = converted.first;
    y1 = converted.second;

    int dx = x1 - x0, dy = y1 - y0;
    int delE = 2*dy, delNE = 2*(dy - dx), d = 2*dy - dx;
    int y = y0;

    for(int x = x0; x <= x1; x++) {
        std::pair<int, int> original = convertToOriginal(x, y, zone);
        glVertex2i(original.first, original.second);
        if(d < 0) {
            d += delE;
        } else {
            d += delNE;
            y++;
        }
    }
}


void findZone(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    if(dx >= 0 && dy >= 0) { // zone 0, zone 1
        if(dx >= dy) {
            draw8Way(x0, y0, x1, y1, 0);
        } else {
            draw8Way(x0, y0, x1, y1, 1);
        }
    } else if(dx < 0 && dy >= 0) { // zone 2, zone 3
        if(abs(dx) >= dy) { // zone 3
            draw8Way(x0, y0, x1, y1, 3);
        } else { // zone 2
            draw8Way(x0, y0, x1, y1, 2);
        }
    } else if(dx < 0 && dy < 0) { // zone 4, zone 5
        if(abs(dx) >= abs(dy)) { // zone 4
            draw8Way(x0, y0, x1, y1, 4);
        } else { // zone 5
            draw8Way(x0, y0, x1, y1, 5);
        }
    } else { // zone 6, zone 7
        if(abs(dx) >= abs(dy)) { // zone 7
            draw8Way(x0, y0, x1, y1, 7);
        } else { // zone 6
            draw8Way(x0, y0, x1, y1, 6);
        }
    }
}


struct Point3D {
    double x, y, z;
    Point3D() {};
    Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

struct Matrix
{
	int row, col;
	vector<vector<double>> mat;
	Matrix() {}
	Matrix(int _row, int _col) {
		row = _row;
		col = _col;
		mat = vector<vector<double>>(row+1, vector<double>(col+1, 0));
	}
	Matrix operator*(const Matrix& m) {
		int r = m.row;
		int c = m.col;
		Matrix res(row, c);
		for(int i = 0; i < row; i++) {
			for(int j = 0; j < c; j++) {
				for(int k = 0; k < col; k++) {
					res.mat[i][j] += mat[i][k] * m.mat[k][j];
				}
			}
		}
		return res;
	}
};

struct segment {
    Point3D p1, p2;
    Point3D p;
    segment() {}
    segment(Point3D _p1, Point3D _p2) {
        p1 = _p1;
        p2 = _p2;
        p = Point3D(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
    }

    segment(double x, double y, double z) {
        p = Point3D(x, y, z);
    }

    segment cross(segment other) {
        double x = p.y * other.p.z - p.z * other.p.y;
        double y = other.p.x * p.z - p.x * other.p.z;
        double z = p.x * other.p.y - other.p.x * p.y;
        return segment(x, y, z);
    }

    double dot(segment other) {
        return p.x * other.p.x + p.y * other.p.y + p.z * other.p.z;
    }
};

Matrix projectionMatrix(4, 4);
Matrix x_rotation(4, 4);
Matrix y_rotation(4, 4);
Matrix z_rotation(4, 4);

Matrix coordinate(4, 1);
Matrix new_coordinate(4, 1);


Point3D P3D[8];
Point3D P3D2[8];
Point3D P2D[8];

double thetaX = 0;
double thetaY = 0;
double thetaZ = 0;

void makeCube(double A) {
    P3D[0] = Point3D(-A, -A, A);
    P3D[1] = Point3D(A, -A, A);
    P3D[2] = Point3D(A, A, A);
    P3D[3] = Point3D(-A, A, A);
    P3D[4] = Point3D(-A, -A, -A);
    P3D[5] = Point3D(A, -A, -A);
    P3D[6] = Point3D(A, A, -A);
    P3D[7] = Point3D(-A, A, -A);
}

double d = 200;
double l = 200;

const double PI = acos(-1.0);

double degToRad(double ang) {
    return ang * PI / 180;
}

void rotate() {
    x_rotation.mat = {{1,   0,                          0,                          0},
                      {0,   cos(degToRad(thetaX)),     -sin(degToRad(thetaX)),    0},
                      {0,   sin(degToRad(thetaX)),     cos(degToRad(thetaX)),     0},
                      {0,   0,                          0,                          1}};

    y_rotation.mat = {{cos(degToRad(thetaY)),    0,     sin(degToRad(thetaY)),    0},
                      {0,                         1,     0,                         0},
                      {-sin(degToRad(thetaY)),   0,     cos(degToRad(thetaY)),    0},
                      {0,                         0,     0,                         1}};

    z_rotation.mat = {{cos(degToRad(thetaZ)),     -sin(degToRad(thetaZ)),     0,      0},
                      {sin(degToRad(thetaZ)),      cos(degToRad(thetaZ)),     0,      0},
                      {0,                           0,                          1,      0},
                      {0,                           0 ,                         0,      1}};


    for(int i = 0; i < 8; i++) {
        coordinate.mat = {{P3D[i].x},
                          {P3D[i].y},
                          {P3D[i].z},
                          {1}};
        new_coordinate = x_rotation * y_rotation * z_rotation * coordinate;
        P3D2[i] = Point3D(new_coordinate.mat[0][0], new_coordinate.mat[1][0], new_coordinate.mat[2][0]);
    }
}


void project() {
    projectionMatrix.mat = {{1,     0,      0,      0}, 
                            {0,     1,      0,      0}, 
                            {0,     0,      1,      0}, 
                            {0,     0,   -1/d,      0}};

    Matrix temp(4, 1);
    Matrix res(4, 1);
    for(int i = 0; i < 8; i++) {
        temp.mat = {{P3D2[i].x}, 
                    {P3D2[i].y}, 
                    {-l - d - P3D2[i].z}, 
                    {1}};
        res = projectionMatrix * temp;
        P2D[i] = Point3D(
            P3D2[i].x,
            P3D2[i].y,
            0
        );
    }
    
    // plane 1
    if(segment(P3D2[0], P3D2[3]).cross(segment(P3D2[0], P3D2[1])).dot(segment(0, 0, 1)) > 0) {
        drawLine(0, 1);
        drawLine(1, 2);
        drawLine(2, 3);
        drawLine(3, 0);
    }

    // plane 2
    if(segment(P3D2[0], P3D2[1]).cross(segment(P3D2[0], P3D2[4])).dot(segment(0, 0, 1)) > 0) {
        drawLine(0, 1);
        drawLine(1, 5);
        drawLine(5, 4);
        drawLine(0, 4);
    }
    

    // plane 3
    if(segment(P3D2[1], P3D2[2]).cross(segment(P3D2[1], P3D2[5])).dot(segment(0, 0, 1)) > 0) {
        drawLine(1, 2);
        drawLine(5, 6);
        drawLine(1, 5);
        drawLine(2, 6);
    }

    // plane 4
    if(segment(P3D2[2], P3D2[3]).cross(segment(P3D2[2], P3D2[6])).dot(segment(0, 0, 1)) > 0) {
        drawLine(2, 3);
        drawLine(6, 7);
        drawLine(2, 6);
        drawLine(3, 7);
    }

    // plane 5
    if(segment(P3D2[0], P3D2[4]).cross(segment(P3D2[0], P3D2[3])).dot(segment(0, 0, 1)) > 0) {
        drawLine(3, 0);
        drawLine(7, 4);
        drawLine(3, 7);
        drawLine(4, 0);
    }
    
    // plane 6
    if(segment(P3D2[4], P3D2[5]).cross(segment(P3D2[4], P3D2[7])).dot(segment(0, 0, 1)) > 0) {
        drawLine(4, 5);
        drawLine(5, 6);
        drawLine(6, 7);
        drawLine(7, 4);
    }

    
}


void drawLine(int i, int j) {
    findZone(P2D[i].x, P2D[i].y, P2D[j].x, P2D[j].y);
}


void keyPressed(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
            thetaX++;
            break;
        case GLUT_KEY_DOWN:
            thetaX--;
            break;
        case GLUT_KEY_RIGHT:
            thetaY++;
            break;
        case GLUT_KEY_LEFT:
            thetaY--;
            break;
        case GLUT_KEY_PAGE_UP:
            thetaZ++;
            break;
        case GLUT_KEY_PAGE_DOWN:
            thetaZ--;
            break;
    }
}

void normalKeyPressed(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
            l += 10;
            break;
        case 's':
            l -= 10;
            break;
        case 'a':
            d -= 10;
            break;
        case 'd':
            d += 10;
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(Width, Height);

    glutInitWindowPosition(10, 50);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("Surface Visibility");
    myInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutIdleFunc(idle);

    glutSpecialFunc(keyPressed);
    glutKeyboardFunc(normalKeyPressed);

    glutMainLoop();
    return 0;
}