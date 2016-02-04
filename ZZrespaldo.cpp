#include <stdio.h>
#include <string.h>
#include <time.h>
#define NMAX 400000

class Nodo {
public:
    Nodo(){
        padre=NULL;
    }
    
    void set(int puz[][3]){
        memcpy(puzzle,puz,9*sizeof(int));
    }
    
    int isEqual(int puz[][3])
    {
        return memcmp(puzzle,puz,9*sizeof(int))==0;
    }

    void print(){
        int i,j;
        for(i=0;i<3;i++) {
            for(j=0;j<3;j++){ 
                printf("%-2d",puzzle[i][j]); printf("\n");
            }
        }
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
    int puzzle[3][3];
    Nodo *padre;

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









void breadthSearch(){
    Nodo raiz,*n,hijos[4];
    int nh,j,cont=0;
    int start[3][3]={{1,2,3},{4,5,6},{7,8,0}};
    int goal[3][3]={{8,7,6},{5,4,3},{2,1,0}};
    raiz.set(start);
    Open.enQueue(raiz);
    while(!Open.isEmpty() && ++cont<400000 ){
        n=Open.deQueue();
        if (n->isEqual(goal)) {Open.reportaSol(n); return;}
        n->generaHijos(hijos,nh);
        for(j=0;j<nh;j++)
            if (!Open.isElement(hijos[j])) Open.enQueue(hijos[j]);
    }
}

int main(){
    clock_t inicio,fin;
    double tiempo;
    inicio=clock();
    breadthSearch();
    fin=clock();
   tiempo=(double)(fin-inicio)/CLOCKS_PER_SEC;
 printf("Tardo %lf segundos\n",tiempo);
 return 0;
}

