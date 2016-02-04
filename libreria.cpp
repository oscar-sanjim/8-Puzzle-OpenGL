#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "Libre.h"

#define PI 3.1415925
#define epsilon 10
Sphere _esfera;

void setNormal(GLfloat a[3],GLfloat b[3],GLfloat c[3]){
    GLfloat v1[3],v2[3],vr[3], norma;
    v1[0]=b[0]-a[0];  v1[1]=b[1]-a[1];  v1[2]=b[2]-a[2];
    v2[0]=c[0]-a[0];  v2[1]=c[1]-a[1];  v2[2]=c[2]-a[2];
    vr[0]=v1[1]*v2[2]-v1[2]*v2[1]; vr[1]=v1[2]*v2[0]-v1[0]*v2[2]; vr[2]=v1[0]*v2[1]-v1[1]*v2[0];
    norma = sqrt(vr[0]*vr[0]+vr[1]*vr[1]+vr[2]*vr[2]);
    vr[0] /= norma; vr[1] /= norma; vr[2] /= norma;
    glNormal3fv(vr);
}

void findNormal(GLfloat a[],GLfloat b[],GLfloat c[],GLfloat n[]){
    GLfloat v1[3],v2[3],vr[3], norma;
    v1[0]=b[0]-a[0];  v1[1]=b[1]-a[1];  v1[2]=b[2]-a[2];
    v2[0]=c[0]-a[0];  v2[1]=c[1]-a[1];  v2[2]=c[2]-a[2];
    n[0]=v1[1]*v2[2]-v1[2]*v2[1]; n[1]=v1[2]*v2[0]-v1[0]*v2[2]; n[2]=v1[0]*v2[1]-v1[1]*v2[0];
    norma = sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
    n[0] /= norma; n[1] /= norma; n[2] /= norma;
}

void findNormalRev(GLfloat p[],GLfloat n[]){
    GLfloat norma = sqrt(p[1]*p[1]+p[2]*p[2]);
    n[0]=0; n[1]=p[1]/norma; n[2]=p[2]/norma;
}

void invNormal(GLfloat n[]){
    n[0]=-n[0]; n[1]=-n[1]; n[2]=-n[2];
}

bool isNear(Point p0,Point p1,Point p){
    Point A,B;
    float AcruzB,A2,AporB;
    A.x=p1.x-p0.x;  A.y=p1.y-p0.y; B.x=p.x-p0.x;   B.y=p.y-p0.y;
    AcruzB=A.x*B.y-A.y*B.x;
    if (AcruzB<0) return false;
    A2=A.x*A.x+A.y*A.y;
    AporB=A.x*B.x+A.y*B.y;
    if (AcruzB*AcruzB > epsilon*epsilon*A2) return false;
    if (AporB>0 && AporB<A2) return true;
    return false;
}


void transpose(GLfloat matriz[][4]){
    GLfloat temp;
    for (int i=0;i<4;i++)
      for (int j=0;j<=i;j++) {
          temp=matriz[i][j]; matriz[i][j]=matriz[j][i]; matriz[j][i]=temp;
      }
}

void matrixVector(float A[][4],float x[],float y[]){
    float acum;
    for (int i=0;i<4;i++){
        acum=0;
        for (int j=0;j<4;j++) acum = acum + A[i][j]*x[j];
        y[i]=acum;
    }
}

void impPoint(Point po){
    printf("(%.2f,%.2f)\n",po.x,po.y);
}


Color getPixel(int x,int y){
  unsigned char Pixels[4];
  Color res={0};
  glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,Pixels);
  res.R=Pixels[0]; res.G=Pixels[1]; res.B=Pixels[2]; res.A=Pixels[3];
  return res;
}

GLuint _listaCir=0;

void _circle(int numDiv){
    float deltaT=2*PI/numDiv;
    _listaCir=glGenLists(1);
    glNewList(_listaCir,GL_COMPILE);
    glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0,0,0);
      for(int n=0;n<=numDiv;n++)
          glVertex3f(cos(n*deltaT),sin(n*deltaT),0);
    glEnd();
    glEndList();
}

