
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

const float PI = 3.14159265f;
float t = 0.0f;

// ------------------------------------------------------------
//  Basic helper functions
// ------------------------------------------------------------
void setColor(float r, float g, float b, float a = 1.0f) {
    glColor4f(r, g, b, a);
}

void rect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
}

// bottom to top smooth color gradient
void gradRect(float x1, float y1, float x2, float y2,
              float br, float bg, float bb,     // bottom color
              float tr, float tg, float tb) {   // top color
    glBegin(GL_QUADS);
        glColor3f(br, bg, bb); glVertex2f(x1, y1); glVertex2f(x2, y1);
        glColor3f(tr, tg, tb); glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
}

void triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1); glVertex2f(x2, y2); glVertex2f(x3, y3);
    glEnd();
}

// ellipse: x = cx + rx*cos(a), y = cy + ry*sin(a)
void ellipse(float cx, float cy, float rx, float ry, int seg = 48) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= seg; i++) {
            float a = 2.0f * PI * i / seg;
            glVertex2f(cx + rx * cosf(a), cy + ry * sinf(a));
        }
    glEnd();
}

void circle(float cx, float cy, float r, int seg = 48) {
    ellipse(cx, cy, r, r, seg);
}

// half circle
void halfCircle(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 30; i++) {
            float a = PI * i / 30;
            glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
        }
    glEnd();
}

// heart shape (parametric equation)
void heart(float cx, float cy, float size) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 64; i++) {
            float a = 2.0f * PI * i / 64;
            float s = sinf(a);
            float x = size * s * s * s;
            float y = size * (13 * cosf(a) - 5 * cosf(2 * a)
                              - 2 * cosf(3 * a) - cosf(4 * a)) / 16.0f;
            glVertex2f(cx + x, cy + y);
        }
    glEnd();
}

// ------------------------------------------------------------
//  Scene parts
// ------------------------------------------------------------
void drawSky() {                     // sunset sky gradient
    gradRect(0, 255, 800, 400, 1.00f, 0.62f, 0.40f,   0.95f, 0.48f, 0.60f);
    gradRect(0, 400, 800, 600, 0.95f, 0.48f, 0.60f,   0.22f, 0.17f, 0.48f);
}

