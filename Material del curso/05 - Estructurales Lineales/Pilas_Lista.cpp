//  Pila con Lista.cpp  
//
#include <cstdlib> // Para exit() y system()
#include <iostream>
#include <string>


using namespace std;

// ==========================================
// Clase Nodo
// ==========================================
template <class T>
class Nodo {
private:
    T dato;
    Nodo* next;

public:
    Nodo(T a) { dato = a; next = NULL; }

    void set_dato(T a) { dato = a; }
    void set_next(Nodo* n) { next = n; }
    T get_dato() { return dato; }
    Nodo* get_next() { return next; }
};

// ==========================================
// Clase Lista
// ==========================================
template <class T>
class Lista {
private:
    Nodo<T>* czo;

    // Métodos recursivos auxiliares privados 
    int size_rec(Nodo<T>* n) {
        if (n == NULL) return 0;
        return 1 + size_rec(n->get_next());
    }

    void toPrint_rec(Nodo<T>* n) {
        if (n == NULL) return;
        cout << n->get_dato();
        toPrint_rec(n->get_next());
    }

    T suma_rec(Nodo<T>* n, T i) {
        if (n == NULL) return i;
        return n->get_dato() + suma_rec(n->get_next(), i);
    }
    
    Nodo<T>* borrar_last_rec(Nodo<T>* n) {
    if (n == NULL) return NULL;
    if (n->get_next() == NULL) {
        delete n;
        return NULL;
    }
    n->set_next(borrar_last_rec(n->get_next()));
    return n;
}

Nodo<T>* duplicar_nodos_rec(Nodo<T>* n) {
    if (n == NULL) return NULL;
    Nodo<T>* nuevo = new Nodo<T>(n->get_dato());
    nuevo->set_next(duplicar_nodos_rec(n->get_next()));
    return nuevo;
}



public:
    // Constructores 
    Lista() { czo = NULL; }
    Lista(Nodo<T>* n) { czo = n; }

    // Destructor
    ~Lista();

    // Regla de los Tres (Rule of Three)
    Lista(Lista<T>& otra);
    Lista<T>& operator=(Lista<T> otra);
    void swap(Lista<T>& otra);

    // Métodos de la lista
    void add(T d);
    bool esvacia();
    T cabeza();
    Nodo<T>* resto();
    int size();
    void toPrint(string p);
    T suma(T i);
    //ejercicios
    void borrar(void); //borra la cabeza
    void borrar_last();//borra el ultimo
    void concat(Lista<T>* l1);// le transfiere los datos de l1 a this
    Lista<T>* copy(void);// hace una copia de la lista
    void tomar(int n);//deja "vivos" los n primeros nodos y borra el resto
    
};

// ==========================================
// Implementación de Lista
// ==========================================

// Destructor
template <class T>
Lista<T>::~Lista() {
    while (czo != NULL) {
        Nodo<T>* temp = czo;
        czo = czo->get_next();
        delete temp;
    }
}

// Agregar nodo al frente
template <class T>
void Lista<T>::add(T d) {
    Nodo<T>* nuevo = new Nodo<T>(d);
    nuevo->set_next(czo);
    czo = nuevo;
}

// Controla si está vacía
template <class T>
bool Lista<T>::esvacia() {
    return czo == NULL;
}

// Obtener dato de la cabeza
template <class T>
T Lista<T>::cabeza() {
    if (this->esvacia()) {
        cout << "Error: Lista vacia" << endl;
        exit(1);
    }
    return czo->get_dato();
}

// Puntero al siguiente nodo
template <class T>
Nodo<T>* Lista<T>::resto() {
    if (czo == NULL) return NULL;
	return czo->get_next();
}

// Tamaño de la lista
template <class T>
int Lista<T>::size() {
    return size_rec(czo);
}

// Muestra la lista
template <class T>
void Lista<T>::toPrint(string p) {
    toPrint_rec(czo);
    cout<<p<<endl;
}

// Suma recursiva
template <class T>
T Lista<T>::suma(T i) {
    return suma_rec(czo, i);
}


template <class T> void Lista<T>::borrar(void)
{ //borra el nodo cabeza
    if (!this->esvacia()) {
        Nodo<T>* tmp = czo;
        czo = czo->get_next();
        delete tmp;
    }
}

// borra el ultimo nodo
template <class T> void Lista<T>::borrar_last() {
    czo = borrar_last_rec(czo);
}

template <class T>
void Lista<T>::concat(Lista<T>* l1) {
    if (l1 == NULL || l1->esvacia()) return;

    // Si la lista actual está vacía, le asignamos la copia de la otra
    if (this->esvacia()) {
        this->czo = duplicar_nodos_rec(l1->czo);
        return;
    }

    // Si no está vacía, navegamos hasta el final y enganchamos la copia
    Nodo<T>* aux = this->czo;
    while (aux->get_next() != NULL) {
        aux = aux->get_next();
    }
    aux->set_next(duplicar_nodos_rec(l1->czo));
}

template <class T> Lista<T>* Lista<T>::copy(void)
{
    Lista<T>* aux = new Lista();
    aux->concat(this);
    return aux;
}

template <class T> void Lista<T>::tomar(int n)
{ //deja "vivos" los n primeros nodos y borra el resto
    if (this->size() > n) {
        this->borrar_last();
        this->tomar(n);
    }
}

template <class T> class Pila:public Lista<T>{
      public:
             Pila(){};
             void apilar(T d){this->add(d);};
             T tope(void){return this->cabeza();};
             void desapilar(void){this->borrar();};
             bool pilavacia(){return this->esvacia();};
};                  


int main()
{
  Pila<string> *p= new Pila<string>();
  p->apilar("10");  p->apilar("11");
  p->apilar("12");  p->apilar("13");
  if (!p->pilavacia()) cout<<p->tope()<<endl;
  if (!p->pilavacia())p->desapilar();
  if (!p->pilavacia())cout<<p->tope()<<endl;
  if (!p->pilavacia())p->desapilar();
  if (!p->pilavacia())cout<<p->tope()<<endl;
  
// ingresar una cadena que contendra ([{ y )]}
// el programa debera determinar si el "balance" de los ([{ es correcto
// errores posibles
// hay mas ((( que )) o [[[ que ] o { que }}}
// que no se corresponda un cerrado con el ultimo abierto {  ]

/*
  string cadena;
  cout<<endl<<"Ingrese cadena= "; cin>>cadena;
  int error=0;
  for(int i=0;(i<cadena.length());i++){
          cout<<cadena.at(i)<<endl;
  
  
  }
*/



    cout << endl << endl;
    system("PAUSE");
    return EXIT_SUCCESS;

}

