#include <windows.h>
#include <GL/glut.h>

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.96, 0.75);
    glPushMatrix();
    glTranslatef(0.7, 0.7, 0.0);
    glutSolidSphere(0.13, 20, 2);
    glPopMatrix();

    glColor3f(1.0, 0.82, 0.80);
    glBegin(GL_QUADS);
        glVertex3f(-0.6, -0.5, 0.0);
        glVertex3f(0.6, -0.5, 0.0);
        glVertex3f(0.6, 0.1, 0.0);
        glVertex3f(-0.6, 0.1, 0.0);
    glEnd();

    glColor3f(0.70, 0.10, 0.22);
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.72, 0.1, 0.0);
        glVertex3f(0.72, 0.1, 0.0);
        glVertex3f(0.0, 0.55, 0.0);
    glEnd();

    glColor3f(0.42, 0.24, 0.08);
    glBegin(GL_QUADS);
        glVertex3f(-0.10, -0.5, 0.0);
        glVertex3f(0.10, -0.5, 0.0);
        glVertex3f(0.10, -0.15, 0.0);
        glVertex3f(-0.10, -0.15, 0.0);
    glEnd();

    glColor3f(0.0, 0.50, 0.50);
    glBegin(GL_QUADS);

        glVertex3f(-0.46, -0.25, 0.0);
        glVertex3f(-0.20, -0.25, 0.0);
        glVertex3f(-0.20, 0.00, 0.0);
        glVertex3f(-0.46, 0.00, 0.0);

        glVertex3f(0.20, -0.25, 0.0);
        glVertex3f(0.46, -0.25, 0.0);
        glVertex3f(0.46, 0.00, 0.0);
        glVertex3f(0.20, 0.00, 0.0);
    glEnd();

    glFlush();
}

void init(void)
{
    glClearColor(0.02, 0.02, 0.18, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simple House");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
