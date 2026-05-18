#include <GL/glut.h>
#include <cmath>
#include <string>

using namespace std;

struct Color
{
    float r, g, b;
};

float rocketY = 320.0f;
float waterFlow1 = 300.0f, waterFlow2 = 1200.0f;
bool isNight = false;
bool rocketLaunched = false;

float speedFactor = 1.0f;
float cloudOffset = 0.0f;
float windmillAngle = 0.0f;

float car1_lane1_x = 200.0f;
float car2_lane1_x = 1100.0f;
float bus1_lane1_x = 1400.0f;
float car1_lane2_x = 1000.0f;
float car2_lane2_x = 200.0f;
float bus1_lane2_x = 600.0f;

float boat1_ch1_x = 200.0f;
float boat2_ch1_x = 1200.0f;
float boat1_ch2_x = 1000.0f;
float boat2_ch2_x = 500.0f;

float airplaneX = -200.0f;

void drawCircle(float cx, float cy, float r, Color color) {
    glColor3f(color.r, color.g, color.b);
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluDisk(quad, 0.0, r, 40, 1);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void drawCirclePortion(float cx, float cy, float radius, float startAngle, float sweepAngle, Color color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    float startRad = startAngle * M_PI / 180.0f;
    float sweepRad = sweepAngle * M_PI / 180.0f;
    int segments = 40;
    int localSegments = (int)(segments * (abs(sweepAngle) / 360.0f));
    if (localSegments < 2) localSegments = 2;
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
                     float rTopRight, float rTopLeft, float rBottomLeft, float rBottomRight, Color color) {
    float width = x2 - x1;
    float height = y2 - y1;

    float maxR = (width < height) ? width / 2.0f : height / 2.0f;
    if (rTopRight > maxR)    rTopRight = maxR;
    if (rTopLeft > maxR)     rTopLeft = maxR;
    if (rBottomLeft > maxR)  rBottomLeft = maxR;
    if (rBottomRight > maxR) rBottomRight = maxR;

    glColor3f(color.r, color.g, color.b);
    glBegin(GL_POLYGON);

    int segments = 15;
    float angle;

    for (int i = 0; i <= segments; i++) {
        angle = (i * M_PI / 2.0f) / segments;
        glVertex2f((x2 - rTopRight) + cos(angle) * rTopRight,
                   (y2 - rTopRight) + sin(angle) * rTopRight);
    }

    for (int i = 0; i <= segments; i++) {
        angle = (M_PI / 2.0f) + (i * M_PI / 2.0f) / segments;
        glVertex2f((x1 + rTopLeft) + cos(angle) * rTopLeft,
                   (y2 - rTopLeft) + sin(angle) * rTopLeft);
    }

    for (int i = 0; i <= segments; i++) {
        angle = M_PI + (i * M_PI / 2.0f) / segments;
        glVertex2f((x1 + rBottomLeft) + cos(angle) * rBottomLeft,
                   (y1 + rBottomLeft) + sin(angle) * rBottomLeft);
    }

    for (int i = 0; i <= segments; i++) {
        angle = (3.0f * M_PI / 2.0f) + (i * M_PI / 2.0f) / segments;
        glVertex2f((x2 - rBottomRight) + cos(angle) * rBottomRight,
                   (y1 + rBottomRight) + sin(angle) * rBottomRight);
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

void drawAirplane(float x) {
    float y = 860.0f;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.8f, 0.8f, 1.0f);

    Color bodyColor   = isNight ? Color{0.40f, 0.40f, 0.40f} : Color{0.95f, 0.95f, 0.95f};
    Color accentColor = isNight ? Color{0.15f, 0.25f, 0.55f} : Color{0.10f, 0.30f, 0.70f};
    Color wingColor   = isNight ? Color{0.35f, 0.35f, 0.35f} : Color{0.75f, 0.75f, 0.75f};
    Color windowColor = isNight ? Color{0.90f, 0.90f, 0.60f} : Color{0.20f, 0.35f, 0.50f};

    // ------ Main body ------
    glColor3f(bodyColor.r, bodyColor.g, bodyColor.b);
    glBegin(GL_POLYGON);
    glVertex2f(0, 15);
    glVertex2f(30, -5);
    glVertex2f(270, -5);
    glVertex2f(310, 5);
    glVertex2f(320, 18);
    glVertex2f(300, 32);
    glVertex2f(260, 35);
    glVertex2f(50, 35);
    glVertex2f(0, 25);
    glEnd();

    // ------- Stripe -------
    glColor3f(accentColor.r, accentColor.g, accentColor.b);
    glBegin(GL_POLYGON);
    glVertex2f(20, 8);
    glVertex2f(305, 8);
    glVertex2f(315, 12);
    glVertex2f(23, 12);
    glEnd();

    // ------ Tail -------
    glColor3f(bodyColor.r, bodyColor.g, bodyColor.b);
    glBegin(GL_POLYGON);
    glVertex2f(20, 30);
    glVertex2f(-5, 95);
    glVertex2f(20, 95);
    glVertex2f(55, 30);
    glEnd();

    // ----- Wing ------
    glColor3f(wingColor.r, wingColor.g, wingColor.b);
    glBegin(GL_POLYGON);
    glVertex2f(120, 5);
    glVertex2f(60, -70);
    glVertex2f(85, -70);
    glVertex2f(170, 5);
    glEnd();

    // ---------- Pilot cabin window -------
    glColor3f(windowColor.r, windowColor.g, windowColor.b);
    glBegin(GL_POLYGON);
    glVertex2f(298, 30);
    glVertex2f(312, 22);
    glVertex2f(305, 18);
    glVertex2f(290, 26);
    glEnd();

    for (int i = 0; i < 18; i++) {
        drawCircle(70 + (i * 12), 20, 3.0f, windowColor);
    }

    if (isNight) {
        drawCircle(140, 37, 3, Color{1.0f, 0.0f, 0.0f});
        drawCircle(140, -7, 3, Color{1.0f, 0.0f, 0.0f});
    }

    glPopMatrix();
}

void drawSky() {
    Color skyDay = Color {0.53f, 0.81f, 0.92f};
    Color skyNight = Color {0.10f, 0.10f, 0.44f};
    drawRect(0, 0, 1920, 1080, isNight ? skyNight : skyDay);

    // Sun/Moon
    glPushMatrix();
    if (isNight) {
        glTranslatef(1600, 900, 0.0);
        glScalef(0.8, 0.8, 1.0);
        glTranslatef(-1600, -900, 0.0);
    }
    drawCircle(1600, 900, 60, isNight ? Color{0.88f, 0.92f, 0.95f} : Color{1.00f, 0.86f, 0.39f});
    glPopMatrix();

    if (isNight) {
        drawCircle(1575, 885, 4, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1598, 865, 6, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1595, 925, 4, Color{0.58f, 0.66f, 0.76f});
        drawCircle(1628, 905, 4, Color{0.58f, 0.66f, 0.76f});

        // Stars
        float stars[][2] = {
            {100, 1000}, {250, 950}, {400, 1020}, {550, 980}, {700, 1030},
            {850, 970}, {1000, 1010}, {1150, 990}, {1300, 1020}, {1450, 960},
            {1600, 1000}, {1750, 980}, {1900, 1010}, {150, 850}, {350, 800},
            {500, 870}, {650, 830}, {800, 880}, {950, 850}, {1100, 820},
            {1250, 860}, {1400, 840}, {1550, 890}, {1700, 810}, {1850, 870},
            {50, 900}, {200, 780}, {450, 750}, {600, 770}, {750, 740},
            {900, 760}, {1050, 730}, {1200, 750}, {1350, 720}, {1500, 740},
            {1650, 710}, {1800, 730}, {1950, 700}
        };
        int numStars = sizeof(stars) / sizeof(stars[0]);
        for (int i = 0; i < numStars; i++) {
            drawCircle(stars[i][0], stars[i][1], 2, Color{1.0f, 1.0f, 1.0f});
        }
    }

    float wrap = 1920.0f;
    drawCloud(fmod(300 + cloudOffset, wrap), 950);
    drawCloud(fmod(900 + cloudOffset, wrap), 920);
    drawCloud(fmod(400 + cloudOffset, wrap), 750);
    drawCloud(fmod(1160 + cloudOffset, wrap), 750);
}

void drawMountain(float px1, float px2, float py, float pm, float h)
{
    Color mountainColor = isNight ? Color{0.12f, 0.23f, 0.12f} : Color{0.24f, 0.47f, 0.24f};
    glColor3f(mountainColor.r, mountainColor.g, mountainColor.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(px1, py);
    glVertex2f(pm, h);
    glVertex2f(px2, py);
    glEnd();

    // Snow
    glPushMatrix();
    glTranslatef(pm, h, 0.0);
    glScalef(0.2, 0.2, 1.0);
    glTranslatef(-pm, -h, 0.0);
    Color snowColor = isNight ? Color{0.5f, 0.5f, 0.5f} : Color{1.00f, 1.00f, 1.00f};
    glColor3f(snowColor.r, snowColor.g, snowColor.b);
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
    drawMountain(1200, 1600, 520, 1400, 640);
    drawMountain(1500, 1940, 520, 1700, 750);
}

void drawRoad() {
    Color roadDay = Color {0.20f, 0.20f, 0.20f};
    Color roadNight = Color {0.12f, 0.12f, 0.12f};
    drawRect(0, 370, 1920, 520, isNight ? roadNight : roadDay);

    glColor3f(1.00f, 1.00f, 1.00f);
    glLineWidth(4);
    glBegin(GL_LINES);
    for (int i = 20; i < 1920; i += 90) {
        glVertex2f(i, 445);
        glVertex2f(i + 60, 445);
    }
    glEnd();
}

void drawCar(float x, string type, string lane) {
    Color carBodyColor = type == "regular" ? Color{1.00f, 0.00f, 0.00f} : Color{0.00f, 0.30f, 1.00f};

    Color carWindowColor;
    if (isNight)
        carWindowColor = Color{1.0f, 1.0f, 0.9f};
    else
        carWindowColor = type == "regular" ? Color{0.0f, 0.91f, 0.96f} : Color{0.00f, 0.00f, 0.00f};

    float ypos = lane == "lane1" ? 400.0 : 480.0;

    glPushMatrix();
    glTranslatef(x, ypos, 0);

    if (lane == "lane2") {
        glScalef(-1.0, 1.0, 1.0);
    }

    // Roof
    drawCirclePortion(70, 30, 55, 0, 180, carBodyColor);
    drawCirclePortion(75, 35, 40, 0, 90, carWindowColor);
    drawCirclePortion(68, 35, 40, 90, 90, carWindowColor);

    // Body
    drawRoundedRect(0, 0, 155, 45, 50, 20, 6, 6, carBodyColor);

    // Tires
    drawCircle(35, -3, 20, Color{0.05f, 0.05f, 0.05f});
    drawCircle(115, -3, 20, Color{0.05f, 0.05f, 0.05f});

    // Rims
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

    // Door handles
    glLineWidth(3.5);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(35, 35);
    glVertex2f(43, 35);
    glVertex2f(80, 35);
    glVertex2f(88, 35);
    glEnd();

    // Headlight
    Color headlightColor = isNight ? Color{1.0f, 1.0f, 0.8f} : Color{1.00f, 0.88f, 0.40f};
    drawRoundedRect(138, 18, 150, 26, 5, 5, 5, 5, headlightColor);

    // Siren
    if (type == "police") {
        drawRoundedRect(64, 84, 78, 105, 5, 5, 0, 0, Color{1.00f, 0.00f, 0.00f});
    }

    glPopMatrix();
}

void drawSchoolBus(float x, string lane)
{
    float ypos = lane == "lane1" ? 400.0 : 480.0;

    glPushMatrix();
    glTranslatef(x, ypos, 0);

    if (lane == "lane2") {
        glScalef(-1.0, 1.0, 1.0);
    }

    // Body
    drawRoundedRect(0, 0, 225, 140, 30, 30, 30, 30, Color{0.98f, 0.68f, 0.05f});

    // Tires
    drawCircle(45, 0, 22, Color{0.05f, 0.05f, 0.05f});
    drawCircle(185, 0, 22, Color{0.05f, 0.05f, 0.05f});

    // Rims
    glPushMatrix();
    glTranslatef(45, 0, 0.0);
    glScalef(0.45, 0.45, 1.0);
    glTranslatef(-45, 0, 0.0);
    drawCircle(45, 0, 20, Color{0.84f, 0.80f, 0.82f});
    glPopMatrix();

    glPushMatrix();
    glTranslatef(185, 0, 0.0);
    glScalef(0.45, 0.45, 1.0);
    glTranslatef(-185, 0, 0.0);
    drawCircle(185, 0, 20, Color{0.84f, 0.80f, 0.82f});
    glPopMatrix();

    // Headlight
    drawRoundedRect(205, 28, 220, 38, 5, 5, 5, 5, Color{1.00f, 1.0f, 1.0f});

    // Front glass
    Color frontGlassColor = isNight ? Color{1.0f, 1.0f, 0.9f} : Color{0.0f, 0.0f, 0.0f};
    drawRoundedRect(200, 60, 225, 120, 0, 30, 30, 0, frontGlassColor);

    // Side windows
    Color sideWindowColor = isNight ? Color{1.0f, 1.0f, 0.9f} : Color{0.0f, 0.91f, 0.96f};
    for (int i = 20; i < 190; i += 45) {
        drawRect(i, 80, i + 30, 120, sideWindowColor);
    }

    // Door
    drawRoundedRect(110, 0, 145, 70, 5, 5, 0, 0, Color{0.45f, 0.27f, 0.13f});
    drawCircle(135, 35, 4, Color{0.98f, 0.68f, 0.05f});

    glPopMatrix();
}

void drawElectricPillar(float x)
{
    drawRoundedRect(x, 520, x + 25, 750, 10, 10, 0, 0, Color{0.55f, 0.55f, 0.55f});
    glLineWidth(20);
    glColor3f(0.50f, 0.35f, 0.20f);
    glBegin(GL_LINES);
    glVertex2f(x - 10, 770);
    glVertex2f(x + 40, 710);
    glEnd();

    if (isNight) {
        drawCircle(x + 12.5f, 750.0f, 8.0f, Color{1.0f, 0.0f, 0.0f});
    }
}

void drawElectricWires()
{
    glLineWidth(2);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0, 770);
    glVertex2f(1920, 770);
    glVertex2f(0, 710);
    glVertex2f(1920, 710);
    glEnd();
}

void drawRiver() {
    Color waterDay = Color {0.27f, 0.51f, 0.71f};
    Color waterNight = Color {0.10f, 0.27f, 0.47f};
    Color col = isNight ? waterNight : waterDay;
    drawRect(0, 0, 1920, 370, col);

    glColor3f(0.78f, 0.86f, 0.94f);
    glLineWidth(2);
    glBegin(GL_LINES);

    for (int x = 0; x < 3; x++) {
        float currentRowY = 280.0f - (x * 90.0f);
        for (int y = 0; y < 10; y++) {
            float startX = waterFlow1 + (y * 40.0f);
            startX = fmod(startX, 1920.0f + 40.0f) - 40.0f;
            glVertex2f(startX, currentRowY + 10);
            glVertex2f(startX + 20.0f, currentRowY);
        }
    }

    for (int x = 0; x < 3; x++) {
        float currentRowY = 280.0f - (x * 90.0f) - 45.0f;
        for (int y = 0; y < 10; y++) {
            float startX = waterFlow2 + (y * 40.0f);
            startX = fmod(startX, 1920.0f + 40.0f) - 40.0f;
            glVertex2f(startX, currentRowY + 10);
            glVertex2f(startX + 20.0f, currentRowY);
        }
    }
    glEnd();
}

void drawBoat(float x, string streamChannel) {
    float ypos = (streamChannel == "channel1") ? 100.0f : 240.0f;

    glPushMatrix();
    glTranslatef(x, ypos, 0);

    float boatScale = 1.25f;

    if (streamChannel == "channel2") {
        glScalef(-1.0f * boatScale, boatScale, 1.0f);
    } else {
        glScalef(boatScale, boatScale, 1.0f);
    }

    Color baseColor       = Color{0.85f, 0.20f, 0.20f};
    Color deckColor       = Color{0.95f, 0.95f, 0.95f};
    Color cabinColor      = Color{0.20f, 0.25f, 0.30f};
    Color windowColor     = isNight ? Color{1.0f, 1.0f, 0.9f} : Color{0.64f, 0.91f, 0.96f};
    Color exhaustColor    = Color{0.30f, 0.30f, 0.30f};

    drawRoundedRect(0, 0, 160, 35, 0.0, 15, 15, 0.0, baseColor);

    glBegin(GL_TRIANGLES);
        glColor3f(baseColor.r - 0.1f, baseColor.g, baseColor.b);
        glVertex2f(160, 0);
        glVertex2f(190, 35);
        glVertex2f(160, 35);
    glEnd();

    // Main cabin
    drawRoundedRect(20, 35, 145, 65, 8.0f, 8.0f, 0.0f, 0.0f, deckColor);

    // Cabin windows
    drawCircle(60, 50, 6, windowColor);
    drawCircle(90, 50, 6, windowColor);
    drawCircle(120, 50, 6, windowColor);

    // Cockpit
    drawRoundedRect(45, 65, 115, 85, 12.0f, 4.0f, 0.0f, 0.0f, cabinColor);

    // Exhaust pipe
    drawRect(30, 65, 40, 90, exhaustColor);
    glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.1f);
        glVertex2f(28, 90);
        glVertex2f(42, 90);
        glVertex2f(45, 95);
        glVertex2f(25, 95);
    glEnd();

    glPopMatrix();
}

void drawTree(float x) {
    float posy = 520;
    Color trunkColor = isNight ? Color{0.25f, 0.17f, 0.1f} : Color{0.50f, 0.35f, 0.20f};
    glColor3f(trunkColor.r, trunkColor.g, trunkColor.b);
    glRectf(x - 12, posy, x + 12, posy + 60);

    Color leafColor = isNight ? Color{0.15f, 0.35f, 0.08f} : Color{0.35f, 0.75f, 0.15f};
    drawCircle(x - 22, posy + 80, 30, leafColor);
    drawCircle(x + 22, posy + 80, 30, leafColor);
    drawCircle(x, posy + 110, 35, leafColor);
}

void drawWindmill(float x, float y) {
    drawRect(x - 8, y, x + 8, y + 130, Color{0.8f, 0.8f, 0.8f});

    float hubRadius = 8.0f;
    float bladeLength = 40.0f;
    float bladeWidth = 8.0f;
    float hubY = y + 130;

    glPushMatrix();
    glTranslatef(x, hubY, 0);
    glRotatef(windmillAngle, 0, 0, 1);

    for (int i = 0; i < 3; i++) {
        float angle = i * 120.0f;
        glPushMatrix();
        glRotatef(angle, 0, 0, 1);

        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);

        glVertex2f(hubRadius, -bladeWidth / 2.0f);
        glVertex2f(hubRadius + bladeLength, -bladeWidth / 2.0f);
        glVertex2f(hubRadius + bladeLength, bladeWidth / 2.0f);
        glVertex2f(hubRadius, bladeWidth / 2.0f);
        glEnd();

        glPopMatrix();
    }
    glPopMatrix();

    drawCircle(x, hubY, hubRadius, Color{0.2f, 0.2f, 0.2f});
}

