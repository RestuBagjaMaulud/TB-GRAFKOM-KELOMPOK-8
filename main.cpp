#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Variable untuk boneka salju
float bonekaScale = 1.0f; // Skala awal boneka
float bonekaTranslateX = 0.0f; // Posisi awal translasi X boneka
float bonekaTranslateY = 0.0f; // Posisi awal translasi Y boneka

// Variabel untuk rotasi dengan mouse
float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
bool mouseDown = false;

void hiddenCarte();
bool hidden = false;

// Variabel ID tekstur
GLuint textureBackgroundID;
GLuint textureBackgroundMoon;
GLuint textureBackgroundDaun;
GLuint textureBackgroundHouse;
GLuint textureBackgroundLangit;
GLuint textureBackgroundTanah;

// Variabel pencahayaan
GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // Posisi cahaya
float light_angle = 0.0f; // Sudut rotasi cahaya

// Variabel posisi kamera
float cameraDistance = 8.0f; // Jarak kamera awal dari bulan

//----------------------------------Fungsi untuk Pencahayaan-----------------------------------
void setupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void updateLightPosition() // Memperbarui posisi cahaya() 
{
    light_angle += 0.1f; // Kecepatan rotasi cahaya
    if (light_angle > 360.0f)
        light_angle -= 360.0f;

    light_position[0] = 5.0f * cos(light_angle * M_PI / 180.0f);
    light_position[2] = 5.0f * sin(light_angle * M_PI / 180.0f);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glutPostRedisplay();
}

void idle() {
    updateLightPosition(); // Perbarui posisi cahaya secara kontinu
    glutPostRedisplay();   // Meminta redisplay untuk menggambar ulang
}