void elipse(float x,float y,float z,float a,float b){
    if (_listaCir==0) _circle(20);
    glPushMatrix();
      glTranslatef(x,y,z);
      glScalef(a,b,1);
      glCallList(_listaCir);
    glPopMatrix();
}

int genVert(GLfloat vert[][3],float xmin,float xmax,int numDiv,float (*f)(float x) ){
    int i=0;
    float x=xmin,y,incX=(xmax-xmin)/numDiv;
    while(x<xmax){
        y=f(x);
        vert[i][0]=x; vert[i][1]=y; vert[i][2]=0;
        x += incX; i++;
    }
    return i;
}


void solRevX(GLfloat vert[][3],int N,int numDiv){
    GLfloat vertP[1000][3],vertP1[1000][3],n[3];
    GLfloat s[1000],t[1000];
    int i,j;
    float seno,coseno, ang=2*PI/numDiv,incS=1.0/numDiv,incT=1.0/(N-1);
    assert(N<1000);
    memcpy(vertP,vert,sizeof(GLfloat)*N*3);
    t[0]=s[0]=0;
    for(j=1;j<N;j++) t[j] = t[j-1]+incT;
    for(i=1;i<=numDiv;i++){
        s[i]=s[i-1]+incS;
        seno=sin(i*ang); coseno=cos(i*ang);
        for(j=0;j<N;j++) {
            vertP1[j][0]=vert[j][0];
            vertP1[j][1]=coseno*vert[j][1];
            vertP1[j][2]=seno*vert[j][1];
        }
        glBegin(GL_QUADS);
        for(j=0;j<N-1;j++){
      //      findNormal(vertP[j],vertP1[j],vertP1[j+1],n);
            findNormalRev(vertP[j],n); glNormal3fv(n); glTexCoord2f(s[i-1],t[j]); glVertex3fv(vertP[j]);
            findNormalRev(vertP1[j],n); glNormal3fv(n); glTexCoord2f(s[i],t[j]); glVertex3fv(vertP1[j]);
            findNormalRev(vertP1[j+1],n); glNormal3fv(n); glTexCoord2f(s[i],t[j+1]);glVertex3fv(vertP1[j+1]);
            findNormalRev(vertP[j+1],n); glNormal3fv(n); glTexCoord2f(s[i-1],t[j+1]); glVertex3fv(vertP[j+1]);
        }
        glEnd();
        memcpy(vertP,vertP1,sizeof(GLfloat)*N*3);
    }
}

void morphSolRevX(GLfloat vertF_A[][3],GLfloat vertF_B[][3],int N,int numDiv,float u){
    GLfloat vertP[1000][3],vertP1[1000][3],vertT[1000][3],n[3];
    GLfloat s[1000],t[1000],incS=1.0/numDiv,incT=1.0/(N-1);
    int i,j;
    float seno,coseno, ang=2*PI/numDiv;
    assert(N<1000);
    t[0]=s[0]=0;
    for(j=1;j<N;j++) t[j] = t[j-1]+incT;
    for(i=0;i<N;i++) for(j=0;j<3;j++) vertT[i][j]=(1-u)*vertF_A[i][j]+u*vertF_B[i][j];
    memcpy(vertP,vertT,sizeof(GLfloat)*N*3);
    for(i=1;i<=numDiv;i++){
        s[i]=s[i-1]+incS;
        seno=sin(i*ang); coseno=cos(i*ang);
        for(j=0;j<N;j++) {
            vertP1[j][0]=vertT[j][0];
            vertP1[j][1]=coseno*vertT[j][1];
            vertP1[j][2]=seno*vertT[j][1];
        }
        glBegin(GL_QUADS);
        for(j=0;j<N-1;j++){
            findNormalRev(vertP[j],n); glNormal3fv(n); glTexCoord2f(s[i-1],t[j]); glVertex3fv(vertP[j]);
            findNormalRev(vertP1[j],n); glNormal3fv(n); glTexCoord2f(s[i],t[j]); glVertex3fv(vertP1[j]);
            findNormalRev(vertP1[j+1],n); glNormal3fv(n);glTexCoord2f(s[i],t[j+1]);glVertex3fv(vertP1[j+1]);
            findNormalRev(vertP[j+1],n); glNormal3fv(n); glTexCoord2f(s[i-1],t[j+1]);glVertex3fv(vertP[j+1]);
        }
        glEnd();
        memcpy(vertP,vertP1,sizeof(GLfloat)*N*3);
    }
}

