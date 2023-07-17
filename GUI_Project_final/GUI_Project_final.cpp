#include <GL/glut.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>


GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Load the image file
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open texture file: " << filename << std::endl;
        return 0;
    }

    // Get the file size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file data into a buffer
    std::vector<unsigned char> imageData(fileSize);
    file.read((char*)&imageData[0], fileSize);

    // Close the file
    file.close();

    // Load the texture
    int width, height;
    width = 512;
    height = 512;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &imageData[0]);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

using namespace std;
// Rotation angle for cube and sphere
float angle = 0.0f;
GLuint textureID;

// Pyramid scale fluctuation parameters
float pyramidScale = 1.5f;
float pyramidScaleDelta = 0.03f;
bool pyramidScaleIncreasing = true;

// Cube vertical translation parameters
float cubeTranslateY = 0.0f;
float cubeTranslateDelta = 0.03f;
bool cubeTranslateUp = true;

// Cone horizontal translation parameters
float coneTranslateX = 0.0f;
float coneTranslateDelta = 0.03f;
bool coneTranslateRight = true;

// Function to draw a cube
void drawCube() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glutSolidCube(0.7);
    glDisable(GL_TEXTURE_2D);
}

// Function to draw a pyramid
void drawPyramid() {
    glBegin(GL_TRIANGLES);
    // Front face
    glVertex3f(0.0f, 0.375f, 0.0f);
    glVertex3f(-0.375f, -0.375f, 0.375f);
    glVertex3f(0.375f, -0.375f, 0.375f);
    // Right face
    glVertex3f(0.0f, 0.375f, 0.0f);
    glVertex3f(0.375f, -0.375f, 0.375f);
    glVertex3f(0.375f, -0.375f, -0.375f);
    // Back face
    glVertex3f(0.0f, 0.375f, 0.0f);
    glVertex3f(0.375f, -0.375f, -0.375f);
    glVertex3f(-0.375f, -0.375f, -0.375f);
    // Left face
    glVertex3f(0.0f, 0.375f, 0.0f);
    glVertex3f(-0.375f, -0.375f, -0.375f);
    glVertex3f(-0.375f, -0.375f, 0.375f);
    // Bottom face
    glVertex3f(-0.375f, -0.375f, 0.375f);
    glVertex3f(0.375f, -0.375f, 0.375f);
    glVertex3f(0.375f, -0.375f, -0.375f);
    glVertex3f(0.375f, -0.375f, -0.375f);
    glVertex3f(-0.375f, -0.375f, -0.375f);
    glVertex3f(-0.375f, -0.375f, 0.375f);
    glEnd();
}

// Function to draw a sphere
void drawSphere() {
    glutSolidSphere(0.35, 16, 16);
}

void drawTetrahedron()
{
    glutSolidTetrahedron();
}

// Function to draw a cone
void drawCone() {
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.35, 0.7, 16, 16);
}

// Function to draw a torus
void drawTorus() {
    glutSolidTorus(0.2, 0.5, 16, 16);
}

// Function to draw an icosahedron
void drawIcosahedron() {
    glutSolidIcosahedron();
}

// Function to draw a teapot
void drawTeapot() {
    glutSolidTeapot(0.5);
}

// Function to draw an octahedron
void drawOctahedron() {
    glutSolidOctahedron();
}

// Function to draw a dodecahedron
void drawDodecahedron() {
    glutSolidDodecahedron();
}

void setupLighting() {
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightPosition[] = { 0.0, 0.0, 1.0, 0.0 };
    GLfloat lightDirection[] = { 0.0, 0.0, -1.0, 0.0 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT1, GL_POSITION, lightDirection);
}

void setupMaterial() {
    GLfloat matAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat matDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat matShininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}



    

// Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 2.0, 1.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setupLighting();
    setupMaterial();
    textureID = loadTexture("C:\\Users\\Bogdan\\Pictures\\texture.bmp");

    // Draw the first row
    glPushMatrix();
    glTranslatef(-2.0f, 1.5f, -5.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glScalef(0.6f, 0.6f, 0.6f);
    drawTetrahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, 1.5f, -5.0f);
    glScalef(pyramidScale, pyramidScale, pyramidScale);
    drawPyramid();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.5f + cubeTranslateY, -5.0f);
    glScalef(0.6f, 0.6f, 0.6f);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 1.5f, -5.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    drawSphere();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0f + coneTranslateX, 1.5f, -5.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    drawCone();
    glPopMatrix();

    // Draw the second row
    glPushMatrix();
    glTranslatef(-2.0f, -1.5f, -5.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    drawTorus();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, -1.5f, -5.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    glRotatef(angle, 5.0f, 3.0f, 6.0f);
    drawIcosahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -5.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    glRotatef(angle, 2.0f, 0.0f, 0.0f);
    drawTeapot();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, -1.5f, -5.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    glRotatef(angle, -2.0f, 0.0f, 0.0f);
    drawOctahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0f, -1.5f, -5.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    drawDodecahedron();
    glPopMatrix();

   

    glutSwapBuffers();
}


// Function to update the scene
void timer(int value) {
    // Update rotation angle
    angle += 1.0f;

    // Update pyramid scale
    if (pyramidScaleIncreasing) {
        pyramidScale += pyramidScaleDelta;
        if (pyramidScale > 1.8f)
            pyramidScaleIncreasing = false;
    }
    else {
        pyramidScale -= pyramidScaleDelta;
        if (pyramidScale < 1.2f)
            pyramidScaleIncreasing = true;
    }

    // Update cube vertical translation
    if (cubeTranslateUp) {
        cubeTranslateY += cubeTranslateDelta;
        if (cubeTranslateY > 0.3f)
            cubeTranslateUp = false;
    }
    else {
        cubeTranslateY -= cubeTranslateDelta;
        if (cubeTranslateY < -0.3f)
            cubeTranslateUp = true;
    }

    // Update cone horizontal translation
    if (coneTranslateRight) {
        coneTranslateX += coneTranslateDelta;
        if (coneTranslateX > 0.5f)
            coneTranslateRight = false;
    }
    else {
        coneTranslateX -= coneTranslateDelta;
        if (coneTranslateX < -0.5f)
            coneTranslateRight = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 400);
    glutCreateWindow("3D Shapes");
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}