void drawStars() {                   // small stars
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 30; i++) {
        float x = (float)((i * 97 + 31) % 800);
        float y = 410.0f + (float)((i * 53) % 185);
        float a = 0.45f + 0.4f * sinf(t * 2.0f + i);
        glColor4f(1.0f, 1.0f, 0.9f, a);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawSun() {                     // sun and glow
    float pulse = 4.0f * sinf(t * 1.5f);
    setColor(1.0f, 0.90f, 0.50f, 0.12f); circle(620, 285, 115 + pulse);
    setColor(1.0f, 0.88f, 0.50f, 0.22f); circle(620, 285, 85 + pulse);
    setColor(1.0f, 0.86f, 0.45f);        circle(620, 285, 48);
}

void cloud(float x, float y, float s) {
    setColor(1.0f, 0.90f, 0.93f, 0.9f);
    circle(x, y, 24 * s);
    circle(x + 26 * s, y - 4 * s, 19 * s);
    circle(x - 26 * s, y - 6 * s, 17 * s);
    circle(x + 8 * s, y + 14 * s, 20 * s);
    ellipse(x, y - 10 * s, 50 * s, 12 * s);
}

void drawClouds() {                  // cloud
    cloud(fmodf(100 + t * 12, 950.0f) - 75, 480, 1.3f);
    cloud(fmodf(500 + t * 7, 950.0f) - 75, 430, 1.0f);
    cloud(fmodf(300 + t * 4, 950.0f) - 75, 500, 0.8f);
}

void drawGround() {                  // mount and grass
    setColor(0.52f, 0.45f, 0.68f);
    ellipse(560, 250, 260, 70);
    ellipse(100, 245, 200, 55);
    setColor(0.40f, 0.66f, 0.42f);
    ellipse(190, 235, 260, 45);
    ellipse(690, 235, 250, 40);
    gradRect(0, 0, 800, 245, 0.18f, 0.52f, 0.20f,   0.38f, 0.72f, 0.34f);
}

void drawTrees() {
    // left side grass
    setColor(0.40f, 0.25f, 0.12f); rect(112, 160, 128, 255);
    setColor(0.15f, 0.48f, 0.22f);
    circle(120, 290, 46); circle(88, 268, 34); circle(152, 268, 34); circle(120, 330, 32);
    setColor(0.25f, 0.62f, 0.30f);
    circle(108, 300, 16); circle(140, 282, 14); circle(122, 335, 12);

    // right side flower
    setColor(0.40f, 0.25f, 0.12f); rect(672, 160, 688, 255);
    setColor(1.0f, 0.62f, 0.75f);
    circle(680, 290, 46); circle(648, 268, 34); circle(712, 268, 34); circle(680, 330, 32);
    setColor(1.0f, 0.78f, 0.86f);
    circle(668, 300, 16); circle(700, 282, 14); circle(682, 335, 12);
}

void drawPath() {                    // road
    setColor(0.92f, 0.82f, 0.66f);
    glBegin(GL_QUADS);
        glVertex2f(370, 165); glVertex2f(430, 165);
        glVertex2f(500, 0);   glVertex2f(300, 0);
    glEnd();
    setColor(0.80f, 0.68f, 0.52f);   // stepping stone
    for (int i = 0; i < 5; i++)
        ellipse(400 + ((i % 2) ? 14 : -14), 22 + i * 28, 26 - i * 3, 7, 20);
}

void window(float cx, float cy) {
    float glow = 0.85f + 0.05f * sinf(t * 2.0f);          // luminous
    setColor(1, 1, 1);               rect(cx - 27, cy - 37, cx + 27, cy + 37);   // frame
    setColor(1.0f, glow, 0.45f);     rect(cx - 23, cy - 33, cx + 23, cy + 33);   // glass
    setColor(1.0f, 0.60f, 0.72f);                                                // curtain
    rect(cx - 23, cy - 33, cx - 13, cy + 33);
    rect(cx + 13, cy - 33, cx + 23, cy + 33);
    glLineWidth(3);
    setColor(1, 1, 1);
    glBegin(GL_LINES);               // window
        glVertex2f(cx, cy - 33);  glVertex2f(cx, cy + 33);
        glVertex2f(cx - 23, cy);  glVertex2f(cx + 23, cy);
    glEnd();
    setColor(0.45f, 0.28f, 0.15f);   // flower box
    rect(cx - 31, cy - 50, cx + 31, cy - 37);
    for (int i = 0; i < 4; i++) {
        float fx = cx - 21 + i * 14;
        setColor(0.15f, 0.55f, 0.20f); circle(fx, cy - 36, 5, 12);
        if (i % 2 == 0) setColor(1.0f, 0.35f, 0.50f);
        else            setColor(1.0f, 0.85f, 0.20f);
        circle(fx, cy - 31, 4, 12);
    }
}

void drawDoor() {
    setColor(1, 1, 1);                                   // white frame
    rect(366, 150, 434, 250); halfCircle(400, 250, 34);
    setColor(0.50f, 0.28f, 0.14f);                       // wooden door
    rect(370, 150, 430, 250); halfCircle(400, 250, 30);
    setColor(0.42f, 0.23f, 0.11f);                       // panels
    rect(376, 158, 396, 235); rect(404, 158, 424, 235);
    glLineWidth(2);
    setColor(0.30f, 0.15f, 0.07f);
    glBegin(GL_LINES); glVertex2f(400, 150); glVertex2f(400, 280); glEnd();
    setColor(1.0f, 0.82f, 0.20f);                        // door knob
    circle(392, 200, 3, 12); circle(408, 200, 3, 12);
    setColor(0.90f, 0.10f, 0.25f);                       // heart wreath
    heart(400, 248, 10);
}

void drawHouse() {
    // chimney
    setColor(0.60f, 0.25f, 0.20f); rect(455, 370, 485, 470);
    setColor(0.40f, 0.15f, 0.12f); rect(450, 462, 490, 474);

    // wall + stone foundation
    setColor(1.0f, 0.92f, 0.82f);  rect(280, 150, 520, 330);
    setColor(0.72f, 0.58f, 0.48f); rect(280, 150, 520, 170);

    // roof
    setColor(0.65f, 0.15f, 0.20f);
    triangle(250, 325, 550, 325, 400, 455);
    glLineWidth(1.5f);
    setColor(0.48f, 0.10f, 0.15f);                       // shingle lines
    glBegin(GL_LINES);
    for (float y = 340; y < 450; y += 15) {
        float half = 150.0f * (455.0f - y) / 130.0f;
        glVertex2f(400 - half, y); glVertex2f(400 + half, y);
    }
    glEnd();
    glLineWidth(3);
    setColor(0.35f, 0.07f, 0.10f);                       // roof outline
    glBegin(GL_LINE_LOOP);
        glVertex2f(250, 325); glVertex2f(550, 325); glVertex2f(400, 455);
    glEnd();
    setColor(0.45f, 0.10f, 0.15f); rect(250, 322, 550, 332);   // eave trim

    // round heart window on roof
    setColor(1, 1, 1);               circle(400, 380, 22);
    setColor(1.0f, 0.88f, 0.50f);    circle(400, 380, 18);
    setColor(0.90f, 0.10f, 0.25f);   heart(400, 380, 9);

    window(325, 250);
    window(475, 250);
    drawDoor();

    // porch lanterns
    setColor(1.0f, 0.90f, 0.50f, 0.30f); circle(360, 232, 16); circle(440, 232, 16);
    setColor(1.0f, 0.85f, 0.40f);        circle(360, 232, 5);  circle(440, 232, 5);

    // steps
    setColor(0.60f, 0.60f, 0.62f); rect(350, 142, 450, 150);
    setColor(0.70f, 0.70f, 0.72f); rect(340, 134, 460, 142);

    // bushes with pink blossoms
    setColor(0.15f, 0.45f, 0.20f);
    circle(265, 172, 24); circle(245, 165, 20); circle(285, 165, 18);
    circle(535, 172, 24); circle(555, 165, 20); circle(515, 165, 18);
    setColor(1.0f, 0.55f, 0.70f);
    for (int j = 0; j < 5; j++) {
        circle(245 + j * 9, 168 + (j % 2) * 8, 3.5f, 10);
        circle(515 + j * 9, 168 + (j % 2) * 8, 3.5f, 10);
    }
}

void drawSmoke() {
    for (int i = 0; i < 5; i++) {
        float p  = fmodf(t * 0.4f + i * 0.2f, 1.0f);      // 0..1 life
        float sx = 470 + sinf(p * 6 + i) * 12 + p * 30;
        float sy = 478 + p * 110;
        setColor(0.95f, 0.95f, 0.97f, 0.55f * (1 - p));
        circle(sx, sy, 7 + p * 16, 24);
    }
}

void drawFence() {
    setColor(0.80f, 0.75f, 0.70f);   // rails
    rect(0, 80, 322, 86);   rect(0, 100, 322, 106);
    rect(478, 80, 800, 86); rect(478, 100, 800, 106);

    setColor(1, 1, 1);               // pickets
    for (int k = 0; k < 15; k++) {
        float xl = 310 - k * 22;
        float xr = 478 + k * 22;
        rect(xl, 70, xl + 12, 115);   triangle(xl, 115, xl + 12, 115, xl + 6, 126);
        rect(xr, 70, xr + 12, 115);   triangle(xr, 115, xr + 12, 115, xr + 6, 126);
    }
    setColor(0.75f, 0.70f, 0.65f);   // gate posts
    rect(316, 65, 326, 130); rect(474, 65, 484, 130);
    setColor(0.90f, 0.10f, 0.25f);
    heart(321, 138, 7); heart(479, 138, 7);
}

void flower(float x, float y, float sway, int k) {
    float col[5][3] = { {1.0f, 0.40f, 0.60f}, {1.0f, 0.20f, 0.30f}, {1.0f, 1.0f, 1.0f},
                        {0.70f, 0.40f, 0.90f}, {1.0f, 0.60f, 0.20f} };
    glLineWidth(2);
    setColor(0.10f, 0.50f, 0.15f);
    glBegin(GL_LINES); glVertex2f(x, y - 14); glVertex2f(x + sway, y); glEnd();   // stem
    setColor(col[k][0], col[k][1], col[k][2]);
    for (int i = 0; i < 5; i++) {                                                  // petals
        float a = 2 * PI * i / 5;
        circle(x + sway + 5 * cosf(a), y + 5 * sinf(a), 4, 12);
    }
    setColor(1.0f, 0.90f, 0.20f);
    circle(x + sway, y, 3, 12);                                                    // center
}

void drawFlowers() {
    for (int i = 0; i < 44; i++) {
        float x = 8.0f + i * 18.0f;
        float y = 14.0f + (float)((i * 53) % 40);
        if (x > 295 && x < 505) continue;
        flower(x, y, 2.0f * sinf(t * 2.0f + i), i % 5);
    }
}

void drawPetals() {
    for (int i = 0; i < 8; i++) {
        float p  = fmodf(t * 0.15f + i / 8.0f, 1.0f);
        float px = 680 + (i - 4) * 22 + sinf(t * 1.5f + i) * 14;
        float py = 340 - p * 300;
        setColor(1.0f, 0.75f, 0.85f, 0.9f);
        ellipse(px, py, 4, 2.5f, 10);
    }
}

void drawHearts() {
    for (int i = 0; i < 6; i++) {
        float p  = fmodf(t * 0.12f + i / 6.0f, 1.0f);
        float hx = 400 + sinf(i * 2.3f + p * 5.0f) * (30 + p * 140);
        float hy = 430 + p * 95;
        float a  = (p * 4 > 1 ? 1.0f : p * 4) * (1 - p);  // fade in + fade out
        if (i % 2 == 0) setColor(0.95f, 0.15f, 0.30f, a);
        else            setColor(1.0f, 0.50f, 0.70f, a);
        heart(hx, hy, 8 + (i % 3) * 3);
    }
}

void drawTitle() {
    const char* title = "HONEYMOON HOUSE";
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;
    float w = 0;
    for (const char* c = title; *c; c++) w += glutBitmapWidth(font, *c);
    float x = 400 - w / 2;
    float y = 548;

    setColor(0.25f, 0.05f, 0.30f, 0.9f);                  // shadow
    glRasterPos2f(x + 2, y - 2);
    for (const char* c = title; *c; c++) glutBitmapCharacter(font, *c);

    setColor(1.0f, 0.95f, 0.70f);                         // gold text
    glRasterPos2f(x, y);
    for (const char* c = title; *c; c++) glutBitmapCharacter(font, *c);

    setColor(0.95f, 0.20f, 0.35f);
    heart(x - 30, y + 7, 11);
    heart(x + w + 30, y + 7, 11);
}

// ------------------------------------------------------------
//  GLUT callbacks
// ------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();


    drawSky();
    drawStars();
    drawSun();
    drawClouds();
    drawGround();
    drawTrees();
    drawPath();
    drawHouse();
    drawSmoke();
    drawFence();
    drawFlowers();
    drawPetals();
    drawHearts();
    //drawTitle();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
}

void timer(int v) {
    t += 0.016f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);

}
void init() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);              // transparency (glow, smoke, hearts)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);      // 2D coordinate system
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Honeymoon House");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
