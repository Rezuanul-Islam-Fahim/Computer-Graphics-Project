#include <GL/glut.h>
#include <cmath>
#include <string>

using namespace std;

struct Color
{
    float r, g, b;
};

float rocketY = 0.0f;
float waterFlow = 0.0f;
bool isNight = false;

Color waterDay = Color {0.27f, 0.51f, 0.71f};
Color waterNight = Color {0.10f, 0.27f, 0.47f};

void drawCircle(float cx, float cy, float r, Color color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i <= 360; i += 10) {
        float rad = i * 3.14159f / 180.0f;
        glVertex2f(cx + r * cos(rad), cy + r * sin(rad));
    }

    glEnd();
}

void drawCirclePortion(float cx, float cy, float radius, float startAngle, float sweepAngle, Color color) {
    glColor3f(color.r, color.g, color.b);

    // GL_TRIANGLE_FAN is ideal for pie shapes / partial circles
    glBegin(GL_TRIANGLE_FAN);

    // 1. Establish the center origin point
    glVertex2f(cx, cy);

    // Convert angles from degrees to radians for C++ math
    float startRad = startAngle * M_PI / 180.0f;
    float sweepRad = sweepAngle * M_PI / 180.0f;

    int segments = 40; // Total smoothness of a full circle

    // Adjust segment count dynamically based on how large the slice actually is
    int localSegments = (int)(segments * (std::abs(sweepAngle) / 360.0f));
    if (localSegments < 2) localSegments = 2; // Safety minimum

    // 2. Loop to sweep out the arc edge
    for (int i = 0; i <= localSegments; i++) {
        float angle = startRad + (i * sweepRad) / localSegments;
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
        glVertex2f(x, y);
    }

    glEnd();
}

void drawRect(float x1, float y1, float x2, float y2, Color color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);

    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);

    glEnd();
}

void drawRoundedRect(float x1, float y1, float x2, float y2,
                             float rTopRight, float rTopLeft, float rBottomLeft, float rBottomRight,
                             Color color) {
    // 1. Determine actual min/max coordinates
    float minX = (x1 < x2) ? x1 : x2;
    float maxX = (x1 > x2) ? x1 : x2;
    float minY = (y1 < y2) ? y1 : y2;
    float maxY = (y1 > y2) ? y1 : y2;

    float width = maxX - minX;
    float height = maxY - minY;

    // 2. Clamp each radius to ensure they don't exceed half the width or height
    float maxR = (width < height) ? width / 2.0f : height / 2.0f;
    if (rTopRight > maxR)    rTopRight = maxR;
    if (rTopLeft > maxR)     rTopLeft = maxR;
    if (rBottomLeft > maxR)  rBottomLeft = maxR;
    if (rBottomRight > maxR) rBottomRight = maxR;

    // 3. Set color
    glColor3f(color.r, color.g, color.b);

    // 4. Draw the polygon sequentially in a counter-clockwise loop
    glBegin(GL_POLYGON);

    int segments = 15;
    float angle;

    // Corner 1: Top-Right (0 to 90 degrees)
    for (int i = 0; i <= segments; i++) {
        angle = (i * M_PI / 2.0f) / segments;
        glVertex2f((maxX - rTopRight) + cos(angle) * rTopRight,
                   (maxY - rTopRight) + sin(angle) * rTopRight);
    }

    // Corner 2: Top-Left (90 to 180 degrees)
    for (int i = 0; i <= segments; i++) {
        angle = (M_PI / 2.0f) + (i * M_PI / 2.0f) / segments;
        glVertex2f((minX + rTopLeft) + cos(angle) * rTopLeft,
                   (maxY - rTopLeft) + sin(angle) * rTopLeft);
    }

    // Corner 3: Bottom-Left (180 to 270 degrees)
    for (int i = 0; i <= segments; i++) {
        angle = M_PI + (i * M_PI / 2.0f) / segments;
        glVertex2f((minX + rBottomLeft) + cos(angle) * rBottomLeft,
                   (minY + rBottomLeft) + sin(angle) * rBottomLeft);
    }

    // Corner 4: Bottom-Right (270 to 360 degrees)
    for (int i = 0; i <= segments; i++) {
        angle = (3.0f * M_PI / 2.0f) + (i * M_PI / 2.0f) / segments;
        glVertex2f((maxX - rBottomRight) + cos(angle) * rBottomRight,
                   (minY + rBottomRight) + sin(angle) * rBottomRight);
    }

    glEnd();
}

