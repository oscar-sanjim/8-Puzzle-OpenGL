#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <list>
#include <string>
#include "Libre.h"

#define NMAX 400000

//compile; g++ puzzleHash.cpp libreria.cpp -std=c++11 -lglut -lGL -lGLU

class Nodo {
public:
    Nodo(){padre=NULL;}
    Nodo *padre;
    int puzzle[3][3];
    void set(int puz[][3]){memcpy(puzzle,puz,9*sizeof(int));}
    int isEqual(int puz[][3]){return memcmp(puzzle,puz,9*sizeof(int))==0;}
    void print(){int i,j;
       for(i=0;i<3;i++) {for(j=0;j<3;j++) printf("%-2d",puzzle[i][j]); printf("\n");}
    }
    void generaHijos(Nodo hijos[],int &numHijos){
        int iC,jC,i,j,bol=1,cont=0;
        for(i=0;i<3 && bol;i++)
          for (j=0;j<3 && bol;j++) if (puzzle[i][j]==0) {iC=i;jC=j;bol=0;}
        if (isValid(iC+1,jC)) creaHijo(hijos[cont++],iC,jC,iC+1,jC);
        if (isValid(iC-1,jC)) creaHijo(hijos[cont++],iC,jC,iC-1,jC);
        if (isValid(iC,jC+1)) creaHijo(hijos[cont++],iC,jC,iC,jC+1);
        if (isValid(iC,jC-1)) creaHijo(hijos[cont++],iC,jC,iC,jC-1);
        numHijos=cont;
    }
private:
    

    friend class Cola;
    int isValid(int i,int j) {return i>=0 && i<3 && j>=0 && j<3;}
    void creaHijo(Nodo &h,int iCero,int jCero,int iCeroPost,int jCeroPost){
        h=*this; h.padre=this;
        h.puzzle[iCero][jCero]=h.puzzle[iCeroPost][jCeroPost];
        h.puzzle[iCeroPost][jCeroPost]=0;
    }
};




class Cola {
public:
    Cola(){ int i; ini=fin=0; for(i=0;i<NMAX;i++) Enc[i]=0;}
    int isEmpty(){return ini==fin;}
    void enQueue(Nodo n){ A[fin++]=n; Enc[hash(n.puzzle)]=1; }
    Nodo * deQueue(){if (!isEmpty()) return &A[ini++]; else return NULL; }
    int isElement(Nodo &n){
       return Enc[hash(n.puzzle)];
    }
    void reportaSol(Nodo *n){
        while(n!=NULL){
            n->print();
            printf("\n");
            n=n->padre;
        }
    }
private:
    int ini,fin;
    Nodo A[NMAX];
    int Enc[NMAX];

    int hash(int tablero[][3]){
        static int fac[8]={40320,5040,720,120,24,6,2,1};
        int tab[3][3];
        memcpy(tab,tablero,9*sizeof(int));
        int cont=0,i,j,k,acum=0,noEncontrado=1;
        for(i=1;i<=8;i++,cont=0,noEncontrado=1)
         for(j=0;j<3 && noEncontrado;j++)
          for(k=0;k<3 && noEncontrado;k++)
            if(tab[j][k]==i) { acum += (fac[i-1]*cont); tab[j][k]=-1;  noEncontrado=0; }
            else
             if(tab[j][k] != -1) cont++;
        return acum;
    }

} Open;

std::list<Nodo *> orderedMovementList;
std::list<Nodo*>::iterator iterator;
int listSize;
int counter = 1;
float red[] = {
    .831f,
    .667f,
    .502f,
    .251f,
    .133f,
    .051f,
    .647f,
    .482f,
    .337f
};

float green[] = {
    .416f,
    .220f,
    .062f,
    .498f,
    .40f,
    .302f,
    .776f,
    .624f,
    .467f
};

float blue[] = {
    .416f,
    .224f,
    .082f,
    .498f,
    .400f,
    .302f,
    .388f,
    .208f,
    .078f
};

void createMovementList(Nodo* n){
    while(n!=NULL){
        orderedMovementList.push_front(n);
        n = n->padre;
    }

    printf("****** This is ours *********\n");
    for(std::list<Nodo*>::iterator i=orderedMovementList.begin(); i!= orderedMovementList.end(); i++){
        (*i)->print();
        printf("\n");
    }
    iterator = orderedMovementList.begin();
    listSize = orderedMovementList.size();
    printf("%d\n",listSize);
}

