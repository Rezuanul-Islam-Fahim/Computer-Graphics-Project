#include <GL/glut.h>
#include <cmath>

struct Color
{
    float r, g, b;
};

float cloud1Pos = 0.0f, cloud2Pos = 0.0f, cloud3Pos = 0.0f, cloud4Pos = 0.0f, cloudSpeed = 0.5f;
float carPos = 0.0f, carSpeed = 1.5f;
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

void drawRect(float x1, float y1, float x2, float y2, Color color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);

    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);

    glEnd();
}

void drawClouds() {
    Color color = isNight ? Color{0.39f, 0.39f, 0.47f} : Color{1.00f, 1.00f, 1.00f};

    glPushMatrix();
    glTranslatef(cloud1Pos, 0, 0);
    drawCircle(300, 950, 50, color);
    drawCircle(360, 970, 55, color);
    drawCircle(420, 950, 50, color);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cloud2Pos, 0, 0);
    drawCircle(900, 920, 45, color);
    drawCircle(960, 940, 50, color);
    drawCircle(1020, 920, 45, color);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cloud4Pos, 0, 0);
    drawCircle(400, 750, 45, color);
    drawCircle(460, 770, 60, color);
    drawCircle(520, 750, 45, color);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cloud3Pos, 0, 0);
    drawCircle(1160, 750, 45, color);
    drawCircle(1220, 770, 60, color);
    drawCircle(1280, 750, 45, color);
    glPopMatrix();
}

void drawSky() {
    Color skyDay = Color {0.53f, 0.81f, 0.92f};
    Color skyNight = Color {0.10f, 0.10f, 0.44f};

    drawRect(0, 0, 1920, 1080, isNight ? skyNight : skyDay);

    if (isNight)
    {
        // -------- Moon ---------
        drawCircle(1600, 900, 50, Color{0.88f, 0.92f, 0.95f});
        drawCircle(1575, 885, 4, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1598, 865, 6, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1595, 925, 4, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1628, 905, 4, Color{0.58f, 0.66f, 0.76f});
    }
    else
    {
        // -------- Sun ---------
        drawCircle(1600, 900, 60, Color{1.00f, 0.86f, 0.39f});
    }

    drawClouds();
}

void drawMountain(float px1, float px2, float py, float pm, float h)
{
    glColor3f(0.24f, 0.47f, 0.24f);
    glBegin(GL_TRIANGLES);
    glVertex2f(px1, py);
    glVertex2f(pm, h);
    glVertex2f(px2, py);
    glEnd();
}

void drawLand() {
    drawMountain(0, 400, 520, 200, 630);
    drawMountain(300, 700, 520, 500, 700);
    drawMountain(1200,1600, 520, 1400, 640);
    drawMountain(1500,1940, 520, 1700, 750);
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

void drawRoad() {
    Color roadDay = Color {0.20f, 0.20f, 0.20f};
    Color roadNight = Color {0.12f, 0.12f, 0.12f};
    drawRect(0, 300, 1920, 450, isNight ? roadNight : roadDay);

    glColor3f(1.00f, 1.00f, 1.00f);
    glLineWidth(4);
    glBegin(GL_LINES);
    for (float x = 0; x < 1920; x += 80) {
        glVertex2f(x, 375);
        glVertex2f(x+40, 375);
    }
    glEnd();
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

void drawCar(float x) {
    glPushMatrix();
    glTranslatef(x, 0, 0);
    glColor3f(1.00f, 0.00f, 0.00f);
    glRectf(50, 330, 120, 380);
    glColor3f(0.59f, 0.59f, 0.59f);
    glRectf(60, 340, 80, 370);
    glRectf(90, 340, 110, 370);
    drawCircle(70, 330, 15, {0.16f, 0.16f, 0.16f});
    drawCircle(100, 330, 15, {0.16f, 0.16f, 0.16f});
    glPopMatrix();
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
    /*drawRiver();
    drawRoad();

    drawHouse(100, 350);
    drawHouse(200, 350);
    drawHouse(1400, 350);
    drawHouse(1550, 350);
    drawTower();

    drawTree(50, 300);
    drawTree(1800, 300);
    drawTree(1650, 300);

    drawCar(carPos);
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