//------------------------------------Fungsi untuk Bulan----------------------------------
void drawMoon() {
    glEnable(GL_TEXTURE_2D); // Pastikan tekstur diaktifkan
    glPushMatrix();

    // Gambar bola dengan tekstur
    glTranslatef(7.0f, 4.0f, -5.0f); // Posisi bulan

    // Aktifkan tekstur dan tentukan koordinat tekstur
    glBindTexture(GL_TEXTURE_2D, textureBackgroundMoon);
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk bulan

    GLUquadric* quad1 = gluNewQuadric();
    gluQuadricTexture(quad1, GL_TRUE);
    gluSphere(quad1, 1.0, 50, 50);
    gluDeleteQuadric(quad1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

//----------------------------------Fungsi untuk menggambar kubus--------------------------
void drawCube(float x, float y, float z, float size) {
    float half = size / 2.0f;
    glBegin(GL_QUADS);
    // Sisi depan
    glVertex3f(x - half, y - half, z + half);
    glVertex3f(x + half, y - half, z + half);
    glVertex3f(x + half, y + half, z + half);
    glVertex3f(x - half, y + half, z + half);

    // Sisi belakang
    glVertex3f(x - half, y - half, z - half);
    glVertex3f(x + half, y - half, z - half);
    glVertex3f(x + half, y + half, z - half);
    glVertex3f(x - half, y + half, z - half);

    // Sisi kiri
    glVertex3f(x - half, y - half, z - half);
    glVertex3f(x - half, y - half, z + half);
    glVertex3f(x - half, y + half, z + half);
    glVertex3f(x - half, y + half, z - half);

    // Sisi kanan
    glVertex3f(x + half, y - half, z - half);
    glVertex3f(x + half, y - half, z + half);
    glVertex3f(x + half, y + half, z + half);
    glVertex3f(x + half, y + half, z - half);

    // Sisi atas
    glVertex3f(x - half, y + half, z - half);
    glVertex3f(x + half, y + half, z - half);
    glVertex3f(x + half, y + half, z + half);
    glVertex3f(x - half, y + half, z + half);

    // Sisi bawah
    glVertex3f(x - half, y - half, z - half);
    glVertex3f(x + half, y - half, z - half);
    glVertex3f(x + half, y - half, z + half);
    glVertex3f(x - half, y - half, z + half);
    glEnd();
}

//--------------------------------Fungsi menggambar rumah---------------------------------
void drawHouse() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBackgroundHouse); // Gunakan tekstur dinding
    glColor3f(1.0f, 1.0f, 1.0f); // warna putih
    // Depan
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.75f, 0.5f); // Kiri bawah
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.75f, 0.5f);  // Kanan bawah
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.25f, 0.5f);   // Kanan atas
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.25f, 0.5f);  // Kiri atas

    // Belakang
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.75f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.75f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.25f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.25f, -0.5f);

    // Samping Kanan
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.75f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.75f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.25f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.25f, -0.5f);

    // Samping Kiri
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.75f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.75f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.25f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.25f, -0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Atap rumah (dinaikkan ke atas)
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    // Segitiga depan
    glVertex3f(-0.7f, 0.1f, 0.7f);
    glVertex3f(0.7f, 0.1f, 0.7f);
    glVertex3f(0.0f, 0.8f, 0.0f);
    // Segitiga belakang
    glVertex3f(-0.7f, 0.1f, -0.7f);
    glVertex3f(0.7f, 0.1f, -0.7f);
    glVertex3f(0.0f, 0.8f, 0.0f);
    // Segitiga kiri
    glVertex3f(-0.7f, 0.1f, -0.7f);
    glVertex3f(-0.7f, 0.1f, 0.7f);
    glVertex3f(0.0f, 0.8f, 0.0f);
    // Segitiga kanan
    glVertex3f(0.7f, 0.1f, -0.7f);
    glVertex3f(0.7f, 0.1f, 0.7f);
    glVertex3f(0.0f, 0.8f, 0.0f);
    glEnd();

    // Cerobong asap (warna coklat gelap)
    glColor3f(0.5f, 0.3f, 0.2f);
    glPushMatrix();
    glTranslatef(0.3f, 0.45f, -0.3f); // Posisi cerobong lebih tinggi
    drawCube(0.0f, 0.0f, 0.0f, 0.2f); // Perbesar ukuran cerobong
    glPopMatrix();

    // Salju di atas cerobong asap
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk salju
    glPushMatrix();
    glTranslatef(0.3f, 0.55f, -0.3f); // Posisi di atas cerobong
    glutSolidSphere(0.1, 16, 16);   // Bentuk salju sebagai bola kecil
    glPopMatrix();

    // Pintu rumah
    glDisable(GL_LIGHTING);
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.51f);
    glScalef(0.2f, 0.4f, 0.01f);
    glutSolidCube(1.0);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    // Jendela kiri
    glColor3f(0.0f, 0.5f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.3f, -0.25f, 0.51f);
    glScalef(0.2f, 0.2f, 0.01f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Jendela kanan
    glColor3f(0.0f, 0.5f, 1.0f);
    glPushMatrix();
    glTranslatef(0.3f, -0.25f, 0.51f);
    glScalef(0.2f, 0.2f, 0.01f);
    glutSolidCube(1.0);
    glPopMatrix();
}

//-----------------------------------Fungsi menggambar boneka salju----------------------
void drawSnowman(float x, float y, float z) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(bonekaTranslateX, bonekaTranslateY, 0.0f);
    glScalef(bonekaScale, bonekaScale, bonekaScale);
    glutSolidSphere(0.4, 18, 18);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidSphere(0.3, 18, 18);
    glTranslatef(0.0f, 0.4f, 0.0f);
    glutSolidSphere(0.2, 18, 18);
    glPopMatrix();
}