/*   IMAGE          */

IMAGE::IMAGE(){
    buffer=NULL; width=0; height=0;
}

IMAGE::IMAGE(int w,int h){
    buffer=(unsigned char *)malloc(w*h*4);
    assert(buffer!=NULL);
    width=w; height=h;
    memset(buffer,0,w*h*4);
}

void IMAGE::setPixel(int x,int y,Color C){
    if (x<width && x>=0 && y<height && y>0) {
         unsigned char *ch=&buffer[(width*y+x)<<2];
        *ch++=C.R; *ch++=C.G;*ch++=C.B; *ch++=C.A;
    }
}

void IMAGE::setBuffer(int w, int h){
    if (buffer != NULL) free(buffer);
    if (buffer==NULL) {
     buffer=(unsigned char *)malloc(w*h*4);
     assert(buffer!=NULL);
     width=w; height=h;
     memset(buffer,0,w*h*4);
    }
}

void IMAGE::Free(){
  if (buffer != NULL) { free(buffer); buffer=NULL;}
}

Color IMAGE::getPixel(int x,int y){
    Color c={0,0,0,0};
    if (x<width && x>=0 && y<height && y>0){
         unsigned char *ch=&buffer[(width*y+x)<<2];
         c.R=*ch++; c.G=*ch++; c.B=*ch++; c.A=*ch++;
     }
    return c;
}

void IMAGE::loadBMPImage(char* filename){
  FILE *stream;
  unsigned char *ap,*apL,R,G,B,Alf,*apIni,*apFin;
  unsigned char buffLinea[8000];
  WORD	bfType,biBitCount;
  int ancho,bfOffBits,biWidth,biHeight,neg=0;

  stream = fopen(filename, "rb" );
  if( stream == NULL ) return;
  fread(&bfType,sizeof(bfType),1,stream);
  if (bfType.L != 'B' && bfType.H != 'M') { printf("Archivo no es BMP\n"); return;}
  fseek(stream, 10, SEEK_SET);
  fread(&bfOffBits,sizeof(bfOffBits),1,stream);
  fseek(stream, 18, SEEK_SET);
  fread(&biWidth,sizeof(biWidth),1,stream);
  fread(&biHeight,sizeof(biHeight),1,stream);
  if(biHeight<0)  { biHeight=-biHeight; neg=1;}
  fseek(stream, 28, SEEK_SET);
  fread(&biBitCount,sizeof(biBitCount),1,stream);
  if(biBitCount.L != 24 && biBitCount.L != 32 )  { printf("BMP no es de 24/32 bits sino de %d\n",biBitCount.L); return;}
  fseek(stream, bfOffBits, SEEK_SET);
  if (biBitCount.L == 24) ancho = 3*biWidth;
  else ancho = 4*biWidth;
  buffer=(unsigned char *)malloc(4*biWidth*biHeight);
  assert(buffer!=NULL);
  ap=buffer;
  if (biBitCount.L == 24) ancho=(ancho-1)/4*4+4;
  for(int i=0;i<biHeight;i++){
      fread(buffLinea,ancho,1,stream);
	  apL=buffLinea;
	  for(int j=0;j<biWidth;j++){
		  B=*apL; apL++; G=*apL; apL++; R=*apL;
                  if (biBitCount.L == 24) apL++;
                  else {apL++; Alf=*apL; apL++;}
		  *ap=R; ap++; *ap=G; ap++; *ap=B; ap++;
                  if (biBitCount.L==24) *ap=255; else *ap=Alf;
                  ap++;
	  }
  }
  fclose( stream );
  apIni=buffer; apFin=apIni+biWidth*(biHeight-1)*4;
  if (neg==1){
      for(int i=0;i<biHeight/2;i++){
          memcpy(buffLinea,apIni,biWidth*4);
          memcpy(apIni,apFin,biWidth*4);
          memcpy(apFin,buffLinea,biWidth*4);
          apIni = apIni+biWidth*4;
          apFin = apFin-biWidth*4;
      }
  }
  width=biWidth; height=biHeight;
}

