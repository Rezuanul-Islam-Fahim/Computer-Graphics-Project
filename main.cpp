#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

const int WIDTH = 800;
const int HEIGHT = 600;

bool isNight = false;
float speedFactor = 0.6f;
float fanAngle = 0.0f;
float waterOffset = 0.0f;

float boatSpeedMultiplier = 1.0f;
float targetBoatMultiplier = 1.0f;

struct Boat {
    float x;
    float y;
    float baseSpeed;
};

Boat boats[5] = {
    {60.0f, 290.0f, 0.6f},
    {180.0f, 298.0f, 0.55f},
    {320.0f, 295.0f, 0.45f},
    {480.0f, 292.0f, 0.7f},
    {650.0f, 300.0f, 0.5f}
};

struct Vehicle {
    float x;
    float y;
    float width;
    float height;
    float baseSpeed;
    int type;
};

Vehicle vehicles[5] = {
    {50.0f, 75.0f, 65.0f, 38.0f, 1.8f, 0},
    {250.0f, 30.0f, 140.0f, 70.0f, 1.4f, 1},
    {500.0f, 55.0f, 60.0f, 36.0f, 2.0f, 0},
    {680.0f, 85.0f, 145.0f, 72.0f, 1.5f, 1},
    {150.0f, 40.0f, 70.0f, 40.0f, 1.9f, 0}
};

const float RIVER_LEFT = 0.0f;
const float RIVER_RIGHT = (float)WIDTH;

struct Star {
    int x, y;
    float brightness;
};
const int NUM_STARS = 120;
Star stars[NUM_STARS];

void initStars() {
    srand(42);
    for (int i = 0; i < NUM_STARS; ++i) {
        stars[i].x = rand() % WIDTH;
        stars[i].y = rand() % (HEIGHT - 150);
        stars[i].brightness = 0.4f + (rand() % 100) / 100.0f * 0.6f;
    }
}

void drawCircle(float cx, float cy, float radius, int segments = 40) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawSunMoon() {
    if (isNight) {
        glColor3f(0.9f, 0.9f, 1.0f);
        drawCircle(700, 520, 30);
        glColor3f(0.7f, 0.7f, 0.8f);
        drawCircle(690, 525, 6);
        drawCircle(710, 515, 4);
        drawCircle(695, 510, 3);
    } else {
        glColor3f(1.0f, 0.9f, 0.2f);
        drawCircle(700, 520, 35);
        glColor3f(1.0f, 0.7f, 0.1f);
        drawCircle(700, 520, 25);
        glColor3f(1.0f, 0.8f, 0.0f);
        for (int i = 0; i < 12; ++i) {
            float angle = i * 30.0f * M_PI / 180.0f;
            float dx = cos(angle);
            float dy = sin(angle);
            glBegin(GL_LINES);
            glVertex2f(700 + 38 * dx, 520 + 38 * dy);
            glVertex2f(700 + 52 * dx, 520 + 52 * dy);
            glEnd();
        }
    }
}