//---------------------------------Fungsi menggambar pohon----------------------------
void drawTree(float x, float y, float z, float size) {
    float trunkHeight = size * 0.3f;  // Tinggi batang
    float trunkWidth = size * 0.2f;   // Lebar batang
    float foliageHeight = size * 0.4f; // Tinggi setiap daun
    float foliageBase = size * 0.6f;  // Lebar dasar daun

    // Gambar batang pohon
    glColor3f(0.55f, 0.27f, 0.07f); // Warna coklat untuk batang
    glPushMatrix();
    glTranslatef(x, y + trunkHeight / 2.0f, z);
    glScalef(trunkWidth, trunkHeight, trunkWidth);
    glutSolidCube(1.0);
    glPopMatrix();

    // Gambar daun pohon (5 segitiga bertumpuk dalam 3D)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBackgroundDaun);
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk mengaktifkan tekstur
    for (int i = 0; i < 5; ++i) {
        float offsetY = trunkHeight + i * foliageHeight * 0.5f;
        float currentBase = foliageBase * (1.0f - 0.2f * i);
        glPushMatrix();
        glTranslatef(x, y + offsetY, z);

        glBegin(GL_TRIANGLES);
        // Segitiga depan
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-currentBase, 0.0f, currentBase);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(currentBase, 0.0f, currentBase);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, foliageHeight, 0.0f);

        // Segitiga belakang
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-currentBase, 0.0f, -currentBase);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(currentBase, 0.0f, -currentBase);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, foliageHeight, 0.0f);

        // Segitiga kiri
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-currentBase, 0.0f, -currentBase);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-currentBase, 0.0f, currentBase);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, foliageHeight, 0.0f);

        // Segitiga kanan
        glTexCoord2f(0.0f, 0.0f); glVertex3f(currentBase, 0.0f, -currentBase);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(currentBase, 0.0f, currentBase);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, foliageHeight, 0.0f);
        glEnd();

        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
}

//-------------------------------------------Fungsi untuk Background--------------------------
void drawBackground() {
    glEnable(GL_TEXTURE_2D);

    // Background belakang
    glBindTexture(GL_TEXTURE_2D, textureBackgroundID);
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.5f, -5.0f, -10.0f); // Kiri bawah
    glTexCoord2f(1.0f, 0.0f); glVertex3f(13.5f, -5.0f, -10.0f);  // Kanan bawah
    glTexCoord2f(1.0f, 1.0f); glVertex3f(13.5f, 8.0f, -10.0f);   // Kanan atas
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.5f, 8.0f, -10.0f);  // Kiri atas
    glEnd();

    // Background depan
    glBindTexture(GL_TEXTURE_2D, textureBackgroundID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.5f, -5.0f, 10.0f); // Kiri bawah
    glTexCoord2f(1.0f, 0.0f); glVertex3f(13.5f, -5.0f, 10.0f);  // Kanan bawah
    glTexCoord2f(1.0f, 1.0f); glVertex3f(13.5f, 8.0f, 10.0f);   // Kanan atas
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.5f, 8.0f, 10.0f);  // Kiri atas
    glEnd();

    // Background kiri
    glBindTexture(GL_TEXTURE_2D, textureBackgroundID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.5f, -5.0f, -10.0f); // Kiri bawah
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-13.5f, -5.0f, 10.0f);  // Kanan bawah
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-13.5f, 8.0f, 10.0f);   // Kanan atas
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.5f, 8.0f, -10.0f);  // Kiri atas
    glEnd();

    // Background kanan
    glBindTexture(GL_TEXTURE_2D, textureBackgroundID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(13.5f, -5.0f, -10.0f); // Kiri bawah
    glTexCoord2f(1.0f, 0.0f); glVertex3f(13.5f, -5.0f, 10.0f);  // Kanan bawah
    glTexCoord2f(1.0f, 1.0f); glVertex3f(13.5f, 8.0f, 10.0f);   // Kanan atas
    glTexCoord2f(0.0f, 1.0f); glVertex3f(13.5f, 8.0f, -10.0f);  // Kiri atas
    glEnd();

    // Background atas
    glBindTexture(GL_TEXTURE_2D, textureBackgroundLangit);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.5f, 8.0f, -10.0f); // Kiri belakang
    glTexCoord2f(1.0f, 0.0f); glVertex3f(13.5f, 8.0f, -10.0f);  // Kanan belakang
    glTexCoord2f(1.0f, 1.0f); glVertex3f(13.5f, 8.0f, 10.0f);   // Kanan depan
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.5f, 8.0f, 10.0f);  // Kiri depan
    glEnd();

    // Background alas (tanah)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBackgroundTanah);
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk tanah
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.5f, -5.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(13.5f, -5.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(13.5f, -5.0f, -10.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.5f, -5.0f, -10.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//-------------------------------------------Fungsi untuk Kartesius---------------
void drawCartesius()
{
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    // x Line
    glVertex3f(-10.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);

    // y Line
    glVertex3f(0.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    // z Line
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(0.0, 0.0, 10.0);
    glEnd();
}

//--------------------------------------Fungsi untuk memuat tekstur---------------------------
GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(path, 0), path);
    FIBITMAP* pImage = FreeImage_ConvertTo24Bits(bitmap);
    int width = FreeImage_GetWidth(pImage);
    int height = FreeImage_GetHeight(pImage);
    void* data = FreeImage_GetBits(pImage);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(pImage);
    FreeImage_Unload(bitmap);
    return textureID;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Posisi kamera tetap berdasarkan cameraDistance
    gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);
    // Perputaran objek otomatis
    glRotatef(xrot, 1, 0, 0);
    glRotatef(yrot, 0, 1, 0); // Perputaran berkelanjutan pada sumbu Y
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Warna latar belakang

    hiddenCarte();
    drawBackground(); // Gambar latar belakang

    // Tambahkan bulan dengan pencahayaan
    glPushMatrix();
    setupLighting();
    drawMoon();
    glPopMatrix();

    // Gambar rumah
    glPushMatrix();
    glTranslatef(-4.0f, -2.3f, -6.0f);
    glScalef(3.5f, 3.5f, 3.5f); // Ukuran rumah diperbesar
    drawHouse();
    glPopMatrix();

    // Gambar boneka salju
    glPushMatrix();
    glTranslatef(0.0f, -4.6f, -6.0f);
    drawSnowman(0.0f, 0.0f, 0.0f);
    glPopMatrix();

    // Gambar pohon
    glPushMatrix();
    drawTree(4.0f, -5.0f, -7.0f, 3.5f);
    glPopMatrix();

    glutSwapBuffers();
}