void drawRocket() {
    glPushMatrix();
    glTranslatef(0, rocketY, 0);

    // Body
    glColor3f(0.9f, 0.9f, 0.9f);
    glRectf(50, 200, 100, 400);

    // Top face
    glColor3f(0.55f, 0.00f, 0.00f);
    glBegin(GL_TRIANGLES);
    glVertex2f(30, 400);
    glVertex2f(75, 470);
    glVertex2f(120, 400);
    glEnd();

    // Fire
    glColor3f(1.00f, 0.39f, 0.00f);
    glBegin(GL_TRIANGLES);
    glVertex2f(55, 200);
    glVertex2f(75, 160);
    glVertex2f(95, 200);
    glEnd();

    // Left wing
    glColor3f(0.12f, 0.12f, 0.12f);
    glBegin(GL_TRIANGLES);
    glVertex2f(100, 200);
    glVertex2f(130, 200);
    glVertex2f(100, 240);
    glEnd();

    // Right wing
    glColor3f(0.12f, 0.12f, 0.12f);
    glBegin(GL_TRIANGLES);
    glVertex2f(50, 200);
    glVertex2f(20, 200);
    glVertex2f(50, 240);
    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawAirplane(airplaneX);
    drawRocket();
    drawLand();

    drawWindmill(950, 520);
    drawWindmill(1320, 520);

    drawTree(400);
    drawTree(650);
    drawTree(1150);
    drawTree(1550);
    drawTree(1800);

    drawElectricPillar(1720);
    drawElectricPillar(1240);
    drawElectricPillar(760);
    drawElectricPillar(280);

    drawElectricWires();
    drawRoad();

    drawCar(car1_lane2_x, "police", "lane2");
    drawCar(car2_lane2_x, "regular", "lane2");
    drawSchoolBus(bus1_lane2_x, "lane2");

    drawCar(car1_lane1_x, "regular", "lane1");
    drawCar(car2_lane1_x, "police", "lane1");
    drawSchoolBus(bus1_lane1_x, "lane1");

    drawRiver();

    drawBoat(boat1_ch2_x, "channel2");
    drawBoat(boat2_ch2_x, "channel2");
    drawBoat(boat1_ch1_x, "channel1");
    drawBoat(boat2_ch1_x, "channel1");

    glutSwapBuffers();
}

