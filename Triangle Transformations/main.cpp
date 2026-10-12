
#include <windows.h>
#include <GL/glut.h>
#include <cmath>

// Transformation variables
float tx = 0.0f, ty = 0.0f;
float angle = 0.0f;
float scaleFactor = 1.0f;

// Triangle center
const float cx = 250.0f;
const float cy = 183.33f;

// 1. User-defined function for Translation
void translatePoint(float &x, float &y, float dx, float dy)
{
    x = x + dx;
    y = y + dy;
}

// 2. User-defined function for Rotation
void rotatePoint(float &x, float &y, float degrees)
{
    float rad = degrees * 3.14159265f / 180.0f;

    float oldX = x;
    float oldY = y;

    x = oldX * cos(rad) - oldY * sin(rad);
    y = oldX * sin(rad) + oldY * cos(rad);
}

// 3. User-defined function for Scaling
void scalePoint(float &x, float &y, float sx, float sy)
{
    x = x * sx;
    y = y * sy;
}

// Apply all transformations to one vertex
void transformVertex(float &x, float &y)
{
    // Move the vertex relative to the triangle center
    x -= cx;
    y -= cy;

    // Apply scaling
    scalePoint(x, y, scaleFactor, scaleFactor);

    // Apply rotation
    rotatePoint(x, y, angle);

    // Return to the original center
    x += cx;
    y += cy;

    // Apply translation
    translatePoint(x, y, tx, ty);
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 500.0, 0.0, 500.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Original triangle vertices
    float x1 = 200.0f, y1 = 150.0f;
    float x2 = 300.0f, y2 = 150.0f;
    float x3 = 250.0f, y3 = 250.0f;

    // Apply user-defined transformations
    transformVertex(x1, y1);
    transformVertex(x2, y2);
    transformVertex(x3, y3);

    // Draw triangle
    glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(x1, y1);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(x2, y2);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(x3, y3);

    glEnd();

    glFlush();
}

// Keyboard controls
void special(int key, int, int)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            tx -= 5.0f;
            break;

        case GLUT_KEY_RIGHT:
            tx += 5.0f;
            break;

        case GLUT_KEY_UP:
            ty += 5.0f;
            break;

        case GLUT_KEY_DOWN:
            ty -= 5.0f;
            break;

        case GLUT_KEY_F1:
            angle += 5.0f;
            break;

        case GLUT_KEY_F2:
            angle -= 5.0f;
            break;

        default:
            return;
    }

    glutPostRedisplay();
}

// Normal keyboard controls for scaling and reset
void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case '+':
        case '=':
            scaleFactor *= 1.1f;
            break;

        case '-':
        case '_':
            scaleFactor /= 1.1f;
            break;

        case 'r':
        case 'R':
            tx = 0.0f;
            ty = 0.0f;
            angle = 0.0f;
            scaleFactor = 1.0f;
            break;

        case 27: // ESC
            exit(0);
            break;

        default:
            return;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(500, 500);
    glutCreateWindow("User Defined Transformations");

    init();

    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}