/*   IMAGE_RGB          */

IMAGE_RGB::IMAGE_RGB(){
    buffer=NULL; width=0; height=0;
}

IMAGE_RGB::IMAGE_RGB(int w,int h){
    buffer=(unsigned char *)malloc(w*h*3);
    assert(buffer!=NULL);
    width=w; height=h;
    memset(buffer,0,w*h*3);
}

void IMAGE_RGB::setPixel(int x,int y,Color C){
 /*   if (x<width && x>=0 && y<height && y>0) {
         unsigned char *ch=&buffer[(width*y+x)<<2];
        *ch++=C.R; *ch++=C.G;*ch++=C.B; *ch++=C.A;
    } */
}

void IMAGE_RGB::blackWhite(){
    unsigned char *base=buffer,*temp;
    int tam=width*height;
    for(int i=0;i<tam;i++){
        unsigned char rojo,verde,azul;
        float gris;
        temp=base;
        rojo=*base++; verde=*base++; azul=*base++;
        gris=0.6*rojo+0.3*verde+0.1*azul;
        *temp++=(unsigned char)gris; *temp++=(unsigned char)gris;
        *temp=(unsigned char)gris;
    }
}

void IMAGE_RGB::setBuffer(int w, int h){
    if (w==0 || h==0) return;
    if (buffer != NULL && w==width && h==height) return;
    if (buffer != NULL) { free(buffer); buffer==NULL;}
    if (buffer==NULL) {
     buffer=(unsigned char *)malloc(w*h*3);
     if (buffer == NULL) { printf("Error al asignar imageRGB\n"); return;}
     assert(buffer!=NULL);
     width=w; height=h;
     memset(buffer,0,w*h*3);
    }
}

void IMAGE_RGB::Free(){
  if (buffer != NULL) { free(buffer); buffer=NULL;}
}

Color IMAGE_RGB::getPixel(int x,int y){
    Color c={0,0,0,0};
  /*  if (x<width && x>=0 && y<height && y>0){
         unsigned char *ch=&buffer[(width*y+x)<<2];
         c.R=*ch++; c.G=*ch++; c.B=*ch++; c.A=*ch++;
     }  */
    return c;
}

 void IMAGE_RGB::loadFromCam(unsigned char *buff,int w,int h,int widthSize){
     int salto=width*3;
     setBuffer(w,h);
     unsigned char * tempBuff=buffer + (height-1)*salto;
     for(int i=0;i<height;i++){
         memcpy(tempBuff,buff,salto);
         tempBuff = tempBuff - salto;
         buff = buff + widthSize;
     }
 }

 void IMAGE_RGB::copy(IMAGE_RGB& imrgb){
     int w=imrgb.width,h=imrgb.height;
     setBuffer(w,h);
     memcpy(buffer,imrgb.buffer,w*h*3);
 }

/* END OF IMAGE          */


/*    TEXTURAS      */

Texture::Texture(){
    textura=-1;
}

void Texture::set(){
 glEnable(GL_TEXTURE_2D);
 glEnable(GL_BLEND);
 glBindTexture(GL_TEXTURE_2D,textura);
}

void Texture::setWrap(){
 glEnable(GL_TEXTURE_2D);
 glEnable(GL_BLEND);
 glBindTexture(GL_TEXTURE_2D,textura);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

void Texture::setReplace(){
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D,textura);
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
 }

