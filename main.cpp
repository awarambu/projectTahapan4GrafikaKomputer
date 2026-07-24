/**
 * TAHAP 3 - ANIMASI KOMPLEKS CANDI 3D (FINAL)
 *
 * Nama  : Geby Rambu Awa
 * NIM   : 248111064
 * Kelas : B
 * Matkul: Grafika Komputer
 *
 * Fitur Final:
 *   ✅ 12 animasi kompleks (api, asap, riak, burung, kelopak, matahari, awan,
 *      lampu, kunang-kunang, pintu, peziarah, lonceng)
 *   ✅ Siklus siang-senja-malam halus dengan warna oranye di senja
 *   ✅ 12 peziarah bergerak dengan pola berbeda
 *   ✅ Warna langit berubah otomatis
 *   ✅ Matahari bergerak (mempengaruhi pencahayaan)
 *   ✅ Bulan muncul saat malam
 *   ✅ Bintang berkelap-kelip saat malam
 *   ✅ Lampu taman otomatis menyala saat malam
 *   ✅ Obor memancarkan efek api
 *   ✅ Kabut (fog) pagi/malam dinamis
 *   ✅ Intensitas cahaya berubah sesuai waktu
 *   ✅ Material candi lebih realistis (specular)
 *   ✅ Pencahayaan natural (matahari + bulan)
 *   ✅ Semua menggunakan OpenGL + GLUT (tanpa shader)
 *
 * Keyboard:
 *  WASD  : gerak (mode First-Person)
 *  Space : toggle First-Person / Orbit
 *  C     : Cinematic mode
 *  L     : toggle lampu taman (manual override)
 *  R     : reset kamera ke Orbit
 *  O     : buka pintu gapura (manual override)
 *  K     : tutup pintu gapura (manual override)
 *  Mouse : lihat/orbit (tahan kiri)
 *  Scroll: zoom in/out (Orbit)
 */

#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

// ============================================================================
// VARIABEL GLOBAL
// ============================================================================

// Kamera
float cameraAngleX = 25.0f;
float cameraAngleY = -45.0f;
float cameraDistance = 60.0f;
float cameraTargetX = 0.0f, cameraTargetY = 0.0f, cameraTargetZ = 0.0f;

// First-person
float camX = 0.0f, camY = 5.0f, camZ = 35.0f;
float camYaw = 0.0f, camPitch = -10.0f;

int cameraMode = 0;              // 0=Orbit, 1=First-Person, 2=Cinematic
int mouseX = 0, mouseY = 0;
bool mouseDown = false;
int windowWidth = 1024;
int windowHeight = 768;
unsigned int textures[10];

// Animasi
float animTime = 0.0f;
bool lampuMenyala = true;
float pintuGapuraAngle = 0.0f;
bool pintuManual = false;
float pintuTargetAngle = 0.0f;
float sunAngle = 1.1f;
float cinematicAngle = 0.0f;

// Siklus siang-malam
float dayFactor = 0.5f;          // 1=siang, 0=malam
float moonAngle = 0.0f;
float fogStart = 55.0f;
float fogEnd = 140.0f;

// Bintang
struct Star {
    float x, y, z;
    float brightness;
};
std::vector<Star> stars;

// ============================================================================
// FUNGSI TEKSTUR
// ============================================================================

void createTexture(unsigned int texID, int width, int height, unsigned char* data) {
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void initTextures() {
    glGenTextures(10, textures);
    int w = 64, h = 64;
    unsigned char* data = new unsigned char[w * h * 4];

    // Texture 0: putih
    for (int i = 0; i < w*h; i++) {
        data[i*4+0] = 255; data[i*4+1] = 255; data[i*4+2] = 255; data[i*4+3] = 255;
    }
    createTexture(textures[0], w, h, data);

    // Texture 1: batu
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        int noise = ((x*13 + y*7) % 31) & 0xFF;
        unsigned char c = 100 + (noise % 80);
        data[i*4+0] = c + 30;
        data[i*4+1] = c;
        data[i*4+2] = c - 20;
        data[i*4+3] = 255;
    }
    createTexture(textures[1], w, h, data);

    // Texture 2: kayu
    for (int i = 0; i < w*h; i++) {
        int y = i / w;
        int stripe = (y % 8 < 4) ? 0 : 40;
        unsigned char c = 120 + stripe;
        data[i*4+0] = c + 50;
        data[i*4+1] = c - 10;
        data[i*4+2] = c - 40;
        data[i*4+3] = 255;
    }
    createTexture(textures[2], w, h, data);

    // Texture 3: merah bata
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        int var = ((x+y) % 3) * 15;
        data[i*4+0] = 180 + var;
        data[i*4+1] = 60 + var;
        data[i*4+2] = 40 + var;
        data[i*4+3] = 255;
    }
    createTexture(textures[3], w, h, data);

    // Texture 4: rumput
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        int var = ((x*3 + y*7) % 5) * 10;
        data[i*4+0] = 30 + var;
        data[i*4+1] = 150 + var;
        data[i*4+2] = 30 + var;
        data[i*4+3] = 255;
    }
    createTexture(textures[4], w, h, data);

    // Texture 5: air
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        int var = (int)(15.0f * sinf(x*0.3f) * cosf(y*0.3f));
        data[i*4+0] = 30 + var;
        data[i*4+1] = 80 + var;
        data[i*4+2] = 180 + var;
        data[i*4+3] = 255;
    }
    createTexture(textures[5], w, h, data);

    // Texture 6: emas
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        int var = ((x+y) % 4) * 20;
        data[i*4+0] = 220 + var;
        data[i*4+1] = 180 + var;
        data[i*4+2] = 40 + var;
        data[i*4+3] = 255;
    }
    createTexture(textures[6], w, h, data);

    // Texture 7: abu-abu
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        int noise = ((x*17 + y*13) % 29) & 0xFF;
        unsigned char c = 140 + (noise % 40);
        data[i*4+0] = c;
        data[i*4+1] = c;
        data[i*4+2] = c;
        data[i*4+3] = 255;
    }
    createTexture(textures[7], w, h, data);

    // Texture 8: checkerboard
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        int checker = ((x/8) + (y/8)) % 2;
        unsigned char c = checker ? 200 : 60;
        data[i*4+0] = c;
        data[i*4+1] = c;
        data[i*4+2] = c;
        data[i*4+3] = 255;
    }
    createTexture(textures[8], w, h, data);

    // Texture 9: gradasi biru
    for (int i = 0; i < w*h; i++) {
        int x = i % w, y = i / w;
        float t = (float)(x + y) / (w + h);
        unsigned char c = (unsigned char)(150 + 105 * t);
        data[i*4+0] = c;
        data[i*4+1] = c - 30;
        data[i*4+2] = 255 - 80 * t;
        data[i*4+3] = 255;
    }
    createTexture(textures[9], w, h, data);

    delete[] data;
}

// ============================================================================
// FUNGSI PRIMITIF DASAR
// ============================================================================