void drawHouse(float x, float y, float scale = 1.0f) {
    float wallR, wallG, wallB, roofR, roofG, roofB, doorR, doorG, doorB, windowR, windowG, windowB;
    if (isNight) {
        wallR = 0.35f; wallG = 0.25f; wallB = 0.2f;
        roofR = 0.4f; roofG = 0.1f; roofB = 0.1f;
        doorR = 0.3f; doorG = 0.2f; doorB = 0.15f;
        windowR = 0.9f; windowG = 0.8f; windowB = 0.6f;
    } else {
        wallR = 0.95f; wallG = 0.8f; wallB = 0.6f;
        roofR = 0.8f; roofG = 0.2f; roofB = 0.2f;
        doorR = 0.55f; doorG = 0.35f; doorB = 0.2f;
        windowR = 0.3f; windowG = 0.5f; windowB = 0.9f;
    }
    float w = 60 * scale;
    float h = 55 * scale;
    float roofOverhang = 8 * scale;
    float roofHeight = 35 * scale;
    float doorW = 20 * scale;
    float doorH = 38 * scale;
    float doorXoff = 20 * scale;
    float windowW = 10 * scale;
    float windowH = 15 * scale;
    float windowY = 25 * scale;
    float chimneyW = 10 * scale;
    float chimneyH = 18 * scale;
    float chimneyXoff = 48 * scale;
    float chimneyYbase = 70 * scale;

    glColor3f(wallR, wallG, wallB);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    glColor3f(roofR, roofG, roofB);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - roofOverhang, y + h);
    glVertex2f(x + w/2, y + h + roofHeight);
    glVertex2f(x + w + roofOverhang, y + h);
    glEnd();
    glColor3f(doorR, doorG, doorB);
    glBegin(GL_QUADS);
    glVertex2f(x + doorXoff, y);
    glVertex2f(x + doorXoff + doorW, y);
    glVertex2f(x + doorXoff + doorW, y + doorH);
    glVertex2f(x + doorXoff, y + doorH);
    glEnd();
    glColor3f(0.9f, 0.8f, 0.2f);
    drawCircle(x + doorXoff + doorW - 3*scale, y + doorH/2, 2*scale, 10);
    glColor3f(windowR, windowG, windowB);
    glBegin(GL_QUADS);
    glVertex2f(x + 8*scale, y + windowY);
    glVertex2f(x + 8*scale + windowW, y + windowY);
    glVertex2f(x + 8*scale + windowW, y + windowY + windowH);
    glVertex2f(x + 8*scale, y + windowY + windowH);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(x + w - 8*scale - windowW, y + windowY);
    glVertex2f(x + w - 8*scale, y + windowY);
    glVertex2f(x + w - 8*scale, y + windowY + windowH);
    glVertex2f(x + w - 8*scale - windowW, y + windowY + windowH);
    glEnd();
    glColor3f(0.4f, 0.3f, 0.2f);
    glBegin(GL_LINES);
    glVertex2f(x + 8*scale + windowW/2, y + windowY);
    glVertex2f(x + 8*scale + windowW/2, y + windowY + windowH);
    glVertex2f(x + 8*scale, y + windowY + windowH/2);
    glVertex2f(x + 8*scale + windowW, y + windowY + windowH/2);
    glVertex2f(x + w - 8*scale - windowW/2, y + windowY);
    glVertex2f(x + w - 8*scale - windowW/2, y + windowY + windowH);
    glVertex2f(x + w - 8*scale - windowW, y + windowY + windowH/2);
    glVertex2f(x + w - 8*scale, y + windowY + windowH/2);
    glEnd();
    glColor3f(0.5f, 0.3f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x + chimneyXoff, y + chimneyYbase);
    glVertex2f(x + chimneyXoff + chimneyW, y + chimneyYbase);
    glVertex2f(x + chimneyXoff + chimneyW, y + chimneyYbase + chimneyH);
    glVertex2f(x + chimneyXoff, y + chimneyYbase + chimneyH);
    glEnd();
}