//------------------------------------------Fungsi reshape--------------------------
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC untuk keluar
        exit(0);
        break;
    case 'r': // Tambahan untuk Zoom in
        cameraDistance -= 0.5f;
        if (cameraDistance < 2.0f) cameraDistance = 2.0f;  // Jangan terlalu dekat
        break;
    case 't': // Tambahan untuk Zoom out
        cameraDistance += 0.5f;
        break;
    case 'v': // Memperbesar skala boneka
            bonekaScale += 0.1f;
        break;
    case 'b': // Memperkecil skala boneka
        if (bonekaScale > 0.1f)
            bonekaScale -= 0.1f;
        break;
    case 'w': // Pindah boneka ke atas
        bonekaTranslateY += 0.1f;
        break;
    case 's': // Pindah boneka ke bawah
        bonekaTranslateY -= 0.1f;
        break;
    case 'a': // Pindah boneka ke kiri
        bonekaTranslateX -= 0.1f;
        break;
    case 'd': // Pindah boneka ke kanan
        bonekaTranslateX += 0.1f;
        break;
    case 'c':
            hidden = !hidden;
            break;
    }
    // Update posisi cahaya di OpenGL
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glutPostRedisplay();
}

void hiddenCarte()
{
    if (hidden)
    {
        drawCartesius();
    }
}

// Fungsi untuk input mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseDown = true;
        xdiff = x - yrot;
        ydiff = -y + xrot;
    } else {
        mouseDown = false;
    }
}

void mouseMotion(int x, int y) {
    if (mouseDown) {
        yrot = x - xdiff;
        xrot = y + ydiff;
        glutPostRedisplay();
    }
}

//------------------------------------------Fungsi inisialisasi--------------------------
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    textureBackgroundID = loadTexture("salju.jpg");
    textureBackgroundMoon = loadTexture("moon.jpg");
    textureBackgroundDaun = loadTexture("christmastree.jpg");
    textureBackgroundHouse = loadTexture("BadanRumah.jpg");
    textureBackgroundLangit = loadTexture("langit.jpg");
    textureBackgroundTanah = loadTexture("TanahSalju.jpg");
}

// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rumah di Musim Salju");
    glutFullScreen(); // tampilan fullscreen
    glewInit();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard); // Daftarkan fungsi keyboard
    glutIdleFunc(idle); // Mengatur fungsi idle
    glutMainLoop();
    return 0;
}