void update(int value) {
    cloudOffset += 1.5f * speedFactor;
    if (cloudOffset > 1920.0f) cloudOffset -= 1920.0f;

    waterFlow1 += 2.0f * speedFactor;
    waterFlow2 += 2.0f * speedFactor;

    windmillAngle += 1.5f * speedFactor;
    if (windmillAngle > 360.0f) windmillAngle -= 360.0f;

    airplaneX += 3.5f * speedFactor;
    if (airplaneX > 1920 + 200) airplaneX = -200;

    car1_lane1_x += 2.5f * speedFactor;
    if (car1_lane1_x > 1920 + 200) car1_lane1_x = -200;
    car2_lane1_x += 2.5f * speedFactor;
    if (car2_lane1_x > 1920 + 200) car2_lane1_x = -200;
    bus1_lane1_x += 2.0f * speedFactor;
    if (bus1_lane1_x > 1920 + 300) bus1_lane1_x = -300;

    car1_lane2_x -= 2.5f * speedFactor;
    if (car1_lane2_x < -200) car1_lane2_x = 1920 + 200;
    car2_lane2_x -= 2.5f * speedFactor;
    if (car2_lane2_x < -200) car2_lane2_x = 1920 + 200;
    bus1_lane2_x -= 2.0f * speedFactor;
    if (bus1_lane2_x < -300) bus1_lane2_x = 1920 + 300;

    boat1_ch1_x += 1.8f * speedFactor;
    if (boat1_ch1_x > 1920 + 200) boat1_ch1_x = -200;
    boat2_ch1_x += 1.8f * speedFactor;
    if (boat2_ch1_x > 1920 + 200) boat2_ch1_x = -200;

    boat1_ch2_x -= 1.8f * speedFactor;
    if (boat1_ch2_x < -200) boat1_ch2_x = 1920 + 200;
    boat2_ch2_x -= 1.8f * speedFactor;
    if (boat2_ch2_x < -200) boat2_ch2_x = 1920 + 200;

    if (rocketLaunched) {
        rocketY += 3.0f * speedFactor;
        if (rocketY > 1080) rocketY = -200;
    }

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'd': case 'D': isNight = false; break;
        case 'n': case 'N': isNight = true; break;
        case 'l': case 'L': rocketLaunched = true; break;
        case 'r': case 'R': rocketLaunched = false; rocketY = 320.0f; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            speedFactor += 0.2f;
            if (speedFactor > 3.0f) speedFactor = 3.0f;
            break;
        case GLUT_KEY_DOWN:
            speedFactor -= 0.2f;
            if (speedFactor < 0.2f) speedFactor = 0.2f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Space Port");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(0, 1920, 0, 1080);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(30, update, 0);
    glutMainLoop();

    return 0;
}