void drawWindmill(float baseX, float baseY) {
    float towerR, towerG, towerB, bladeR, bladeG, bladeB;
    if (isNight) {
        towerR = 0.4f; towerG = 0.35f; towerB = 0.3f;
        bladeR = 0.45f; bladeG = 0.45f; bladeB = 0.5f;
    } else {
        towerR = 0.75f; towerG = 0.7f; towerB = 0.65f;
        bladeR = 0.85f; bladeG = 0.85f; bladeB = 0.9f;
    }
    float towerWidth = 30.0f;
    float towerHeight = 75.0f;
    float topWidth = 18.0f;
    glColor3f(towerR, towerG, towerB);
    glBegin(GL_QUADS);
    glVertex2f(baseX - towerWidth/2, baseY);
    glVertex2f(baseX + towerWidth/2, baseY);
    glVertex2f(baseX + topWidth/2, baseY + towerHeight);
    glVertex2f(baseX - topWidth/2, baseY + towerHeight);
    glEnd();
    if (isNight) glColor3f(0.9f, 0.8f, 0.5f);
    else glColor3f(0.2f, 0.3f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(baseX - 6, baseY + 25);
    glVertex2f(baseX + 6, baseY + 25);
    glVertex2f(baseX + 6, baseY + 38);
    glVertex2f(baseX - 6, baseY + 38);
    glEnd();
    float cx = baseX;
    float cy = baseY + towerHeight;
    float bladeLength = 55.0f;
    float bladeWidth = 12.0f;
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glRotatef(fanAngle, 0.0f, 0.0f, 1.0f);
    glColor3f(bladeR, bladeG, bladeB);
    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glRotatef(i * 90.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(-bladeWidth/2, 0);
        glVertex2f( bladeWidth/2, 0);
        glVertex2f( bladeWidth/2, bladeLength);
        glVertex2f(-bladeWidth/2, bladeLength);
        glEnd();
        glColor3f(0.5f, 0.3f, 0.2f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-bladeWidth/3, bladeLength);
        glVertex2f( bladeWidth/3, bladeLength);
        glVertex2f(0, bladeLength + 8);
        glEnd();
        glColor3f(bladeR, bladeG, bladeB);
        glPopMatrix();
    }
    glColor3f(0.6f, 0.5f, 0.4f);
    drawCircle(0, 0, 10, 20);
    glPopMatrix();
}

void drawGround() {
    if (isNight) glColor3f(0.08f, 0.15f, 0.04f);
    else glColor3f(0.35f, 0.65f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WIDTH, 0);
    glVertex2f(WIDTH, 270);
    glVertex2f(0, 270);
    glEnd();
    glColor3f(0.2f, 0.5f, 0.15f);
    for (int i = 0; i < 150; ++i) {
        float x = (i * 23) % WIDTH;
        float height = 8 + (i % 12);
        glBegin(GL_LINES);
        glVertex2f(x, 270);
        glVertex2f(x, 270 + height);
        glEnd();
    }
}

void drawRiver() {
    if (isNight) glColor3f(0.03f, 0.08f, 0.35f);
    else glColor3f(0.2f, 0.55f, 0.85f);
    glBegin(GL_QUADS);
    glVertex2f(RIVER_LEFT, 275);
    glVertex2f(RIVER_RIGHT, 275);
    glVertex2f(RIVER_RIGHT, 370);
    glVertex2f(RIVER_LEFT, 370);
    glEnd();
    float streakYStart = 295.0f;
    float streakYEnd = 350.0f;
    int numStreaks = 6;
    for (int i = 0; i < numStreaks; ++i) {
        float y = streakYStart + (i * (streakYEnd - streakYStart) / numStreaks);
        for (int seg = 0; seg < 6; ++seg) {
            float x = RIVER_LEFT + fmod(waterOffset + seg * 60.0f + i * 45.0f, RIVER_RIGHT - RIVER_LEFT);
            if (x < RIVER_RIGHT && x > RIVER_LEFT) {
                if (isNight) glColor3f(0.35f, 0.45f, 0.7f);
                else glColor3f(0.6f, 0.85f, 1.0f);
                glBegin(GL_QUADS);
                glVertex2f(x, y - 2);
                glVertex2f(x + 18, y - 2);
                glVertex2f(x + 18, y + 2);
                glVertex2f(x, y + 2);
                glEnd();
            }
        }
    }
    for (int i = 0; i < 15; ++i) {
        float x = RIVER_LEFT + fmod(waterOffset * 1.3f + i * 70.0f, RIVER_RIGHT - RIVER_LEFT);
        float y = 305 + (i % 15);
        if (x < RIVER_RIGHT && x > RIVER_LEFT) {
            if (isNight) glColor3f(0.3f, 0.4f, 0.65f);
            else glColor3f(0.5f, 0.8f, 1.0f);
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 6, y);
            glVertex2f(x + 6, y + 3);
            glVertex2f(x, y + 3);
            glEnd();
        }
    }
    glColor3f(0.5f, 0.4f, 0.3f);
    for (int i = 0; i < 30; ++i) {
        float x = i * 30.0f;
        drawCircle(x, 274, 3, 8);
        drawCircle(x, 371, 3, 8);
    }
}

