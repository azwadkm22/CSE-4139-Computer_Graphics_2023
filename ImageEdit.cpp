#include <bits/stdc++.h>
#include <GL/glut.h>
#include <ctime>
using namespace std;
#define Pi acos(-1)

#define WIDTH 512
#define HEIGHT 512

#define BI_RGB 0

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

// Convert H to 0 to 243
// Convert S and V to 255
int pictureHeight = 0;
int pictureWidth = 0;

unsigned char* imageData;

void LoadTexture(const char* filename)
{
    GLuint texture;
    FILE* file;
    // unsigned char* imageData;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    int width, height, size, offset;

    file = fopen(filename, "rb");
    if (!file) {
        cerr << "Error: could not open file " << filename << endl;
        return;
    }

    fread(&fileHeader, sizeof(fileHeader), 1, file);
    fread(&infoHeader, sizeof(infoHeader), 1, file);

    // Check if the color data is stored in reverse byte order (BGR)
    bool reverseBytes = false;
    if (infoHeader.biBitCount == 24 && infoHeader.biCompression == BI_RGB) {
        reverseBytes = true;
    }

    width = infoHeader.biWidth;
    pictureWidth = width;
    height = infoHeader.biHeight;
    pictureHeight = height;
    size = width * height * 3;

    // Allocate memory for the pixel data buffer
    imageData = new unsigned char[size];

    // Seek to the beginning of the pixel data
    offset = fileHeader.bfOffBits;
    fseek(file, offset, SEEK_SET);

    // Read the pixel data into the buffer
    fread(imageData, size, 1, file);
    
    // If the color data is stored in reverse byte order, swap the order of the color bytes
    if (reverseBytes) {
        for (int i = 0; i < size; i += 3) {
            unsigned char temp = imageData[i];
            imageData[i] = imageData[i+2];
            imageData[i+2] = temp;
        }
    }

    fclose(file);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

}


void myInit (void){
    glClearColor(0.0, 0.0, 0.0,1.0);
}

static void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, pictureWidth, 0, pictureHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void idle(void)
{
    glutPostRedisplay();
}

bool done = false;



void displayOriginal()
{
    glPointSize(1);
    glBegin(GL_POINTS);
    int colorIndex = 0;
    for(int x = 0; x < pictureWidth; x++){
        for(int y = 0; y < pictureHeight; y++){
            // printf("%d %d\n", x, y);
            colorIndex = (y * pictureWidth + x)*3;
            // printf("%d %d %d\n", imageData[colorIndex], imageData[colorIndex + 1], imageData[colorIndex + 2]);        
            glColor3ub(imageData[colorIndex], imageData[colorIndex + 1], imageData[colorIndex + 2]);
            // y * WIDTH + x
            glVertex2i(x, y);
        }
    }
    glEnd();
    glutSwapBuffers();
}


void displayNegative()
{
    glPointSize(1);
    glBegin(GL_POINTS);
    int colorIndex = 0;
    for(int x = 0; x < pictureWidth; x++){
        for(int y = 0; y < pictureHeight; y++){
            // printf("%d %d\n", x, y);
            colorIndex = (y * pictureWidth + x)*3;
            // printf("%d %d %d\n", imageData[colorIndex], imageData[colorIndex + 1], imageData[colorIndex + 2]);        
            glColor3ub(255 - imageData[colorIndex] , 255 - imageData[colorIndex + 1], 255 - imageData[colorIndex + 2]);
            // y * WIDTH + x
            glVertex2i(x, y);
        }
    }
    glEnd();
    glutSwapBuffers();
}

void displayHue()
{
    glPointSize(1);
    glBegin(GL_POINTS);
    int colorIndex = 0;
    for(int x = 0; x < pictureWidth; x++){
        for(int y = 0; y < pictureHeight; y++){
            // printf("%d %d\n", x, y);
            colorIndex = (y * pictureWidth + x)*3;
            // printf("%d %d %d\n", imageData[colorIndex], imageData[colorIndex + 1], imageData[colorIndex + 2]);        
            double R = imageData[colorIndex];
            double G = imageData[colorIndex+1];
            double B = imageData[colorIndex+2];
            

            double maxx = max(R, max(G, B));
            double minn = min(R, min(G, B));
            double diff = maxx - minn;
            double H = 0;

            if(maxx == R)
            {
                H = double(double(G-B) / diff) * 60;
            }


            else if(maxx == G)
            {
                H = double(double(B-R) / diff)*60 + 120;
            }


            else if(maxx == B)
            {
                H = double(double(R-G) / diff)*60 + 240;
            }

            if(H<0)
            {
                H += 360;
            }
            H /= 360;
            H *= 240;
            glColor3ub(H, H, H);
            // y * WIDTH + x
            glVertex2i(x, y);
        }
    }
    glEnd();
    glutSwapBuffers();
}


void displaySaturation()
{
    glPointSize(1);
    glBegin(GL_POINTS);
    int colorIndex = 0;
    for(int x = 0; x < pictureWidth; x++){
        for(int y = 0; y < pictureHeight; y++){
            // printf("%d %d\n", x, y);
            colorIndex = (y * pictureWidth + x)*3;
            // printf("%d %d %d\n", imageData[colorIndex], imageData[colorIndex + 1], imageData[colorIndex + 2]);        
            double R = imageData[colorIndex];
            double G = imageData[colorIndex+1];
            double B = imageData[colorIndex+2];
            

            double maxx = max(R, max(G, B));
            double minn = min(R, min(G, B));
            double diff = maxx - minn;
            double S = diff/maxx;
            S *= 240;
            glColor3ub(S, S, S);
            // y * WIDTH + x
            glVertex2i(x, y);
        }
    }
    glEnd();
    glutSwapBuffers();
}

void displayV()
{
    glPointSize(1);
    glBegin(GL_POINTS);
    int colorIndex = 0;
    for(int x = 0; x < pictureWidth; x++){
        for(int y = 0; y < pictureHeight; y++){
            // printf("%d %d\n", x, y);
            colorIndex = (y * pictureWidth + x)*3;
            // printf("%d %d %d\n", imageData[colorIndex], imageData[colorIndex + 1], imageData[colorIndex + 2]);        
            double R = imageData[colorIndex];
            double G = imageData[colorIndex+1];
            double B = imageData[colorIndex+2];
            

            double maxx = max(R, max(G, B));
            glColor3ub(maxx, maxx, maxx);
            // y * WIDTH + x
            glVertex2i(x, y);
        }
    }
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    LoadTexture("Input.bmp");
    printf("%d %d\n", pictureWidth, pictureHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Original");
    glutReshapeFunc(reshape);
    glutDisplayFunc(displayOriginal);
    glutIdleFunc(idle);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10 + (WIDTH*2), 10 + HEIGHT/2);
    glutCreateWindow("Negative");
    glutReshapeFunc(reshape);
    glutDisplayFunc(displayNegative);
    glutIdleFunc(idle);


    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10 + (WIDTH*2), 10);
    glutCreateWindow("Hue");
    glutReshapeFunc(reshape);
    glutDisplayFunc(displayHue);
    glutIdleFunc(idle);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10 + HEIGHT + 60);
    glutCreateWindow("Saturation");
    glutReshapeFunc(reshape);
    glutDisplayFunc(displaySaturation);
    glutIdleFunc(idle);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10 + WIDTH, 10 + HEIGHT + 60);
    glutCreateWindow("Value");
    glutReshapeFunc(reshape);
    glutDisplayFunc(displayV);
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}