void drawCloud(float px, float py)
{
    Color color = isNight ? Color{0.39f, 0.39f, 0.47f} : Color{1.00f, 1.00f, 1.00f};

    drawCircle(px, py, 50, color);
    drawCircle(px + 60, py + 20, 55, color);
    drawCircle(px + 120, py, 50, color);
}

void drawSky() {
    Color skyDay = Color {0.53f, 0.81f, 0.92f};
    Color skyNight = Color {0.10f, 0.10f, 0.44f};

    drawRect(0, 0, 1920, 1080, isNight ? skyNight : skyDay);

    // --------- Sun/Moon ---------
    glPushMatrix();
    if (isNight)
    {
        glTranslatef(1600, 900, 0.0);
        glScalef(0.8, 0.8, 1.0);
        glTranslatef(-1600, -900, 0.0);
    }
    drawCircle(1600, 900, 60, isNight ? Color{0.88f, 0.92f, 0.95f} : Color{1.00f, 0.86f, 0.39f});
    glPopMatrix();

    if (isNight)
    {
        drawCircle(1575, 885, 4, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1598, 865, 6, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1595, 925, 4, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1628, 905, 4, Color{0.58f, 0.66f, 0.76f});
    }

    // --------- Clouds ---------
    drawCloud(300, 950);
    drawCloud(900, 920);
    drawCloud(400, 750);
    drawCloud(1160, 750);
}

void drawMountain(float px1, float px2, float py, float pm, float h)
{
    glColor3f(0.24f, 0.47f, 0.24f);
    glBegin(GL_TRIANGLES);
    glVertex2f(px1, py);
    glVertex2f(pm, h);
    glVertex2f(px2, py);
    glEnd();

    // ----- Snow ------
    glPushMatrix();
    glTranslatef(pm, h, 0.0);
    glScalef(0.2, 0.2, 1.0);
    glTranslatef(-pm, -h, 0.0);
    glColor3f(1.00f, 1.00f, 1.00f);
    glBegin(GL_TRIANGLES);
    glVertex2f(px1, py);
    glVertex2f(pm, h);
    glVertex2f(px2, py);
    glEnd();
    glPopMatrix();
}

void drawLand() {
    drawMountain(0, 400, 520, 200, 630);
    drawMountain(300, 700, 520, 500, 700);
    drawMountain(1200,1600, 520, 1400, 640);
    drawMountain(1500,1940, 520, 1700, 750);
}

void drawRoad() {
    Color roadDay = Color {0.20f, 0.20f, 0.20f};
    Color roadNight = Color {0.12f, 0.12f, 0.12f};
    drawRect(0, 370, 1920, 520, isNight ? roadNight : roadDay);

    glColor3f(1.00f, 1.00f, 1.00f);
    glLineWidth(4);
    glBegin(GL_LINES);
    for (int i = 20; i < 1920; i += 90)
    {
        glVertex2f(i, 445);
        glVertex2f(i + 60, 445);
    }
    glEnd();
}

void drawCar(float x, string lane) {
    float ypos = lane == "lane1" ? 400.0 : 480.0;
    glPushMatrix();
    glTranslatef(x, ypos, 0);

    if (lane == "lane2")
    {
        glScalef(-1.0, 1.0, 1.0);
    }

    // ------- Roof -------
    drawCirclePortion(70, 30, 55, 0, 180, Color{1.00f, 0.00f, 0.00f});
    drawCirclePortion(75, 35, 40, 0, 90, Color{0.64f, 0.91f, 0.96f});
    drawCirclePortion(68, 35, 40, 90, 90, Color{0.64f, 0.91f, 0.96f});

    // ------ Body -------
    drawRoundedRect(0, 0, 155, 45, 50, 20, 6, 6, Color{1.00f, 0.00f, 0.00f});

    // ------ Tires ------
    drawCircle(35, -3, 20, Color{0.05f, 0.05f, 0.05f});
    drawCircle(115, -3, 20, Color{0.05f, 0.05f, 0.05f});

    // ------- Rims --------
    glPushMatrix();
    glTranslatef(35, -3, 0.0);
    glScalef(0.4, 0.4, 1.0);
    glTranslatef(-35, 3, 0.0);
    drawCircle(35, -3, 20, Color{0.84f, 0.80f, 0.82f});
    glPopMatrix();

    glPushMatrix();
    glTranslatef(115, -3, 0.0);
    glScalef(0.4, 0.4, 1.0);
    glTranslatef(-115, 3, 0.0);
    drawCircle(115, -3, 20, Color{0.84f, 0.80f, 0.82f});
    glPopMatrix();

    // ------ Door handles --------
    glLineWidth(3.5);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(35, 35);
    glVertex2f(43, 35);
    glVertex2f(80, 35);
    glVertex2f(88, 35);
    glEnd();

    // ------ Headlight -------
    drawRoundedRect(138, 18, 150, 26, 5, 5, 5, 5, Color{1.00f, 0.88f, 0.40f});


    glPopMatrix();
}