IMAGE& Texture::getImage(){
    return im;
}

void Texture::loadTextureFromImage(IMAGE ima){
    if ( textura > 0) if ( (im.buffer != NULL) && (im.buffer !=ima.buffer) ) free(im.buffer);
    im=ima;
    assert(im.buffer!=NULL);
    if (textura==-1) glGenTextures(1,&textura);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glBindTexture(GL_TEXTURE_2D,textura);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,im.width,im.height,0,GL_RGBA,GL_UNSIGNED_BYTE,im.buffer);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
 //   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}

void Texture::loadTextureFromImageRGB(IMAGE_RGB ima){
    imRGB=ima;
    assert(ima.buffer!=NULL);
    if (textura==-1) { glGenTextures(1,&textura);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glBindTexture(GL_TEXTURE_2D,textura);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    }
    glBindTexture(GL_TEXTURE_2D,textura);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,imRGB.width,imRGB.height,0,GL_BGR,GL_UNSIGNED_BYTE,imRGB.buffer);
    glEnable(GL_TEXTURE_2D);
}

void Texture::changeTextureFromImageRGB(IMAGE_RGB ima){
   if (textura==-1 || imRGB.buffer==NULL) return;
   if (imRGB.buffer !=ima.buffer) {
          printf("El buffer de la imagen en changeTexture debe ser el mismo\n");
          return;
     }
   glBindTexture(GL_TEXTURE_2D,textura);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ima.width, ima.height, 0, GL_BGR, GL_UNSIGNED_BYTE, imRGB.buffer);
   glEnable(GL_TEXTURE_2D);
}

void Texture::loadSubTextureFromImage(IMAGE ima, int xoffset, int yoffset){
   if (textura==-1) return;
   im=ima;
   glBindTexture(GL_TEXTURE_2D,textura);
   glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, im.width, im.height,GL_RGBA,GL_UNSIGNED_BYTE,im.buffer);
   glEnable(GL_TEXTURE_2D);
}

void Texture::loadBMPTexture(char *filename){
    im.loadBMPImage(filename);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1,&textura);
    glBindTexture(GL_TEXTURE_2D,textura);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,im.width,im.height,0,GL_RGBA,GL_UNSIGNED_BYTE,im.buffer);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}