void drawCube(float w, float h, float d, unsigned int textureID, float specIntensity) {
    float matAmbient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
    float matDiffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
    float matSpecular[] = { specIntensity, specIntensity, specIntensity, 1.0f };
    float matShininess[] = { 64.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    if (textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    float x = w / 2.0f;
    float y = h / 2.0f;
    float z = d / 2.0f;

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);

    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x,  y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( x,  y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);

    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);

    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y, -z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawSphere(float radius, int slices, int stacks) {
    glutSolidSphere(radius, slices, stacks);
}

void drawCone(float radius, float height, int segments) {
    glutSolidCone(radius, height, segments, segments);
}

void drawCylinder(float radius, float height, int segments) {
    glutSolidCylinder(radius, height, segments, segments);
}

void drawQuad(float x1, float y1, float z1,
              float x2, float y2, float z2,
              float x3, float y3, float z3,
              float x4, float y4, float z4) {
    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();
}

// ============================================================================
// PEMODELAN OBJEK STATIS & ANIMASI
// ============================================================================

const float GAPURA_KEDUA_Z = -12.0f;

void drawGapuraUtama() {
    glPushMatrix();
    glTranslatef(0, 0, -28);
    for (int side = -1; side <= 1; side += 2) {
        glPushMatrix();
        glTranslatef(side * 3.5f, 0, 0);
        glColor3f(0.5f, 0.4f, 0.3f);
        drawCube(2.0f, 0.5f, 2.5f, textures[1], 0.2f);
        glColor3f(0.55f, 0.45f, 0.35f);
        glPushMatrix();
        glTranslatef(0, 1.0f, 0);
        drawCube(1.8f, 2.0f, 2.3f, textures[1], 0.2f);
        glPopMatrix();
        glColor3f(0.5f, 0.38f, 0.28f);
        glPushMatrix();
        glTranslatef(0, 2.5f, 0);
        drawCone(1.6f, 1.8f, 8);
        glPopMatrix();
        glColor3f(0.45f, 0.35f, 0.25f);
        glPushMatrix();
        glTranslatef(0, 3.8f, 0);
        drawCone(1.2f, 1.2f, 8);
        glPopMatrix();
        glColor3f(0.6f, 0.5f, 0.3f);
        glPushMatrix();
        glTranslatef(0, 4.8f, 0);
        drawSphere(0.3f, 8, 8);
        glPopMatrix();
        glColor3f(0.4f, 0.35f, 0.3f);
        for (int i = -1; i <= 1; i++) {
            glPushMatrix();
            glTranslatef(i * 0.5f, 1.2f, 1.16f);
            drawCube(0.3f, 0.4f, 0.1f, textures[7], 0.2f);
            glPopMatrix();
        }
        glPopMatrix();
    }
    glColor3f(0.5f, 0.4f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 3.0f, 0);
    drawCube(5.0f, 0.4f, 1.0f, textures[1], 0.2f);
    glPopMatrix();
    glPopMatrix();
}

void drawGapuraKedua() {
    glPushMatrix();
    glTranslatef(0, 0, GAPURA_KEDUA_Z);
    glColor3f(0.5f, 0.4f, 0.3f);
    drawCube(4.0f, 0.4f, 2.0f, textures[7], 0.2f);
    for (int side = -1; side <= 1; side += 2) {
        glPushMatrix();
        glTranslatef(side * 1.8f, 0.7f, 0);
        glColor3f(0.55f, 0.45f, 0.35f);
        drawCube(1.0f, 1.4f, 1.8f, textures[1], 0.2f);
        glPopMatrix();
    }
    glColor3f(0.5f, 0.38f, 0.28f);
    glPushMatrix();
    glTranslatef(0, 1.8f, 0);
    drawCone(2.5f, 1.5f, 6);
    glPopMatrix();
    glColor3f(0.45f, 0.35f, 0.25f);
    glPushMatrix();
    glTranslatef(0, 2.8f, 0);
    drawCone(1.8f, 1.0f, 6);
    glPopMatrix();
    glColor3f(0.6f, 0.5f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 3.6f, 0);
    drawSphere(0.25f, 8, 8);
    glPopMatrix();

    // Pintu
    glColor3f(0.35f, 0.25f, 0.15f);
    glPushMatrix();
    glTranslatef(-0.05f, 0.7f, 1.0f);
    glRotatef(-pintuGapuraAngle, 0, 1, 0);
    glTranslatef(-0.75f, 0, 0);
    drawCube(1.5f, 1.3f, 0.08f, textures[2], 0.3f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.05f, 0.7f, 1.0f);
    glRotatef(pintuGapuraAngle, 0, 1, 0);
    glTranslatef(0.75f, 0, 0);
    drawCube(1.5f, 1.3f, 0.08f, textures[2], 0.3f);
    glPopMatrix();
    glPopMatrix();
}

void drawTanggaMasuk() {
    glPushMatrix();
    glTranslatef(0, 0, -32);
    for (int step = 0; step < 5; step++) {
        glPushMatrix();
        glTranslatef(0, step * 0.15f, -step * 0.3f);
        glColor3f(0.5f + step * 0.02f, 0.4f + step * 0.02f, 0.3f);
        drawCube(4.0f - step * 0.1f, 0.15f, 0.6f - step * 0.03f, textures[7], 0.2f);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawPagarDepan() {
    glPushMatrix();
    glTranslatef(0, 0, -30);
    for (int side = -1; side <= 1; side += 2) {
        glPushMatrix();
        glTranslatef(side * 5.5f, 0, 0);
        glColor3f(0.45f, 0.4f, 0.35f);
        for (int i = -3; i <= 3; i++) {
            glPushMatrix();
            glTranslatef(0, 0.4f, i * 1.0f);
            drawCube(0.3f, 0.8f, 0.3f, textures[2], 0.3f);
            glPopMatrix();
        }
        glPushMatrix();
        glTranslatef(0, 0.3f, 0);
        drawCube(0.2f, 0.1f, 6.0f, textures[2], 0.3f);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, 0.7f, 0);
        drawCube(0.2f, 0.1f, 6.0f, textures[2], 0.3f);
        glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawPelataranDepan() {
    glPushMatrix();
    glTranslatef(0, -0.1f, -25);
    glColor3f(0.6f, 0.55f, 0.45f);
    drawCube(14.0f, 0.05f, 8.0f, textures[7], 0.1f);
    glPopMatrix();
}

void drawJalanSetapak() {
    glPushMatrix();
    glTranslatef(0, 0.05f, -18);
    for (int i = -6; i <= 6; i++) {
        glPushMatrix();
        glTranslatef(0, 0, i * 0.8f);
        glColor3f(0.5f + (i % 2) * 0.05f, 0.45f + (i % 2) * 0.05f, 0.35f);
        drawCube(0.8f, 0.1f, 0.6f, textures[7], 0.2f);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawPagarKeliling() {
    float fencePositions[][2] = {{-9, 0}, {9, 0}, {0, -10}, {0, 10}};
    for (int f = 0; f < 4; f++) {
        glPushMatrix();
        glTranslatef(fencePositions[f][0], 0, fencePositions[f][1]);
        bool isX = (f < 2);
        float len = isX ? 20.0f : 18.0f;
        glColor3f(0.45f, 0.4f, 0.35f);
        int numPosts = (int)(len / 1.2f);
        for (int i = 0; i < numPosts; i++) {
            glPushMatrix();
            float pos = -len/2 + i * 1.2f;
            if (isX) glTranslatef(pos, 0.4f, 0);
            else glTranslatef(0, 0.4f, pos);
            drawCube(0.2f, 0.8f, 0.2f, textures[2], 0.3f);
            glPopMatrix();
        }
        glPushMatrix();
        if (isX) {
            glTranslatef(0, 0.3f, 0);
            drawCube(len, 0.08f, 0.15f, textures[2], 0.3f);
        } else {
            glTranslatef(0, 0.3f, 0);
            drawCube(0.15f, 0.08f, len, textures[2], 0.3f);
        }
        glPopMatrix();
        glPushMatrix();
        if (isX) {
            glTranslatef(0, 0.7f, 0);
            drawCube(len, 0.08f, 0.15f, textures[2], 0.3f);
        } else {
            glTranslatef(0, 0.7f, 0);
            drawCube(0.15f, 0.08f, len, textures[2], 0.3f);
        }
        glPopMatrix();
        glPopMatrix();
    }
}

// Lampu taman dengan pulsing
void drawLampuTaman(float x, float z, float seed) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glColor3f(0.35f, 0.3f, 0.25f);
    drawCylinder(0.08f, 1.8f, 8);
    glColor3f(0.4f, 0.35f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 1.9f, 0);
    drawCylinder(0.2f, 0.15f, 8);
    glPopMatrix();

    bool lampuAktif = lampuMenyala && (dayFactor < 0.5f);
    float pulse = lampuAktif ? (0.6f + 0.4f * (0.5f + 0.5f * sinf(animTime * 3.0f + seed))) : 0.15f;
    glDisable(GL_LIGHTING);
    glColor3f(1.0f * pulse, 0.6f * pulse, 0.1f * pulse);
    glPushMatrix();
    glTranslatef(0, 2.1f, 0);
    drawSphere(0.15f, 8, 8);
    glPopMatrix();
    glColor3f(1.0f * pulse, 0.8f * pulse, 0.3f * pulse);
    glPushMatrix();
    glTranslatef(0, 2.15f, 0);
    drawSphere(0.08f + 0.02f * pulse, 6, 6);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawSemuaLampu() {
    float positions[][2] = {{-7,-7},{7,-7},{-7,7},{7,7},{-5,-5},{5,-5},{-5,5},{5,5}};
    for (int i=0; i<8; i++) drawLampuTaman(positions[i][0], positions[i][1], i * 1.3f);
}

// Pohon beringin dengan sway
void drawPohonBeringin(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);
    float swayAngle = 1.2f * sinf(animTime * 0.9f + x * 0.3f + z * 0.2f);
    glRotatef(swayAngle, 0, 0, 1);
    glColor3f(0.4f, 0.3f, 0.2f);
    drawCylinder(0.3f, 2.0f, 8);
    glColor3f(0.35f, 0.28f, 0.18f);
    for (int i=0; i<5; i++) {
        float angle = i * 72.0f * 3.14159f / 180.0f;
        glPushMatrix();
        glTranslatef(0.25f*cosf(angle), 1.8f+0.2f*sinf(angle*2), 0.25f*sinf(angle));
        float branchSway = 1.8f * sinf(animTime * 1.1f + angle + x);
        glRotatef(30.0f+i*10.0f + branchSway, 0, 0, 1);
        glRotatef(angle*180.0f/3.14159f, 0, 1, 0);
        drawCylinder(0.12f, 1.2f, 6);
        glPopMatrix();
    }
    glColor3f(0.1f, 0.5f, 0.1f);
    for (int i=0; i<8; i++) {
        float theta = i*45.0f*3.14159f/180.0f;
        float phi = 20.0f+30.0f*sinf(i*1.5f);
        float r = 0.8f+0.3f*sinf(i*0.7f);
        glPushMatrix();
        glTranslatef(r*cosf(theta)*cosf(phi*3.14159f/180.0f),
                     2.5f+r*0.5f*sinf(phi*3.14159f/180.0f),
                     r*sinf(theta)*cosf(phi*3.14159f/180.0f));
        drawSphere(0.5f+0.2f*sinf(i*1.3f), 6, 6);
        glPopMatrix();
    }
    glColor3f(0.05f, 0.45f, 0.05f);
    for (int i=0; i<5; i++) {
        float theta = i*72.0f*3.14159f/180.0f;
        glPushMatrix();
        glTranslatef(0.6f*cosf(theta), 3.0f+0.3f*sinf(i*2.0f), 0.6f*sinf(theta));
        drawSphere(0.6f, 6, 6);
        glPopMatrix();
    }
    glPopMatrix();
}

// Pohon kamboja dengan sway
void drawPohonKamboja(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);
    float swayAngle = 0.8f * sinf(animTime * 1.0f + x * 0.5f + z * 0.3f);
    glRotatef(swayAngle, 0, 0, 1);
    glColor3f(0.5f, 0.4f, 0.3f);
    drawCylinder(0.15f, 1.5f, 8);
    glColor3f(0.45f, 0.35f, 0.25f);
    for (int i=0; i<4; i++) {
        float angle = i*90.0f*3.14159f/180.0f + 45.0f;
        glPushMatrix();
        glTranslatef(0.1f*cosf(angle), 1.2f, 0.1f*sinf(angle));
        float branchSway = 1.5f * sinf(animTime * 1.3f + angle + x);
        glRotatef(45.0f+i*10.0f + branchSway, 0, 0, 1);
        glRotatef(angle*180.0f/3.14159f, 0, 1, 0);
        drawCylinder(0.05f, 0.8f, 6);
        glPopMatrix();
    }
    float flowerColors[][3] = {{1.0f,0.85f,0.7f},{1.0f,0.9f,0.8f},{0.9f,0.7f,0.5f}};
    for (int i=0; i<6; i++) {
        float theta = i*60.0f*3.14159f/180.0f;
        float r = 0.3f+0.15f*sinf(i*1.7f);
        glPushMatrix();
        glTranslatef(r*cosf(theta), 1.6f+0.15f*cosf(i*1.3f), r*sinf(theta));
        glColor3fv(flowerColors[i%3]);
        drawSphere(0.08f+0.03f*sinf(i*2.0f), 6, 6);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawGroundTiled(float size, unsigned int textureID, float repeat,
                      float y, float r, float g, float b) {
    glColor3f(r, g, b);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    float h = size / 2.0f;
    float matAmbient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
    float matDiffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
    float matSpecular[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    float matShininess[] = { 8.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(-h, y, -h);
    glTexCoord2f(repeat, 0.0f);     glVertex3f( h, y, -h);
    glTexCoord2f(repeat, repeat);   glVertex3f( h, y,  h);
    glTexCoord2f(0.0f, repeat);     glVertex3f(-h, y,  h);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawRumput() {
    glPushMatrix();
    glTranslatef(0, -0.05f, 0);
    glColor3f(0.2f, 0.5f, 0.15f);
    drawCube(26.0f, 0.05f, 26.0f, textures[4], 0.1f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -0.05f, 0);
    glColor3f(0.15f, 0.45f, 0.1f);
    drawCube(20.0f, 0.03f, 20.0f, textures[4], 0.1f);
    glPopMatrix();
    drawGroundTiled(260.0f, textures[4], 40.0f, -0.09f, 0.22f, 0.42f, 0.16f);
}

void drawBangkuBatu(float x, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef(rot, 0, 1, 0);
    glColor3f(0.5f, 0.45f, 0.38f);
    drawCube(1.2f, 0.15f, 0.4f, textures[1], 0.2f);
    glColor3f(0.45f, 0.4f, 0.35f);
    glPushMatrix();
    glTranslatef(-0.45f, -0.2f, 0);
    drawCube(0.15f, 0.4f, 0.3f, textures[1], 0.2f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.45f, -0.2f, 0);
    drawCube(0.15f, 0.4f, 0.3f, textures[1], 0.2f);
    glPopMatrix();
    glColor3f(0.5f, 0.45f, 0.38f);
    glPushMatrix();
    glTranslatef(0, 0.3f, -0.25f);
    drawCube(1.0f, 0.2f, 0.08f, textures[1], 0.2f);
    glPopMatrix();
    glPopMatrix();
}

void drawCandiPerwara(float x, float z, float scale, int index) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);
    glColor3f(0.5f, 0.42f, 0.32f);
    drawCube(2.0f, 0.3f, 2.0f, textures[1], 0.2f);
    glColor3f(0.55f, 0.45f, 0.35f);
    glPushMatrix();
    glTranslatef(0, 0.5f, 0);
    drawCube(1.6f, 1.0f, 1.6f, textures[1], 0.2f);
    glPopMatrix();
    glColor3f(0.5f, 0.4f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 1.2f, 0);
    drawCube(1.2f, 0.6f, 1.2f, textures[1], 0.2f);
    glPopMatrix();
    glColor3f(0.45f, 0.35f, 0.25f);
    glPushMatrix();
    glTranslatef(0, 1.7f, 0);
    drawCone(1.0f, 0.8f, 6);
    glPopMatrix();
    glColor3f(0.4f, 0.3f, 0.2f);
    glPushMatrix();
    glTranslatef(0, 2.2f, 0);
    drawCone(0.7f, 0.6f, 6);
    glPopMatrix();
    glColor3f(0.6f, 0.5f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 2.7f, 0);
    drawSphere(0.15f, 8, 8);
    glPopMatrix();
    for (int side = 0; side < 4; side++) {
        float angle = side * 90.0f * 3.14159f / 180.0f;
        glPushMatrix();
        glTranslatef(0.9f * cosf(angle), 0.7f, 0.9f * sinf(angle));
        glColor3f(0.4f, 0.35f, 0.3f);
        drawCube(0.2f, 0.3f, 0.1f, textures[7], 0.2f);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawBadanCandiInduk() {
    glColor3f(0.5f, 0.42f, 0.32f);
    drawCube(5.0f, 0.5f, 5.0f, textures[1], 0.2f);
    glColor3f(0.55f, 0.45f, 0.35f);
    glPushMatrix();
    glTranslatef(0, 0.6f, 0);
    drawCube(4.2f, 1.0f, 4.2f, textures[1], 0.2f);
    glPopMatrix();
    glColor3f(0.52f, 0.42f, 0.32f);
    glPushMatrix();
    glTranslatef(0, 1.3f, 0);
    drawCube(3.4f, 0.8f, 3.4f, textures[1], 0.2f);
    glPopMatrix();
    glColor3f(0.48f, 0.38f, 0.28f);
    glPushMatrix();
    glTranslatef(0, 1.9f, 0);
    drawCube(2.6f, 0.7f, 2.6f, textures[1], 0.2f);
    glPopMatrix();
    glColor3f(0.5f, 0.4f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 2.5f, 0);
    drawCube(1.8f, 0.8f, 1.8f, textures[1], 0.2f);
    glPopMatrix();
    glColor3f(0.4f, 0.35f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 2.3f, 2.6f);
    drawCube(0.8f, 0.6f, 0.05f, textures[1], 0.3f);
    glPopMatrix();
    for (int side = 0; side < 4; side++) {
        float angle = side * 90.0f * 3.14159f / 180.0f;
        for (int row = 0; row < 3; row++) {
            glPushMatrix();
            float r = 2.4f;
            float y = 0.8f + row * 0.4f;
            glTranslatef(r * cosf(angle), y, r * sinf(angle));
            glColor3f(0.4f + row * 0.03f, 0.35f + row * 0.03f, 0.3f + row * 0.02f);
            drawCube(0.8f, 0.15f, 0.05f, textures[7], 0.2f);
            glPopMatrix();
        }
    }
}

void drawAtapCandiInduk() {
    float roofColors[][3] = {{0.45f,0.35f,0.25f},{0.4f,0.3f,0.2f},{0.35f,0.25f,0.18f}};
    for (int tier = 0; tier < 3; tier++) {
        float baseW = 3.0f - tier * 0.8f;
        float height = 0.7f - tier * 0.1f;
        float yPos = 3.2f + tier * 0.6f;
        glColor3fv(roofColors[tier]);
        glPushMatrix();
        glTranslatef(0, yPos, 0);
        drawCone(baseW / 1.5f, height, 8);
        glPopMatrix();
        glColor3f(0.5f, 0.4f, 0.3f);
        glPushMatrix();
        glTranslatef(0, yPos - 0.1f, 0);
        drawCube(baseW, 0.1f, baseW, textures[3], 0.2f);
        glPopMatrix();
    }
}

void drawTanggaCandiInduk() {
    glPushMatrix();
    glTranslatef(0, 0, 2.8f);
    for (int step = 0; step < 6; step++) {
        glPushMatrix();
        glTranslatef(0, step * 0.15f, -step * 0.25f);
        glColor3f(0.5f + step * 0.02f, 0.42f + step * 0.02f, 0.32f);
        drawCube(3.0f - step * 0.08f, 0.15f, 0.5f, textures[7], 0.2f);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawStupa() {
    glPushMatrix();
    glTranslatef(0, 5.6f, 0);
    glColor3f(0.5f, 0.4f, 0.3f);
    drawCylinder(0.6f, 0.2f, 12);
    glColor3f(0.55f, 0.45f, 0.35f);
    glPushMatrix();
    glTranslatef(0, 0.2f, 0);
    drawSphere(0.5f, 12, 8);
    glPopMatrix();
    glColor3f(0.6f, 0.5f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 0.7f, 0);
    drawCone(0.2f, 0.3f, 8);
    glPopMatrix();
    glColor3f(0.65f, 0.55f, 0.35f);
    glPushMatrix();
    glTranslatef(0, 0.9f, 0);
    drawSphere(0.1f, 8, 8);
    glPopMatrix();
    glPopMatrix();
}

void drawArcaBilik() {
    glPushMatrix();
    glTranslatef(0, 2.6f, 0);
    glColor3f(0.6f, 0.5f, 0.4f);
    glPushMatrix();
    glTranslatef(0, 0.4f, 0);
    drawSphere(0.15f, 8, 8);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0.1f, 0);
    drawCylinder(0.12f, 0.3f, 8);
    glPopMatrix();
    for (int arm = -1; arm <= 1; arm += 2) {
        glPushMatrix();
        glTranslatef(arm * 0.15f, 0.2f, 0);
        glRotatef(arm * 30.0f, 0, 0, 1);
        drawCylinder(0.04f, 0.15f, 6);
        glPopMatrix();
    }
    glColor3f(0.4f, 0.35f, 0.3f);
    glPushMatrix();
    glTranslatef(0, -0.05f, 0);
    drawCube(0.3f, 0.05f, 0.3f, textures[7], 0.2f);
    glPopMatrix();
    glPopMatrix();
}

void drawKolam() {
    glPushMatrix();
    glTranslatef(-3.5f, 0, -5.0f);
    glColor3f(0.3f, 0.35f, 0.4f);
    drawCube(3.0f, 0.1f, 2.0f, textures[7], 0.2f);
    float shimmer = 0.15f * sinf(animTime * 2.0f);
    glColor4f(0.2f + shimmer, 0.4f + shimmer, 0.6f + shimmer, 0.75f);
    glPushMatrix();
    glTranslatef(0, 0.1f, 0);
    drawCube(2.6f, 0.03f, 1.6f, textures[5], 0.4f + 0.2f*fabsf(shimmer));
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < 3; i++) {
        float t = fmodf(animTime * 0.35f + i * 0.33f, 1.0f);
        float radius = t * 1.2f;
        float alpha = (1.0f - t) * 0.5f;
        glColor4f(0.9f, 0.95f, 1.0f, alpha);
        glPushMatrix();
        glTranslatef(0, 0.13f, 0);
        glBegin(GL_LINE_LOOP);
        for (int a = 0; a < 24; a++) {
            float ang = a * 2.0f * 3.14159f / 24.0f;
            glVertex3f(radius * cosf(ang) * 1.2f, 0, radius * sinf(ang) * 0.7f);
        }
        glEnd();
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glColor3f(0.45f, 0.4f, 0.35f);
    glPushMatrix();
    glTranslatef(-1.5f, 0.08f, 0);
    drawCube(0.15f, 0.15f, 2.0f, textures[1], 0.2f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.5f, 0.08f, 0);
    drawCube(0.15f, 0.15f, 2.0f, textures[1], 0.2f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0.08f, -1.0f);
    drawCube(3.0f, 0.15f, 0.15f, textures[1], 0.2f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0.08f, 1.0f);
    drawCube(3.0f, 0.15f, 0.15f, textures[1], 0.2f);
    glPopMatrix();
    glPopMatrix();
}

void drawBatuYoniLingga(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glColor3f(0.45f, 0.4f, 0.35f);
    drawCube(0.8f, 0.15f, 0.8f, textures[1], 0.2f);
    glColor3f(0.5f, 0.42f, 0.35f);
    glPushMatrix();
    glTranslatef(0, 0.2f, 0);
    drawCylinder(0.15f, 0.6f, 8);
    glPopMatrix();
    glColor3f(0.55f, 0.45f, 0.38f);
    glPushMatrix();
    glTranslatef(0, 0.55f, 0);
    drawSphere(0.1f, 8, 8);
    glPopMatrix();
    glPopMatrix();
}

void drawPapanInformasi(float x, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef(rot, 0, 1, 0);
    glColor3f(0.4f, 0.35f, 0.3f);
    drawCylinder(0.05f, 1.2f, 8);
    glColor3f(0.5f, 0.4f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 1.2f, 0);
    drawCube(0.8f, 0.5f, 0.05f, textures[2], 0.3f);
    glPopMatrix();
    glColor3f(0.35f, 0.3f, 0.25f);
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.38f, 1.2f, 0.03f);
        drawCube(0.04f, 0.5f, 0.04f, textures[7], 0.2f);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, 1.2f + i * 0.22f, 0.03f);
        drawCube(0.8f, 0.04f, 0.04f, textures[7], 0.2f);
        glPopMatrix();
    }
    glPopMatrix();
}

// ============================================================================
// FUNGSI WARNA LANGIT & PENCAHAYAAN (SENJA)
// ============================================================================

void getSkyColor(float df,
                 float& skyR, float& skyG, float& skyB,
                 float& ambR, float& ambG, float& ambB,
                 float& diffR, float& diffG, float& diffB) {
    // df: 0..1, 1=siang, 0=malam
    float r, g, b;
    if (df >= 0.80f) {
        // SIANG
        r = 0.50f; g = 0.75f; b = 1.00f;
        ambR = 0.35f; ambG = 0.35f; ambB = 0.35f;
        diffR = 1.00f; diffG = 1.00f; diffB = 0.95f;
    } else if (df >= 0.30f) {
        // SENJA (oranye)
        float t = (df - 0.30f) / 0.50f; // 0..1
        // Warna langit: transisi dari oranye (0.30) ke biru (0.80)
        float startR = 1.00f, startG = 0.48f, startB = 0.18f;
        float endR   = 0.50f, endG   = 0.75f, endB   = 1.00f;
        r = startR + (endR - startR) * t;
        g = startG + (endG - startG) * t;
        b = startB + (endB - startB) * t;
        // Pencahayaan senja
        ambR = 0.28f + 0.07f * t;
        ambG = 0.18f + 0.17f * t;
        ambB = 0.12f + 0.23f * t;
        diffR = 1.00f;
        diffG = 0.60f + 0.40f * t;
        diffB = 0.28f + 0.67f * t;
    } else {
        // MALAM
        r = 0.03f; g = 0.05f; b = 0.12f;
        ambR = 0.10f; ambG = 0.10f; ambB = 0.14f;
        diffR = 0.22f; diffG = 0.24f; diffB = 0.35f;
    }
    skyR = r; skyG = g; skyB = b;
}

// ============================================================================
// SKYBOX (menggunakan getSkyColor)
// ============================================================================

void drawSkybox() {
    glPushMatrix();
    glTranslatef(0, 15.0f, 0);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    float r, g, b, ar, ag, ab, dr, dg, db;
    getSkyColor(dayFactor, r, g, b, ar, ag, ab, dr, dg, db);

    glColor3f(r, g, b);
    glPushMatrix();
    glScalef(80.0f, 60.0f, 80.0f);
    drawSphere(1.0f, 32, 32);
    glPopMatrix();

    // Matahari
    if (dayFactor > 0.1f) {
        float sunX = 35.0f * cosf(sunAngle);
        float sunY = 20.0f * sinf(sunAngle) + 5.0f;
        float sunZ = 20.0f * sinf(sunAngle * 0.7f);
        glPushMatrix();
        glTranslatef(sunX, sunY, sunZ);
        // Warna matahari berubah: putih saat siang, oranye saat senja
        float sunR = 1.0f;
        float sunG = 0.9f - 0.4f * (1.0f - dayFactor);
        float sunB = 0.8f - 0.6f * (1.0f - dayFactor);
        glColor3f(sunR, sunG, sunB);
        drawSphere(1.2f, 16, 16);
        glColor4f(sunR, sunG, sunB * 0.5f, 0.2f);
        glPushMatrix();
        glScalef(3.0f, 3.0f, 3.0f);
        drawSphere(1.0f, 16, 16);
        glPopMatrix();
        glPopMatrix();
    }

    // Bulan
    if (dayFactor < 0.5f) {
        float moonX = 35.0f * cosf(moonAngle);
        float moonY = 15.0f * sinf(moonAngle) + 5.0f;
        float moonZ = 20.0f * sinf(moonAngle * 0.7f);
        glPushMatrix();
        glTranslatef(moonX, moonY, moonZ);
        glColor3f(0.9f, 0.9f, 1.0f);
        drawSphere(0.8f, 16, 16);
        glColor4f(0.8f, 0.8f, 1.0f, 0.15f);
        glPushMatrix();
        glScalef(2.5f, 2.5f, 2.5f);
        drawSphere(1.0f, 16, 16);
        glPopMatrix();
        glPopMatrix();
    }

    // Bintang
    if (dayFactor < 0.4f) {
        float bright = 1.0f - dayFactor * 2.5f;
        bright = std::min(1.0f, bright);
        glPointSize(1.8f);
        glBegin(GL_POINTS);
        for (const auto& s : stars) {
            float twinkle = 0.5f + 0.5f * sinf(animTime * 0.5f + s.x * 0.1f + s.y * 0.2f);
            glColor4f(1.0f, 1.0f, 1.0f, bright * s.brightness * twinkle);
            glVertex3f(s.x, s.y, s.z);
        }
        glEnd();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// ===== AWAN BERGERAK =====
void drawAwan(float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);
    glDisable(GL_LIGHTING);
    glColor4f(0.95f, 0.95f, 0.98f, 0.8f);
    float positions[][3] = {{0,0,0},{1.0f,0.1f,0.3f},{-1.0f,-0.05f,-0.2f},
                            {1.5f,0.15f,-0.3f},{-1.5f,0.05f,0.4f},
                            {0.5f,0.2f,0.8f},{-0.5f,-0.1f,-0.7f}};
    for (int i=0; i<7; i++) {
        glPushMatrix();
        glTranslatef(positions[i][0], positions[i][1], positions[i][2]);
        glScalef(1.0f, 0.3f, 0.6f);
        drawSphere(0.6f+0.2f*sinf(i*0.7f), 8, 6);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawAwanBergerak(float baseX, float y, float z, float scale, float speed) {
    float amplitude = 10.0f;
    float x = baseX + amplitude * sinf(animTime * speed);
    drawAwan(x, y, z, scale);
}

// ===== ANIMASI API =====
void drawApiObor(float x, float y, float z, float seed) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.35f, 0.3f, 0.25f);
    drawCylinder(0.06f, 0.6f, 8);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float flicker = 0.6f + 0.4f * sinf(animTime * 14.0f + seed) * cosf(animTime * 9.0f + seed * 2.0f);
    glColor4f(1.0f, 0.55f + 0.15f*flicker, 0.1f, 0.85f);
    glPushMatrix();
    glTranslatef(0, 0.65f + 0.05f*flicker, 0);
    glScalef(0.5f + 0.15f*flicker, 1.0f + 0.4f*flicker, 0.5f + 0.15f*flicker);
    drawSphere(0.16f, 8, 8);
    glPopMatrix();
    glColor4f(1.0f, 0.85f, 0.3f, 0.8f);
    glPushMatrix();
    glTranslatef(0, 0.75f + 0.08f*flicker, 0);
    glScalef(0.3f, 0.7f + 0.3f*flicker, 0.3f);
    drawSphere(0.1f, 6, 6);
    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawSemuaOborTangga() {
    drawApiObor(-2.3f, 0.75f, -31.5f, 0.0f);
    drawApiObor( 2.3f, 0.75f, -31.5f, 1.7f);
    drawApiObor(-1.7f, 0.75f, 7.8f, 3.1f);
    drawApiObor( 1.7f, 0.75f, 7.8f, 4.6f);
}

// ===== ASAP DUPA =====
void drawAsapDupa(float x, float y, float z) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < 6; i++) {
        float t = fmodf(animTime * 0.35f + i * 0.17f, 1.0f);
        float px = x + 0.1f * sinf(t * 6.28f + i);
        float py = y + t * 0.7f;
        float pz = z + 0.1f * cosf(t * 6.28f + i);
        float alpha = (1.0f - t) * 0.3f;
        float size = 0.06f + t * 0.14f;
        glPushMatrix();
        glTranslatef(px, py, pz);
        glColor4f(0.85f, 0.85f, 0.85f, alpha);
        drawSphere(size, 6, 6);
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// ===== BURUNG =====
void drawBurung(float radius, float height, float speed, float phase, float centerZ) {
    glDisable(GL_LIGHTING);
    float angle = animTime * speed + phase;
    float x = radius * cosf(angle);
    float z = centerZ + radius * sinf(angle);
    float y = height + 0.5f * sinf(animTime * 2.0f + phase);
    float wingFlap = sinf(animTime * 12.0f + phase);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-(angle * 180.0f / 3.14159f) - 90.0f, 0, 1, 0);
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, 0, 0); glVertex3f(-0.35f, wingFlap * 0.15f, -0.12f); glVertex3f(-0.05f, 0, -0.05f);
    glVertex3f(0, 0, 0); glVertex3f( 0.35f, wingFlap * 0.15f, -0.12f); glVertex3f( 0.05f, 0, -0.05f);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawSemuaBurung() {
    drawBurung(10.0f, 9.0f, 0.5f, 0.0f, 0.0f);
    drawBurung(8.0f, 10.5f, -0.4f, 2.1f, 0.0f);
    drawBurung(12.0f, 8.0f, 0.3f, 4.2f, 2.0f);
    drawBurung(6.0f, 11.0f, -0.6f, 1.0f, -3.0f);
}

// ===== KELOPAK BUNGA =====
void drawKelopakJatuh(float baseX, float baseZ, float seedOffset) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < 4; i++) {
        float t = fmodf(animTime * 0.12f + i * 0.25f + seedOffset, 1.0f);
        float px = baseX + 0.5f * sinf(t * 6.28f + i) + 0.2f * sinf(animTime * 2.0f + i);
        float py = 2.1f - t * 2.1f;
        float pz = baseZ + 0.5f * cosf(t * 6.28f + i);
        glPushMatrix();
        glTranslatef(px, py, pz);
        glRotatef(animTime * 90.0f + i * 40.0f, 0, 1, 0);
        glColor4f(1.0f, 0.9f, 0.8f, 1.0f - t * 0.6f);
        drawQuad(-0.05f, 0, 0,  0.05f, 0, 0,  0.05f, 0.09f, 0,  -0.05f, 0.09f, 0);
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawSemuaKelopakJatuh() {
    drawKelopakJatuh(-3.0f, -7.0f, 0.0f);
    drawKelopakJatuh(3.0f, -7.0f, 0.4f);
    drawKelopakJatuh(-3.5f, 7.0f, 0.7f);
    drawKelopakJatuh(3.5f, 7.0f, 0.2f);
}

// ===== KUNANG-KUNANG =====
void drawKunangKunang(float baseX, float baseZ, float seed) {
    float t = animTime * 0.6f + seed;
    float x = baseX + 1.2f * sinf(t) + 0.3f * sinf(t * 3.0f);
    float y = 0.4f + 0.3f * sinf(t * 2.0f + seed);
    float z = baseZ + 1.2f * cosf(t * 1.3f);
    float glow = 0.4f + 0.6f * (0.5f + 0.5f * sinf(animTime * 8.0f + seed));
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor4f(1.0f, 1.0f, 0.4f, glow);
    drawSphere(0.035f, 6, 6);
    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawSemuaKunangKunang() {
    if (dayFactor > 0.4f) return;
    float basePos[][2] = {{-6,-6},{6,-6},{-6,6},{6,6},{-3,0},{3,0},{0,-3},{0,3}};
    for (int i = 0; i < 8; i++)
        drawKunangKunang(basePos[i][0], basePos[i][1], i * 1.9f);
}

// ============================================================================
// PEZIARAH (DENGAN VARIASI WARNA) - VERSI LEBIH JELAS
// ============================================================================

void drawPeziarah(float radius, float speed, float phase, float centerZ, float colorSeed = 0.0f) {
    float angle = animTime * speed + phase;
    float x = radius * cosf(angle);
    float z = centerZ + radius * sinf(angle);
    float walkBob = fabsf(sinf(animTime * 8.0f + phase)) * 0.04f;
    float legSwing = sinf(animTime * 8.0f + phase) * 25.0f;
    float armSwing = sinf(animTime * 8.0f + phase + 3.14159f) * 25.0f; // lengan berlawanan

    // Warna pakaian (bervariasi)
    float r = 0.3f + 0.5f * sinf(colorSeed);
    float g = 0.2f + 0.5f * sinf(colorSeed + 1.5f);
    float b = 0.1f + 0.5f * sinf(colorSeed + 3.0f);
    // Warna kulit
    float skinR = 0.8f, skinG = 0.6f, skinB = 0.4f;

    glPushMatrix();
    glTranslatef(x, walkBob, z);
    glRotatef(-(angle * 180.0f / 3.14159f) + 90.0f, 0, 1, 0);

    // ===== BADAN =====
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(0, 0.9f, 0);
    glScalef(0.6f, 0.9f, 0.4f);
    drawSphere(0.25f, 10, 10);
    glPopMatrix();

    // ===== KEPALA =====
    glColor3f(skinR, skinG, skinB);
    glPushMatrix();
    glTranslatef(0, 1.3f, 0);
    drawSphere(0.15f, 12, 12);
    glPopMatrix();

    // ===== LENGAN KIRI =====
    glColor3f(skinR * 0.9f, skinG * 0.9f, skinB * 0.9f);
    glPushMatrix();
    glTranslatef(-0.3f, 0.9f, 0);
    glRotatef(armSwing, 1, 0, 0);
    glRotatef(15.0f, 0, 0, 1);
    glPushMatrix();
    glTranslatef(0, -0.15f, 0);
    drawCylinder(0.04f, 0.3f, 6);
    glPopMatrix();
    glColor3f(skinR, skinG, skinB);
    glPushMatrix();
    glTranslatef(0, -0.35f, 0);
    drawSphere(0.04f, 6, 6);
    glPopMatrix();
    glPopMatrix();

    // ===== LENGAN KANAN =====
    glColor3f(skinR * 0.9f, skinG * 0.9f, skinB * 0.9f);
    glPushMatrix();
    glTranslatef(0.3f, 0.9f, 0);
    glRotatef(-armSwing, 1, 0, 0);
    glRotatef(-15.0f, 0, 0, 1);
    glPushMatrix();
    glTranslatef(0, -0.15f, 0);
    drawCylinder(0.04f, 0.3f, 6);
    glPopMatrix();
    glColor3f(skinR, skinG, skinB);
    glPushMatrix();
    glTranslatef(0, -0.35f, 0);
    drawSphere(0.04f, 6, 6);
    glPopMatrix();
    glPopMatrix();

    // ===== KAKI KIRI =====
    glColor3f(0.2f, 0.2f, 0.2f); // celana hitam
    glPushMatrix();
    glTranslatef(-0.12f, 0.3f, 0);
    glRotatef(legSwing, 1, 0, 0);
    drawCylinder(0.06f, 0.3f, 6);
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0, -0.3f, 0.05f);
    glScalef(1.0f, 0.3f, 1.5f);
    drawSphere(0.05f, 6, 6);
    glPopMatrix();
    glPopMatrix();

    // ===== KAKI KANAN =====
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.12f, 0.3f, 0);
    glRotatef(-legSwing, 1, 0, 0);
    drawCylinder(0.06f, 0.3f, 6);
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0, -0.3f, 0.05f);
    glScalef(1.0f, 0.3f, 1.5f);
    drawSphere(0.05f, 6, 6);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void drawSemuaPeziarah() {
    // ==== PEZIARAH DI SEKITAR CANDI INDUK (5 orang) ====
    drawPeziarah(4.0f, 0.35f, 0.0f, 5.0f, 0.0f);
    drawPeziarah(5.5f, -0.25f, 3.0f, 5.0f, 1.2f);
    drawPeziarah(6.0f, 0.20f, 1.5f, 5.0f, 2.5f);
    drawPeziarah(3.5f, -0.30f, 4.5f, 5.0f, 3.8f);
    drawPeziarah(5.0f, 0.15f, 2.2f, 5.0f, 5.0f);

    // ==== PEZIARAH DI PELATARAN DEPAN (3 orang) ====
    drawPeziarah(3.0f, 0.15f, 2.0f, -22.0f, 0.7f);
    drawPeziarah(4.0f, -0.20f, 0.7f, -22.0f, 1.9f);
    drawPeziarah(5.0f, 0.10f, 3.5f, -22.0f, 3.1f);

    // ==== PEZIARAH DI SEKITAR GAPURA KEDUA (2 orang) ====
    drawPeziarah(2.0f, 0.10f, 4.0f, -12.0f, 4.3f);
    drawPeziarah(3.0f, -0.15f, 1.2f, -12.0f, 5.6f);

    // ==== PEZIARAH DI GAPURA UTAMA (2 orang) ====
    drawPeziarah(2.5f, 0.12f, 3.5f, -28.0f, 0.3f);
    drawPeziarah(3.0f, -0.18f, 0.5f, -28.0f, 1.4f);
}

// ===== LONCENG =====
void drawLoncengGoyang(float x, float y, float z) {
    float swing = sinf(animTime * 2.0f) * 15.0f;
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.4f, 0.35f, 0.25f);
    drawCube(0.05f, 0.25f, 0.05f, textures[2], 0.2f);
    glPushMatrix();
    glTranslatef(0, -0.15f, 0);
    glRotatef(swing, 0, 0, 1);
    glColor3f(0.6f, 0.5f, 0.3f);
    drawCone(0.12f, 0.2f, 10);
    glColor3f(0.65f, 0.55f, 0.35f);
    glPushMatrix();
    glTranslatef(0, -0.15f, 0);
    drawSphere(0.03f, 6, 6);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

// ===== BATU TAMBAHAN =====
void drawBatuTambahan(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);
    glColor3f(0.45f+0.1f*sinf(x), 0.4f+0.1f*cosf(z), 0.35f);
    glRotatef(x*15.0f, 0, 1, 0);
    glRotatef(z*10.0f, 1, 0, 0);
    drawCube(0.3f+0.1f*sinf(x+z), 0.15f+0.1f*cosf(x-z), 0.25f+0.1f*sinf(z-x), textures[1], 0.2f);
    glPopMatrix();
}

void drawPelataranBelakang() {
    glPushMatrix();
    glTranslatef(0, -0.05f, 12);
    glColor3f(0.5f, 0.45f, 0.4f);
    drawCube(12.0f, 0.05f, 8.0f, textures[7], 0.1f);
    glPopMatrix();
}

void drawPagarDalam() {
    float positions[][2] = {{-3.5f,0},{3.5f,0},{0,-3.5f},{0,3.5f}};
    for (int f=0; f<4; f++) {
        glPushMatrix();
        glTranslatef(positions[f][0], 0, positions[f][1]);
        bool isX = (f < 2);
        float len = 7.0f;
        glColor3f(0.45f, 0.4f, 0.35f);
        int numPosts = (int)(len / 0.6f);
        for (int i=0; i<numPosts; i++) {
            glPushMatrix();
            float pos = -len/2 + i*0.6f;
            if (isX) glTranslatef(pos, 0.2f, 0);
            else glTranslatef(0, 0.2f, pos);
            drawCube(0.2f, 0.4f, 0.2f, textures[1], 0.2f);
            glPopMatrix();
        }
        glPopMatrix();
    }
}

// ============================================================================
// COLLISION
// ============================================================================

struct AABB { float minX, maxX, minZ, maxZ; };
std::vector<AABB> collisionBoxes = {
    {-6.5f, 6.5f, 3.5f, 8.0f},
    {-2.0f, 2.0f, -33.0f, -27.0f},
    {-3.0f, 3.0f, -13.0f, -11.0f},
    {-9.3f, -8.7f, -10.0f, 10.0f},
    { 8.7f,  9.3f, -10.0f, 10.0f},
    {-9.0f,  9.0f, -10.3f, -9.7f},
    {-9.0f,  9.0f,  9.7f, 10.3f},
};

bool checkCollision(float x, float z) {
    for (size_t i = 0; i < collisionBoxes.size(); i++) {
        const AABB& b = collisionBoxes[i];
        if (x > b.minX && x < b.maxX && z > b.minZ && z < b.maxZ) return true;
    }
    return false;
}

void tryMove(float dx, float dz) {
    float newX = camX + dx;
    float newZ = camZ + dz;
    if (!checkCollision(newX, camZ)) camX = newX;
    if (!checkCollision(camX, newZ)) camZ = newZ;
}

// ============================================================================
// UPDATE ANIMASI
// ============================================================================

void initStars() {
    srand(42);
    for (int i = 0; i < 200; i++) {
        Star s;
        float theta = (rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
        float phi = acos(2.0f * (rand() / (float)RAND_MAX) - 1.0f);
        float r = 75.0f;
        s.x = r * sin(phi) * cos(theta);
        s.y = r * cos(phi) * 0.6f + 12.0f;
        s.z = r * sin(phi) * sin(theta);
        s.brightness = 0.5f + 0.5f * (rand() / (float)RAND_MAX);
        stars.push_back(s);
    }
}

void updateAnimation() {
    animTime += 0.016f;

    // Siklus matahari
    sunAngle += 0.0015f;
    if (sunAngle > 2.0f * 3.14159f) sunAngle -= 2.0f * 3.14159f;
    dayFactor = 0.5f + 0.5f * sinf(sunAngle);
    dayFactor = std::max(0.0f, std::min(1.0f, dayFactor));
    moonAngle = sunAngle + 3.14159f;

    // Fog dinamis
    fogStart = 45.0f + 15.0f * (1.0f - dayFactor);
    fogEnd   = 110.0f + 40.0f * dayFactor;

    // Pintu gapura
    float targetAngle = 0.0f;
    if (pintuManual) targetAngle = pintuTargetAngle;
    else if (cameraMode == 1) {
        float dx = camX - 0.0f;
        float dz = camZ - GAPURA_KEDUA_Z;
        float dist = sqrtf(dx * dx + dz * dz);
        if (dist < 6.0f) targetAngle = 75.0f;
    }
    pintuGapuraAngle += (targetAngle - pintuGapuraAngle) * 0.08f;

    // Cinematic
    if (cameraMode == 2) {
        cinematicAngle += 0.15f;
        if (cinematicAngle > 360.0f) cinematicAngle -= 360.0f;
    }
}

// ============================================================================
// RENDER SCENE
// ============================================================================

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- Warna langit & fog (menggunakan getSkyColor) ---
    float skyR, skyG, skyB;
    float ambR, ambG, ambB, diffR, diffG, diffB;
    getSkyColor(dayFactor, skyR, skyG, skyB, ambR, ambG, ambB, diffR, diffG, diffB);

    glClearColor(skyR, skyG, skyB, 1.0f);
    GLfloat fogColor[] = { skyR, skyG, skyB, 1.0f };
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, fogStart);
    glFogf(GL_FOG_END, fogEnd);

    // --- Projection & ModelView ---
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // --- Kamera ---
    if (cameraMode == 1) {
        float lookX = camX + sinf(camYaw * 3.14159f / 180.0f) * cosf(camPitch * 3.14159f / 180.0f);
        float lookY = camY + sinf(camPitch * 3.14159f / 180.0f);
        float lookZ = camZ + cosf(camYaw * 3.14159f / 180.0f) * cosf(camPitch * 3.14159f / 180.0f);
        gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);
    } else if (cameraMode == 2) {
        float angleRad = cinematicAngle * 3.14159f / 180.0f;
        float dist = 45.0f + 5.0f * sinf(angleRad * 0.5f);
        float cx = dist * sinf(angleRad);
        float cz = dist * cosf(angleRad);
        float cy = 18.0f + 8.0f * sinf(angleRad * 0.7f);
        gluLookAt(cx, cy, cz, 0, 2.0f, 0, 0, 1, 0);
    } else {
        float angleRad = cameraAngleY * 3.14159f / 180.0f;
        float angleRadX = cameraAngleX * 3.14159f / 180.0f;
        float cx = cameraDistance * cosf(angleRadX) * sinf(angleRad);
        float cy = cameraDistance * sinf(angleRadX);
        float cz = cameraDistance * cosf(angleRadX) * cosf(angleRad);
        gluLookAt(cx, cy + 5.0f, cz,
                  cameraTargetX, cameraTargetY, cameraTargetZ,
                  0, 1, 0);
    }

    // ===== LIGHTING (menggunakan nilai dari getSkyColor) =====
    float sunX = 35.0f * cosf(sunAngle);
    float sunY = 20.0f * sinf(sunAngle) + 8.0f;
    float sunZ = 20.0f * sinf(sunAngle * 0.7f);
    GLfloat lightPos[] = { sunX, sunY, sunZ, 1.0f };

    GLfloat lightAmbient[]  = { ambR, ambG, ambB, 1.0f };
    GLfloat lightDiffuse[]  = { diffR, diffG, diffB, 1.0f };
    GLfloat lightSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Bulan (LIGHT1)
    if (dayFactor < 0.5f) {
        float moonIntensity = (0.5f - dayFactor) * 0.6f;
        float moonX2 = 35.0f * cosf(moonAngle);
        float moonY2 = 15.0f * sinf(moonAngle) + 5.0f;
        float moonZ2 = 20.0f * sinf(moonAngle * 0.7f);
        GLfloat moonPos[] = { moonX2, moonY2, moonZ2, 1.0f };
        GLfloat moonAmb[]  = {0.03f * moonIntensity, 0.03f * moonIntensity, 0.08f * moonIntensity, 1.0f};
        GLfloat moonDiff[] = {0.05f * moonIntensity, 0.05f * moonIntensity, 0.15f * moonIntensity, 1.0f};
        glLightfv(GL_LIGHT1, GL_POSITION, moonPos);
        glLightfv(GL_LIGHT1, GL_AMBIENT, moonAmb);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, moonDiff);
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }

    // Global ambient
    GLfloat globalAmbient[] = {0.1f + 0.1f*dayFactor, 0.1f + 0.1f*dayFactor, 0.15f + 0.1f*dayFactor, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat matSpecular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat matShininess[] = {20.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // ===== GAMBAR OBJEK =====
    drawSkybox();
    drawAwanBergerak(-15, 16, -10, 1.5f, 0.15f);
    drawAwanBergerak(12, 18, 8, 1.2f, -0.12f);
    drawAwanBergerak(-8, 20, 12, 1.0f, 0.18f);
    drawAwanBergerak(18, 15, -15, 1.3f, -0.2f);
    drawAwanBergerak(0, 22, -20, 0.8f, 0.22f);

    drawRumput();
    drawPelataranDepan();
    drawPelataranBelakang();
    drawJalanSetapak();
    drawPagarDepan();
    drawPagarKeliling();
    drawPagarDalam();

    drawGapuraUtama();
    drawGapuraKedua();
    drawTanggaMasuk();
    drawLoncengGoyang(0, 3.0f, -28.0f);

    drawCandiPerwara(-4.5f, -3.0f, 0.7f, 1);
    drawCandiPerwara(4.5f, -3.0f, 0.7f, 2);
    drawCandiPerwara(-4.5f, 3.0f, 0.7f, 3);
    drawCandiPerwara(4.5f, 3.0f, 0.7f, 4);

    glPushMatrix();
    glTranslatef(0, 0, 5.0f);
    drawBadanCandiInduk();
    drawAtapCandiInduk();
    drawTanggaCandiInduk();
    drawStupa();
    drawArcaBilik();
    drawAsapDupa(0.0f, 2.15f, 3.2f);
    glPopMatrix();

    drawPohonBeringin(-6.0f, -8.0f, 0.8f);
    drawPohonBeringin(6.0f, -8.0f, 0.8f);
    drawPohonBeringin(-7.0f, 8.0f, 0.9f);
    drawPohonBeringin(7.0f, 8.0f, 0.9f);
    drawPohonKamboja(-3.0f, -7.0f, 0.7f);
    drawPohonKamboja(3.0f, -7.0f, 0.7f);
    drawPohonKamboja(-3.5f, 7.0f, 0.7f);
    drawPohonKamboja(3.5f, 7.0f, 0.7f);

    drawSemuaKelopakJatuh();
    drawSemuaLampu();
    drawSemuaOborTangga();

    drawBangkuBatu(-2.5f, -6.0f, 20.0f);
    drawBangkuBatu(2.5f, -6.0f, -20.0f);
    drawBangkuBatu(-2.5f, 6.0f, 160.0f);
    drawBangkuBatu(2.5f, 6.0f, -160.0f);

    drawKolam();
    drawBatuYoniLingga(-2.0f, -4.5f);
    drawBatuYoniLingga(2.0f, -4.5f);
    drawPapanInformasi(-1.5f, -10.0f, 10.0f);
    drawPapanInformasi(1.5f, -10.0f, -10.0f);

    float stonePos[][2] = {{-5.5f,-5.0f},{5.5f,-5.0f},{-5.5f,5.0f},{5.5f,5.0f},
                           {-6.5f,-2.0f},{6.5f,-2.0f},{-6.5f,2.0f},{6.5f,2.0f},
                           {-8.0f,-6.0f},{8.0f,-6.0f},{-8.0f,6.0f},{8.0f,6.0f}};
    for (int i=0; i<12; i++)
        drawBatuTambahan(stonePos[i][0], stonePos[i][1], 0.5f+0.3f*sinf(i*0.7f));

    drawSemuaBurung();
    drawSemuaPeziarah();   // <--- PEZIARAH DIGAMBAR DI SINI
    drawSemuaKunangKunang();

    // ===== HUD =====
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    char info[256];
    const char* modeNames[3] = {"Orbit", "First-Person", "Cinematic"};
    const char* timeNames[3] = {"Siang", "Senja", "Malam"};
    int timeIdx = (dayFactor > 0.7f) ? 0 : ((dayFactor > 0.3f) ? 1 : 2);
    sprintf(info, "TAHAP 3 - Kompleks Candi 3D (Animasi) | Mode: %s | %s",
            modeNames[cameraMode], timeNames[timeIdx]);
    glRasterPos2i(20, windowHeight - 30);
    for (char* c = info; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    sprintf(info, "WASD: Gerak | Mouse: Lihat/Orbit | Space: First-Person | C: Cinematic | L: Lampu | R: Reset | O: Buka Pintu | K: Tutup Pintu");
    glRasterPos2i(20, windowHeight - 55);
    for (char* c = info; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

// ============================================================================
// INPUT HANDLER
// ============================================================================

void handleKeyboard(unsigned char key, int x, int y) {
    float speed = 0.3f;
    switch (key) {
        case 27: exit(0); break;
        case ' ':
            cameraMode = (cameraMode == 1) ? 0 : 1;
            if (cameraMode == 1) {
                camX = 0; camY = 5.0f; camZ = 35.0f;
                camYaw = 0; camPitch = -10.0f;
            }
            break;
        case 'c': case 'C':
            cameraMode = (cameraMode == 2) ? 0 : 2;
            break;
        case 'l': case 'L':
            lampuMenyala = !lampuMenyala;
            break;
        case 'o': case 'O':
            pintuManual = true;
            pintuTargetAngle = 75.0f;
            break;
        case 'k': case 'K':
            pintuManual = true;
            pintuTargetAngle = 0.0f;
            break;
        case 'w': case 'W':
            if (cameraMode == 1) {
                float dx = speed * sinf(camYaw * 3.14159f / 180.0f);
                float dz = speed * cosf(camYaw * 3.14159f / 180.0f);
                tryMove(dx, dz);
            }
            break;
        case 's': case 'S':
            if (cameraMode == 1) {
                float dx = -speed * sinf(camYaw * 3.14159f / 180.0f);
                float dz = -speed * cosf(camYaw * 3.14159f / 180.0f);
                tryMove(dx, dz);
            }
            break;
        case 'a': case 'A':
            if (cameraMode == 1) {
                float dx = speed * cosf(camYaw * 3.14159f / 180.0f);
                float dz = -speed * sinf(camYaw * 3.14159f / 180.0f);
                tryMove(dx, dz);
            }
            break;
        case 'd': case 'D':
            if (cameraMode == 1) {
                float dx = -speed * cosf(camYaw * 3.14159f / 180.0f);
                float dz = speed * sinf(camYaw * 3.14159f / 180.0f);
                tryMove(dx, dz);
            }
            break;
        case 'r': case 'R':
            cameraAngleX = 25.0f; cameraAngleY = -45.0f; cameraDistance = 60.0f;
            camX = 0; camY = 5.0f; camZ = 35.0f; camYaw = 0; camPitch = -10.0f;
            cameraMode = 0;
            pintuManual = false;
            break;
    }
    glutPostRedisplay();
}

void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        mouseDown = (state == GLUT_DOWN);
        mouseX = x; mouseY = y;
    }
    if (button == 3) cameraDistance = std::max(10.0f, cameraDistance - 2.0f);
    if (button == 4) cameraDistance = std::min(120.0f, cameraDistance + 2.0f);
    glutPostRedisplay();
}

void handleMouseMotion(int x, int y) {
    if (mouseDown) {
        int dx = x - mouseX, dy = y - mouseY;
        if (cameraMode == 1) {
            camYaw += dx * 0.15f;
            camPitch = std::max(-85.0f, std::min(85.0f, camPitch + dy * 0.15f));
        } else if (cameraMode == 0) {
            cameraAngleY += dx * 0.3f;
            cameraAngleX = std::max(-85.0f, std::min(85.0f, cameraAngleX + dy * 0.3f));
        }
        mouseX = x; mouseY = y;
        glutPostRedisplay();
    }
}

void handleResize(int width, int height) {
    windowWidth = width; windowHeight = height;
    glViewport(0, 0, width, height);
}

void timerFunc(int value) {
    updateAnimation();
    glutPostRedisplay();
    glutTimerFunc(16, timerFunc, 0);
}

// ============================================================================
// INIT
// ============================================================================

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.35f, 0.55f, 0.75f, 1.0f);

    glEnable(GL_FOG);
    GLfloat fogColor[] = {0.55f, 0.7f, 0.85f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 55.0f);
    glFogf(GL_FOG_END, 140.0f);
    glHint(GL_FOG_HINT, GL_NICEST);

    initTextures();
    initStars();
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("TAHAP 3 - Kompleks Candi 3D Beranimasi (Geby Rambu Awa - 248111064)");

    initOpenGL();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeyboard);
    glutMouseFunc(handleMouse);
    glutMotionFunc(handleMouseMotion);
    glutTimerFunc(16, timerFunc, 0);

    glutMainLoop();
    return 0;
}