void drawRiver() {
    Color col = isNight ? waterNight : waterDay;
    drawRect(0, 0, 1920, 300, col);

    glColor3f(0.78f, 0.86f, 0.94f);
    for (float x = waterFlow; x < 2000; x += 80) {
        glBegin(GL_LINES);
        glVertex2f(x, 150); glVertex2f(x+40, 170);
        glVertex2f(x+20, 200); glVertex2f(x+60, 220);
        glEnd();
    }
}

void drawTree(float x, float y) {
    glColor3f(0.43f, 0.17f, 0.00f);
    glRectf(x-10, y, x+10, y+50);
    glColor3f(0.21f, 0.55f, 0.04f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x-25, y+50);
    glVertex2f(x, y+100);
    glVertex2f(x+25, y+50);
    glVertex2f(x-20, y+80);
    glVertex2f(x, y+130);
    glVertex2f(x+20, y+80);
    glEnd();
}

void drawHouse(float x, float y) {
    Color wall = isNight ? Color{0.31f, 0.31f, 0.39f} : Color{0.82f, 0.71f, 0.55f};
    Color roof = isNight ? Color{0.24f, 0.24f, 0.31f} : Color{0.63f, 0.31f, 0.16f};
    drawRect(x, y, x+80, y+80, wall);
    glColor3f(roof.r, roof.g, roof.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(x-10, y+80); glVertex2f(x+40, y+130); glVertex2f(x+90, y+80);
    glEnd();

    glColor3f(0.39f, 0.59f, 0.78f);
    glRectf(x+15, y+55, x+35, y+75);
    glRectf(x+45, y+55, x+65, y+75);

    glColor3f(0.31f, 0.24f, 0.16f);
    glRectf(x+32, y, x+48, y+40);
}

void drawTower() {
    Color main = isNight ? Color{0.20f, 0.20f, 0.27f} : Color{0.86f, 0.86f, 0.94f};
    drawRect(700, 400, 800, 900, main);
    glColor3f(0.39f, 0.59f, 0.78f);
    glRectf(720, 500, 780, 850);
    glColor3f(0.59f, 0.39f, 0.20f);
    glBegin(GL_TRIANGLES);
    glVertex2f(680, 900); glVertex2f(750, 980); glVertex2f(820, 900);
    glEnd();
}

void drawRocket() {
    glPushMatrix();
    glTranslatef(0, rocketY, 0);
    glColor3f(0.78f, 0.78f, 0.82f);
    glRectf(50, 200, 100, 400);
    glBegin(GL_TRIANGLES);
    glVertex2f(35, 400); glVertex2f(75, 450); glVertex2f(115, 400);
    glEnd();
    glColor3f(1.00f, 0.39f, 0.00f);
    glBegin(GL_TRIANGLES);
    glVertex2f(55, 200); glVertex2f(75, 160); glVertex2f(95, 200);
    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawLand();
    drawRoad();

    // -------- Cars --------
    drawCar(200, "lane1");
    drawCar(1000, "lane2");

    /*drawRiver();

    drawHouse(100, 350);
    drawHouse(200, 350);
    drawHouse(1400, 350);
    drawHouse(1550, 350);
    drawTower();

    drawTree(50, 300);
    drawTree(1800, 300);
    drawTree(1650, 300);
    drawRocket();*/

    glutSwapBuffers();
}

/*void update(int value) {
    cloud1Pos += cloudSpeed;
    if (cloud1Pos > 1920) cloud1Pos = -500;

    cloud2Pos += cloudSpeed * 0.8f;
    if (cloud2Pos > 1920) cloud2Pos = -500;

    carPos += carSpeed;
    if (carPos > 1920) carPos = -200;

    rocketY += 1.0f;
    if (rocketY > 600) rocketY = -200;

    waterFlow += 2.0f;
    if (waterFlow > 80) waterFlow = 0;

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}*/

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'd': case 'D': isNight = false; break;
        case 'n': case 'N': isNight = true; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("River City Space Port");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(0, 1920, 0, 1080);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    // glutTimerFunc(30, update, 0);
    glutMainLoop();

    return 0;
}
