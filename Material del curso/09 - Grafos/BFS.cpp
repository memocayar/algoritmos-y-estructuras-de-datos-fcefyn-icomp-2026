// pagina 242 Alfred V. Aho  1988 Addison Wesley
// Busqueda en Amplitud
#include <cstdlib>
#include <iostream>
#define INFI  9000
#define MAXNODOS  5
#define no_visitado 0
#define visitado 1
#define MAX 10

using namespace std;
// almacena MAX-1 elementos
// cola circular
class Cola{
      private:
              int Q[MAX];
              int pi,pf;
  public:
      Cola(void){pi=0;pf=MAX-1;};
      //~Cola(void);
      int tope(){return Q[pi];};
      bool colavacia(){return pi==(pf+1)%MAX;};
      bool colallena(){return pi==(pf+2)%MAX;};
      void encolar(int a);
      void desencolar();
      int ultimo();
      string imprimir(string s);
};
//-------- Metodos de Cola --------------------
void Cola::encolar(int a)
{  if (!colallena()){
     pf=(pf+1)%MAX;
     Q[pf]=a; 
   }
   else cout<<"Cola llena"<<endl;  
}
void Cola::desencolar(void)
{  if (!colavacia())pi=(pi+1)%MAX;
   else cout<<"Cola vacia"<<endl;  
}
int Cola::ultimo(void)
{   return Q[pf];
}
string Cola::imprimir(string s)
{    if(!colavacia()){  
         //by Martin     
         for(int i=pi;i!=pf+1;i=(i+1)%MAX)cout<<Q[i]<<"--";
            
            }  
               
       return " <<< Q";
}
//------------------------------------------------------

void BFS(int x, int peso[][MAXNODOS], int marca[]);

using namespace std;

int main(int argc, char *argv[])
{   /*int peso[MAXNODOS][MAXNODOS]={INFI, INFI,    1,    1, INFI,
                                  INFI, INFI,    1, INFI, INFI,
                                     1,    1, INFI,    1,    1,
                                     1, INFI,    1, INFI,    1,
                                  INFI, INFI,    1,    1, INFI      
                                 };
    */
    int peso[MAXNODOS][MAXNODOS]={INFI,    1,    1,    1, INFI,
                                  INFI, INFI,    1,    1, INFI,
                                  INFI, INFI, INFI,    1, INFI,
                                  INFI, INFI, INFI, INFI,    1,
                                  INFI, INFI, INFI, INFI, INFI      
                                 };
	int marca[MAXNODOS];
 
    int i, j, v, s,t;
    
    for(i=0;i<MAXNODOS;i++) marca[i]=no_visitado;
    
    BFS(0,peso,marca);
         
                            
  
    cout<<endl<<"Matriz de pesos";   
    for(i=0;i<MAXNODOS;i++){
           cout<<"\n";                 
           for(j=0;j<MAXNODOS;j++){
                  printf(" %5i",peso[i][j]);                 
           }                 
    }
    cout<<endl<<"Arreglo de marcas"<<endl;   
    for(i=0;i<MAXNODOS;i++){
            cout<<i<<"  "<<marca[i]<<endl;                 
    }
    
    cout<<endl<<endl<<endl;
    system("PAUSE");
    return EXIT_SUCCESS;
}

void BFS(int x, int peso[][MAXNODOS], int marca[]){
   int y;
   Cola Q;
   
   marca[x]=visitado;
   Q.encolar(x);
   while(!Q.colavacia()){
      x=Q.tope();
      Q.desencolar();
      cout<<"x="<<x<<endl;                     
      for(y=0;y<MAXNODOS;y++){
          if((peso[x][y]==1)&&(marca[y]==no_visitado)){
               marca[y]=visitado;
               Q.encolar(y);
          }     
      }//fin for y      
   }//fin while
}