void breadthSearch(){
    Nodo raiz,*n,hijos[4];
    int nh,j,cont=0;
    int goal[3][3]={{1,2,3},{4,5,6},{7,8,0}};
    int start[3][3]={{8,0,6},{5,3,2},{7,4,1}};
    raiz.set(start);
    Open.enQueue(raiz);
    while(!Open.isEmpty() && ++cont<400000 ){
        n=Open.deQueue();
        if (n->isEqual(goal)) {

            Open.reportaSol(n);
            createMovementList(n);
            return;
        }
        n->generaHijos(hijos,nh);
        for(j=0;j<nh;j++)
            if (!Open.isElement(hijos[j])) Open.enQueue(hijos[j]);
    }
}


// OpenGL Functions //
bool init(void)
{
    glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               // Black Background
    glClearDepth(1.0f);                                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
    return true;
}


void render(void)
{
	char result[100];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glTranslatef(0.0f,0.0f,-7.0f);

    std::string counterString = std::to_string(counter);
    char const *pListCounter = counterString.c_str();

    std::string listSizeString = std::to_string(listSize);
    char const *pListSize = listSizeString.c_str();

    strcpy(result, pListCounter);
    strcat(result, "/");
    strcat(result, pListSize);

    outTextXY(-4.5f,6.0f, result);
    outTextXY(-7.0f,-13.0f, "Press space bar");
    glLoadIdentity();
      

    for (int i = 0; i < 3; ++i)
    {   
        glLoadIdentity();                   // Reset The Current Modelview Matrix
        glTranslatef(-7.0f,-3.0f + (3 * i),-7.0f);
    
        for (int j = 0; j < 3; ++j)
        {
            int num = (*iterator)->puzzle[2-i][j];
            glTranslatef(3.5f,0.0f,0.0f);
            glColor3f(red[(num+8)%9] ,green[(num+8)%9] ,blue[(num+8)%9]);              // Set The Color To Blue One Time Only
            glBegin(GL_QUADS);                  // Start Drawing Quads
            glVertex3f(-1.0f, 1.0f, 0.0f);          // Left And Up 1 Unit (Top Left)
            glVertex3f( 1.0f, 1.0f, 0.0f);          // Right And Up 1 Unit (Top Right)
            glVertex3f( 1.0f,-1.0f, 0.0f);          // Right And Down One Unit (Bottom Right)
            glVertex3f(-1.0f,-1.0f, 0.0f);          // Left And Down One Unit (Bottom Left)
            glEnd();
            std::string s = std::to_string(num);
            char const *pchar = s.c_str();
            outTextXY(-.1f,-.2f, pchar);
        }                        // Done Drawing A Quad
    }
    
    
    glutSwapBuffers ( );
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
   // glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    // Calculate The Aspect Ratio And Set The Clipping Volume
    if (h == 0) h = 1;

    gluPerspective(80, (float)w/(float)h, 1.0, 5000.0);
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
    glLoadIdentity();                // Reset The Modelview Matrix
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:        // When Escape Is Pressed...
            exit(0);    // Exit The Program
            break;          // Ready For Next Case
        case 32:
            if(counter < listSize){
                counter++;
                iterator++;
                glutPostRedisplay();
            }
            break;
        default:        // Now Wrap It Up
            break;
    }
}


int main(int argc, char** argv){
    clock_t inicio,fin;
    double tiempo;
    inicio=clock();
    breadthSearch();
    fin=clock();
    tiempo=(double)(fin-inicio)/CLOCKS_PER_SEC;
    printf("Tardo %lf segundos\n",tiempo);


    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);     // Display Mode (Rgb And Double Buffered)


    glutInitWindowSize(500, 500);                // Window Size If We Start In Windowed Mode
    glutCreateWindow("8 Puzzle"); // Window Title
    init();                                          // Our Initialization
 
    //glViewport(0,0,500,500);

    
    glutDisplayFunc(render);                         // Register The Display Function
    glutReshapeFunc(reshape);                        // Register The Reshape Handler
    glutKeyboardFunc(keyboard);                      // Register The Keyboard Handler
    
    glutMainLoop();                                  // Go To GLUT Main Loop
    return 0;
}

