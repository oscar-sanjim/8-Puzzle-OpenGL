/* 
 * File:   Libre.h
 * Author: jaimesalazar
 *
 * Created on March 31, 2010, 8:29 AM
 */

#ifndef _LIBRE_H
#define	_LIBRE_H

#include <GL/gl.h>

#define PI 3.1415925



struct Color{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};


struct Point {
    float x;
    float y;
};

struct Point3D {
    float x;
    float y;
    float z;
};


class IMAGE {
public:
  unsigned char *buffer;
  int width;
  int height;
  IMAGE();
  IMAGE(int w,int h);
  void setBuffer(int w,int h);
  void Free();
  void setPixel(int x,int y,Color C);
  Color getPixel(int x,int y);
  void loadBMPImage(char *filename);
};

class IMAGE_RGB {
public:
  unsigned char *buffer;
  int width;
  int height;
  IMAGE_RGB();
  IMAGE_RGB(int w,int h);
  void setBuffer(int w,int h);
  void Free();
  void setPixel(int x,int y,Color C);
  Color getPixel(int x,int y);
  void loadFromCam(unsigned char *buff,int w,int h,int widthSize);
  void copy(IMAGE_RGB&);
  void blackWhite();
};

typedef struct {
    unsigned char L;
    unsigned char H;
} WORD;

void setNormal(GLfloat a[3],GLfloat b[3],GLfloat c[3]);
void findNormal(GLfloat a[],GLfloat b[],GLfloat c[],GLfloat n[]);
void findNormalRev(GLfloat p[],GLfloat n[]);
void _circle(int numDiv);
void elipse(float x,float y,float z,float a,float b);
int genVert(GLfloat vert[][3],float xmin,float xmax,int numDiv,float (*f)(float x) );
void solRevX(GLfloat vert[][3],int N,int numDiv);
void morphSolRevX(GLfloat vertF_A[][3],GLfloat vertF_B[][3],int N,int numDiv,float u);

/*  TEXTURAS          */


class Texture
{
private :
    GLuint textura;
    IMAGE im;
    IMAGE_RGB imRGB;
    int tipo;  // 0: im, 1: RGB
public:
    Texture();
    void set();
    void loadBMPTexture(char *filename);
    void loadTextureFromImage(IMAGE ima);
    void loadTextureFromImageRGB(IMAGE_RGB imaRGB);
    void loadSubTextureFromImage(IMAGE ima,int offsetx,int offsety);
    void changeTextureFromImageRGB(IMAGE_RGB imaRGB);
    IMAGE& getImage();
    void setWrap();
    void setReplace();
};

void textureLighting(bool b);

/*    MESH  */

class Mesh
{
    GLfloat xmin;
    GLfloat xmax;
    GLfloat ymin;
    GLfloat ymax;
    int NDivX;
    int NDivY;
    GLfloat *buff;
    GLfloat *buffNormal;
public:
    Mesh();
    ~Mesh();
    void genMesh(GLfloat x0,GLfloat x1,GLfloat y0,GLfloat y1,int NDX,int NDY,float (*)(float x,float y));
    void setZ(int i,int j,GLfloat z);
    GLfloat getZ(int i,int j);  // i>=0 , i <=NDivX , lo mismo para j
    void setNormal(int i,int j,GLfloat N[]);
    void getNormal(int i,int j,GLfloat N[]);
    void getPoint(int i,int j,GLfloat N[]);
    void drawMesh();
    void escaleZ(float sc);
};

class ParamMesh //
{
    float paso;
    GLuint lista;
    void (*fuv)(float&,float&,float,float);
    void createMesh();
public:
    ParamMesh();
    ~ParamMesh();
    void setFuv(void (*fuv)(float&,float&,float,float));
    void draw();
};


/* SHPERE */

class Sphere
{
private:
    static Point3D puntos[600][300];
    static bool callist;
    static GLuint lista;
    void generaPuntos();
    void generaLista();
public:
    Sphere();
    ~Sphere();
    void draw();
};


void outTextXY(float x,float y,const char* s);
void setColorFont(GLfloat r,GLfloat g,GLfloat b,GLfloat al);
bool isNear(Point p0,Point p1,Point p);
void transpose(GLfloat matriz[][4]);
void matrixVector(float A[][4],float x[],float y[]);
void impPoint(Point po);
bool isEqual(Color,Color);
Color getPixel(int x,int y);

#endif	/* _LIBRE_H */