void textureLighting(bool b){
    if (b)  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    else glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

/*  FIN DE  TEXTURAS      */


/*  MESH   */

void Mesh::genMesh(GLfloat x0,GLfloat x1,GLfloat y0,GLfloat y1,int NDX,int NDY,float (*f)(float,float)) {
    float incX=(x1-x0)/NDX, incY=(y1-y0)/NDY,x,y,a[3],b[3],c[3],n[3];
    int i,j;
    if (buff != NULL){
        if ((NDivX+1)*(NDivY+1)<(NDX+1)*(NDY+1)) {
           free(buff); free(buffNormal);
           buff=(GLfloat *) malloc(sizeof(GLfloat)*(NDX+1)*(NDY+1));
           buffNormal=(GLfloat *) malloc(sizeof(GLfloat)*(NDX+1)*(NDY+1)*3);
        }
    } else {
        buff=(GLfloat *) malloc(sizeof(GLfloat)*(NDX+1)*(NDY+1));
        buffNormal=(GLfloat *) malloc(sizeof(GLfloat)*(NDX+1)*(NDY+1)*3);
    }
    xmin=x0; xmax=x1; ymin=y0; ymax=y1; NDivX=NDX; NDivY=NDY;   
    assert(buff != NULL); 
    assert(buffNormal != NULL);
    for(i=0,x=x0;i<=NDX;i++,x += incX)
       for(j=0,y=y0;j<=NDY;j++,y += incY)
           setZ(i,j,(*f)(x,y));
    for(i=0;i<NDX;i++)
       for(j=0;j<NDY;j++){
           getPoint(i,j,a);
           getPoint(i+1,j,b);
           getPoint(i,j+1,c);
           findNormal(a,b,c,n);
           setNormal(i,j,n);
        }
     for(j=0;j<NDY;j++){
         getNormal(NDX-1,j,n);
         setNormal(NDX,j,n);
     }
    for(i=0;i<NDX;i++){
         getNormal(i,NDY-1,n);
         setNormal(i,NDY,n);
     }
    getNormal(NDX-1,NDY-1,n);
    setNormal(NDX,NDY,n);
}

void Mesh::drawMesh(){
    int i,j;
    GLfloat P0[3],P1[3],N0[3],N1[3];
    if (buff==NULL) { printf("Buff null\n"); return; }
    for(i=0;i<NDivX;i++){
        glBegin(GL_TRIANGLE_STRIP);
         for(j=0;j<NDivY;j++){
             getPoint(i,j,P0); getNormal(i,j,N0);
             getPoint(i+1,j,P1); getNormal(i+1,j,N1);
             glNormal3fv(N0);
             glTexCoord2f(i*1.0/NDivX,j*1.0/NDivY);
             glVertex3fv(P0);
             glNormal3fv(N1);
             glTexCoord2f((i+1)*1.0/NDivX,j*1.0/NDivY);
             glVertex3fv(P1);
         }
        glEnd();
    }
}

Mesh::Mesh(){
    buff=buffNormal=NULL;
}

Mesh::~Mesh(){
    if (buff != NULL) free(buff);
    if (buffNormal != NULL) free(buffNormal);
}

void Mesh::setZ(int i,int j,GLfloat z){
    assert(i>=0 && i<=NDivX && j>=0 && j<=NDivY);
    if (buff!= NULL) buff[(NDivX+1)*j+i]=z;
}

GLfloat Mesh::getZ(int i,int j){
    assert(i>=0 && i<=NDivX && j>=0 && j<=NDivY);
    if (buff!= NULL) return buff[(NDivX+1)*j+i];
}

void Mesh::setNormal(int i,int j,GLfloat N[]){
 assert(i>=0 && i<=NDivX && j>=0 && j<=NDivY);
 if (buffNormal!= NULL) memcpy(&buffNormal[((NDivX+1)*j+i)*3],N,sizeof(GLfloat)*3);
}

void Mesh::getNormal(int i,int j,GLfloat N[]){
 assert(i>=0 && i<=NDivX && j>=0 && j<=NDivY);
 if (buffNormal!= NULL) memcpy(N,&buffNormal[((NDivX+1)*j+i)*3],sizeof(GLfloat)*3);
}

void Mesh::getPoint(int i,int j,GLfloat P[]){
    P[0]=xmin + i*(xmax-xmin)/NDivX;
    P[1]=ymin + j*(ymax-ymin)/NDivY;
    P[2]=getZ(i,j);
}

void Mesh::escaleZ(float sc){
    int i,j;
    float z;
    GLfloat N[3],mag;
    for(i=0;i<=NDivX;i++)
       for(j=0;j<=NDivY;j++){
           z=getZ(i,j); setZ(i,j,sc*z);
           getNormal(i,j,N);
           N[0] *= sc;
           N[1] *= sc;
           mag=sqrt(N[0]*N[0]+N[1]*N[1]+N[2]*N[2]);
           N[0] /= mag; N[1] /= mag; N[2] /= mag;
           setNormal(i,j,N);
       }
}

ParamMesh::ParamMesh(){
    lista=0;
    paso=0.05;
}

ParamMesh::~ParamMesh(){
}

void ParamMesh::createMesh(){
         float u0,v0,u1,v1,u2,v2,u3,v3;
         glBegin(GL_QUADS);
         for (float i=0;i<1.0;i+=paso)
             for(float j=0;j<1.0;j+=paso){
                 (*fuv)(u0,v0,i,j);
                 (*fuv)(u1,v1,i+paso,j);
                 (*fuv)(u2,v2,i+paso,j+paso);
                  (*fuv)(u3,v3,i,j+paso);
                  glTexCoord2f(u0,v0); glVertex2f(i,j);
                  glTexCoord2f(u1,v1); glVertex2f(i+paso,j);
                  glTexCoord2f(u2,v2); glVertex2f(i+paso,j+paso);
                  glTexCoord2f(u3,v3); glVertex2f(i,j+paso);

             }
         glEnd();
}

void ParamMesh::setFuv(void (*fuv1)(float&,float&,float,float)){
    if (lista != 0) glDeleteLists(lista,1);
    lista=glGenLists(1);
    fuv=fuv1;
    glNewList(lista,GL_COMPILE);
      createMesh();
    glEndList();
}

void ParamMesh::draw(){
    glCallList(lista);
}

/* FIN DE MESH  */

/* SPHERE   */

bool Sphere::callist=false;
GLuint Sphere::lista=0;
Point3D Sphere::puntos[600][300]={0};

Sphere::Sphere(){
  //  callist=false;
}

Sphere::~Sphere(){
}

void Sphere::draw(){
    if (callist==false) { 
        generaPuntos();
        lista=glGenLists(1);
        generaLista(); callist=true;
    }
    glCallList(lista);
}

void Sphere::generaLista(){
    int i,j;
    glNewList(lista,GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for(i=0;i<599;i++)
        for(j=0;j<299;j++){
            glTexCoord2d(1.0*i/600,1.0*j/300);
            glVertex3f(puntos[i][j].x,puntos[i][j].y,puntos[i][j].z);
            glTexCoord2d(1.0*(i+1)/600,1.0*j/300);
            glVertex3f(puntos[i+1][j].x,puntos[i+1][j].y,puntos[i+1][j].z);
            glTexCoord2d(1.0*i/600,1.0*(j+1)/300);
            glVertex3f(puntos[i][j+1].x,puntos[i][j+1].y,puntos[i][j+1].z);
            glTexCoord2d(1.0*(i+1)/600,1.0*j/300);
            glVertex3f(puntos[i+1][j].x,puntos[i+1][j].y,puntos[i+1][j].z);
            glTexCoord2d(1.0*(i+1)/600,1.0*(j+1)/300);
            glVertex3f(puntos[i+1][j+1].x,puntos[i+1][j+1].y,puntos[i+1][j+1].z);
            glTexCoord2d(1.0*i/600,1.0*(j+1)/300);
            glVertex3f(puntos[i][j+1].x,puntos[i][j+1].y,puntos[i][j+1].z);
        }
    glEnd();
    glEndList();
}

void Sphere::generaPuntos(){
    int i,j;
    float theta,phi,incTheta=2*PI/599,incPhi=PI/299;
    for(j=0,phi=-PI/2;j<300;j++,phi += incPhi)
        for(i=0,theta=0;i<600;i++,theta += incTheta){
            puntos[i][j].y=sin(phi);
            puntos[i][j].z=cos(phi)*cos(theta);
            puntos[i][j].x=cos(phi)*sin(theta);
        }
}

/* Fin de Sphere */

/*  TEXTO  */

GLfloat _colorRFont=0,_colorGFont=0,_colorBFont=1,_colorAFont=1;

void setColorFont(GLfloat r,GLfloat g,GLfloat b,GLfloat al){
  _colorRFont=r; _colorGFont=g; _colorBFont=b; _colorAFont=al;
}

bool isEqual(Color c1, Color c2){
    return c1.A==c2.A && c1.B==c2.B && c1.G == c2.G && c1.R == c2.R;
}

void outTextXY(float x,float y,const char* s)
{
    GLfloat colorAct[4];
    GLboolean para;
 //   glGetFloatv(GL_CURRENT_COLOR,colorAct);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x,y);
 //   glGetBooleanv(GL_TEXTURE_2D,&para);
 //   if (para == 1) glDisable(GL_TEXTURE_2D);
   while (*s) {
   glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *s);
         s++;
   }
    
 //  if (para == 1) glEnable(GL_TEXTURE_2D);
 //  glColor4f(colorAct[0],colorAct[1],colorAct[2],colorAct[3]);
}