void drawBoat(float x, float y, float scale = 1.0f) {
    float hullW = 36.0f * scale;
    float hullH = 11.0f * scale;
    float mastH = 22.0f * scale;
    float sailW = 20.0f * scale;
    float mastW = 4.0f * scale;
    if (isNight) glColor3f(0.35f, 0.25f, 0.18f);
    else glColor3f(0.65f, 0.45f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(x - hullW/2, y - hullH/2);
    glVertex2f(x + hullW/2, y - hullH/2);
    glVertex2f(x + hullW/3, y + hullH/2);
    glVertex2f(x - hullW/3, y + hullH/2);
    glEnd();
    glColor3f(0.55f, 0.45f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(x - mastW/2, y);
    glVertex2f(x + mastW/2, y);
    glVertex2f(x + mastW/2, y + mastH);
    glVertex2f(x - mastW/2, y + mastH);
    glEnd();
    if (isNight) glColor3f(0.7f, 0.7f, 0.8f);
    else glColor3f(1.0f, 1.0f, 0.95f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + mastW/2, y + mastH);
    glVertex2f(x + sailW, y + mastH/2);
    glVertex2f(x + mastW/2, y + mastH/2);
    glEnd();
    glColor3f(0.9f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + mastW/2, y + mastH);
    glVertex2f(x + mastW/2 + 8*scale, y + mastH - 4*scale);
    glVertex2f(x + mastW/2, y + mastH - 4*scale);
    glEnd();
    if (isNight) glColor3f(0.5f, 0.4f, 0.3f);
    else glColor3f(0.8f, 0.6f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(x - hullW/4, y + hullH/4);
    glVertex2f(x - hullW/8, y + hullH/4);
    glVertex2f(x - hullW/8, y + hullH/2);
    glVertex2f(x - hullW/4, y + hullH/2);
    glEnd();
    glColor3f(0.9f, 0.8f, 0.5f);
    drawCircle(x - hullW/6, y + hullH/3, 1.5f * scale, 6);
}

void drawStars() {
    if (!isNight) return;
    for (int i = 0; i < NUM_STARS; ++i) {
        glColor3f(stars[i].brightness, stars[i].brightness, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(stars[i].x, stars[i].y);
        glVertex2f(stars[i].x + 2, stars[i].y);
        glVertex2f(stars[i].x + 2, stars[i].y + 2);
        glVertex2f(stars[i].x, stars[i].y + 2);
        glEnd();
    }
}

void drawTree(float x, float y, float scale) {
    float trunkW = 12.0f * scale;
    float trunkH = 28.0f * scale;
    float radius = 16.0f * scale;
    if (isNight) glColor3f(0.3f, 0.2f, 0.12f);
    else glColor3f(0.55f, 0.35f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(x - trunkW/2, y);
    glVertex2f(x + trunkW/2, y);
    glVertex2f(x + trunkW/2, y + trunkH);
    glVertex2f(x - trunkW/2, y + trunkH);
    glEnd();
    if (isNight) glColor3f(0.08f, 0.25f, 0.08f);
    else glColor3f(0.2f, 0.7f, 0.25f);
    drawCircle(x, y + trunkH + radius*0.6, radius, 20);
    drawCircle(x - radius*0.5, y + trunkH + radius*0.2, radius*0.75, 20);
    drawCircle(x + radius*0.5, y + trunkH + radius*0.2, radius*0.75, 20);
}

void drawRoad() {
    if (isNight) glColor3f(0.12f, 0.12f, 0.18f);
    else glColor3f(0.28f, 0.28f, 0.32f);
    glBegin(GL_QUADS);
    glVertex2f(0, 30);
    glVertex2f(WIDTH, 30);
    glVertex2f(WIDTH, 110);
    glVertex2f(0, 110);
    glEnd();
    glColor3f(0.9f, 0.9f, 0.5f);
    for (int i = 0; i < 20; ++i) {
        float x = i * 45.0f;
        glBegin(GL_QUADS);
        glVertex2f(x, 68);
        glVertex2f(x + 25, 68);
        glVertex2f(x + 25, 72);
        glVertex2f(x, 72);
        glEnd();
    }
}

void drawVehicle(float x, float y, float w, float h, int type) {
    if (type == 0) {
        if (isNight) glColor3f(0.25f, 0.15f, 0.2f);
        else glColor3f(0.9f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
        glColor3f(0.4f, 0.4f, 0.5f);
        glBegin(GL_QUADS);
        glVertex2f(x + w*0.15, y + h*0.6);
        glVertex2f(x + w*0.85, y + h*0.6);
        glVertex2f(x + w*0.8, y + h);
        glVertex2f(x + w*0.2, y + h);
        glEnd();
        glColor3f(0.7f, 0.8f, 0.9f);
        glBegin(GL_QUADS);
        glVertex2f(x + w*0.2, y + h*0.65);
        glVertex2f(x + w*0.45, y + h*0.65);
        glVertex2f(x + w*0.45, y + h*0.85);
        glVertex2f(x + w*0.2, y + h*0.85);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(x + w*0.55, y + h*0.65);
        glVertex2f(x + w*0.8, y + h*0.65);
        glVertex2f(x + w*0.8, y + h*0.85);
        glVertex2f(x + w*0.55, y + h*0.85);
        glEnd();
        glColor3f(0.1f, 0.1f, 0.1f);
        drawCircle(x + w*0.25, y, 6, 12);
        drawCircle(x + w*0.75, y, 6, 12);
        if (isNight) glColor3f(1.0f, 0.9f, 0.4f);
        else glColor3f(1.0f, 1.0f, 0.3f);
        drawCircle(x + w - 5, y + h*0.5, 3, 8);
        drawCircle(x + 5, y + h*0.5, 3, 8);
    } else {
        if (isNight) glColor3f(0.15f, 0.25f, 0.3f);
        else glColor3f(0.2f, 0.5f, 0.9f);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
        glColor3f(0.7f, 0.8f, 0.9f);
        int numWindows = 3;
        float windowW = w * 0.15;
        float windowH = h * 0.28;
        float windowY = y + h * 0.52;
        float spacing = (w - 2 * windowW) / (numWindows + 1);
        for (int i = 0; i < numWindows; ++i) {
            float wx = x + spacing + i * (windowW + spacing);
            glBegin(GL_QUADS);
            glVertex2f(wx, windowY);
            glVertex2f(wx + windowW, windowY);
            glVertex2f(wx + windowW, windowY + windowH);
            glVertex2f(wx, windowY + windowH);
            glEnd();
        }
        glColor3f(0.7f, 0.6f, 0.4f);
        float doorW = w * 0.14;
        float doorH = h * 0.6;
        float doorX = x + w * 0.43;
        glBegin(GL_QUADS);
        glVertex2f(doorX, y);
        glVertex2f(doorX + doorW, y);
        glVertex2f(doorX + doorW, y + doorH);
        glVertex2f(doorX, y + doorH);
        glEnd();
        glColor3f(0.1f, 0.1f, 0.1f);
        drawCircle(x + w*0.25, y, 8, 12);
        drawCircle(x + w*0.75, y, 8, 12);
        if (isNight) glColor3f(1.0f, 0.8f, 0.3f);
        else glColor3f(1.0f, 0.7f, 0.1f);
        drawCircle(x + w - 6, y + h*0.35, 4, 8);
        drawCircle(x + 6, y + h*0.35, 4, 8);
    }
}

void drawClouds() {
    if (isNight) return;
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(120, 500, 25, 20);
    drawCircle(150, 510, 30, 20);
    drawCircle(180, 500, 25, 20);
    drawCircle(150, 495, 22, 20);
    drawCircle(450, 480, 20, 20);
    drawCircle(475, 490, 28, 20);
    drawCircle(500, 482, 22, 20);
}

bool compareVehicleY(const Vehicle &a, const Vehicle &b) {
    return a.y > b.y;
}

void display() {
    if (isNight) glClearColor(0.02f, 0.02f, 0.1f, 1.0f);
    else glClearColor(0.45f, 0.75f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    drawStars();
    drawClouds();
    drawGround();
    drawRiver();
    for (int i = 0; i < 5; ++i) {
        float boatX = boats[i].x;
        float distToLeft = fabs(boatX - 45.0f);
        float distToRight = fabs(boatX - 730.0f);
        float minDist = std::min(distToLeft, distToRight);
        float scale = 1.5f - (minDist / 400.0f) * 0.8f;
        if (scale < 0.7f) scale = 0.7f;
        if (scale > 1.5f) scale = 1.5f;
        drawBoat(boats[i].x, boats[i].y, scale);
    }
    drawTree(35, 240, 2.0f);
    drawHouse(15, 115, 1.5f);
    drawHouse(480, 240, 1.0f);
    drawHouse(700, 225, 1.0f);
    drawTree(130, 120, 1.1f);
    drawTree(200, 235, 0.9f);
    drawTree(300, 245, 1.3f);
    drawTree(410, 245, 0.8f);
    drawTree(450, 240, 1.0f);
    drawTree(590, 242, 1.2f);
    drawTree(640, 238, 0.7f);
    drawTree(660, 235, 0.9f);
    drawTree(780, 238, 1.1f);
    drawWindmill(380, 240);
    drawRoad();
    Vehicle sortedVehicles[5];
    for (int i = 0; i < 5; ++i) sortedVehicles[i] = vehicles[i];
    std::sort(sortedVehicles, sortedVehicles + 5, compareVehicleY);
    for (int i = 0; i < 5; ++i) {
        drawVehicle(sortedVehicles[i].x, sortedVehicles[i].y,
                    sortedVehicles[i].width, sortedVehicles[i].height,
                    sortedVehicles[i].type);
    }
    drawSunMoon();
    glutSwapBuffers();
}

void updateScene() {
    fanAngle += 2.0f * speedFactor;
    if (fanAngle > 360.0f) fanAngle -= 360.0f;
    waterOffset += 1.8f * speedFactor;
    if (waterOffset > 200.0f) waterOffset -= 200.0f;
    if (boatSpeedMultiplier < targetBoatMultiplier) {
        boatSpeedMultiplier += 0.005f;
        if (boatSpeedMultiplier > targetBoatMultiplier) boatSpeedMultiplier = targetBoatMultiplier;
    } else if (boatSpeedMultiplier > targetBoatMultiplier) {
        boatSpeedMultiplier -= 0.005f;
        if (boatSpeedMultiplier < targetBoatMultiplier) boatSpeedMultiplier = targetBoatMultiplier;
    }
    for (int i = 0; i < 5; ++i) {
        boats[i].x += boats[i].baseSpeed * speedFactor * boatSpeedMultiplier;
        if (boats[i].x > RIVER_RIGHT + 30) boats[i].x = RIVER_LEFT - 30;
        if (boats[i].x < RIVER_LEFT - 30) boats[i].x = RIVER_RIGHT + 30;
    }
    bool collision = true;
    int iter = 0;
    while (collision && iter < 10) {
        collision = false;
        for (int i = 0; i < 5; ++i) {
            for (int j = i + 1; j < 5; ++j) {
                float dx = boats[i].x - boats[j].x;
                float dy = boats[i].y - boats[j].y;
                float dist = sqrt(dx*dx + dy*dy);
                float minDist = 25.0f;
                if (dist < minDist) {
                    collision = true;
                    float angle = atan2(dy, dx);
                    float overlap = minDist - dist;
                    float move = overlap * 0.5f;
                    boats[i].x += move * cos(angle);
                    boats[i].y += move * sin(angle);
                    boats[j].x -= move * cos(angle);
                    boats[j].y -= move * sin(angle);
                    boats[i].x = std::max(RIVER_LEFT - 30.0f, std::min(RIVER_RIGHT + 30.0f, boats[i].x));
                    boats[i].y = std::max(280.0f, std::min(310.0f, boats[i].y));
                    boats[j].x = std::max(RIVER_LEFT - 30.0f, std::min(RIVER_RIGHT + 30.0f, boats[j].x));
                    boats[j].y = std::max(280.0f, std::min(310.0f, boats[j].y));
                }
            }
        }
        iter++;
    }
    for (int i = 0; i < 5; ++i) {
        vehicles[i].x += vehicles[i].baseSpeed * speedFactor;
        if (vehicles[i].x > WIDTH + 200) vehicles[i].x = -200;
        if (vehicles[i].x < -200) vehicles[i].x = WIDTH + 200;
    }
    glutPostRedisplay();
}

void idle() {
    updateScene();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'n':
        case 'N':
            isNight = true;
            targetBoatMultiplier = 0.0f;
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            isNight = false;
            targetBoatMultiplier = 1.0f;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            speedFactor += 0.15f;
            if (speedFactor > 3.5f) speedFactor = 3.5f;
            printf("Speed increased to: %.2f\n", speedFactor);
            break;
        case GLUT_KEY_DOWN:
            speedFactor -= 0.15f;
            if (speedFactor < 0.2f) speedFactor = 0.2f;
            printf("Speed decreased to: %.2f\n", speedFactor);
            break;
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.5f);
    initStars();
    boats[0].x = 60.0f; boats[0].y = 290.0f; boats[0].baseSpeed = 0.6f;
    boats[1].x = 180.0f; boats[1].y = 298.0f; boats[1].baseSpeed = 0.55f;
    boats[2].x = 320.0f; boats[2].y = 295.0f; boats[2].baseSpeed = 0.45f;
    boats[3].x = 480.0f; boats[3].y = 292.0f; boats[3].baseSpeed = 0.7f;
    boats[4].x = 650.0f; boats[4].y = 300.0f; boats[4].baseSpeed = 0.5f;
    fanAngle = 0.0f;
    waterOffset = 0.0f;
    speedFactor = 0.6f;
    isNight = false;
    boatSpeedMultiplier = 1.0f;
    targetBoatMultiplier = 1.0f;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Village with River");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
