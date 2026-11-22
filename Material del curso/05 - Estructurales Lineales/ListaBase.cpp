/******************************************************************************
               Online C++ Compiler.
   Code, Compile, Run and Debug C++ program online.
*******************************************************************************/

#include <cstdlib> // Para exit() y system()
#include <iostream>
#include <string>
#include <utility> // Para std::swap

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
    Nodo(T a) 
    { dato = a; 
      next = NULL; 
        
    }

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

    void toPrint_rec(Nodo<T>* n, string p, int pos) {
        if (n == NULL) return;
        cout << p << pos << " "<< n->get_dato() << endl;
        toPrint_rec(n->get_next(), p, pos +1);
    }
    
    void toPrint_norec( string p) {
        Nodo<T>* n = czo;
        int pos = 0;
        while (n != NULL)
        {
            cout << p << pos << " "<< n->get_dato() << endl;
            n = n->get_next();
            ++pos;
        }
        
    }

    T suma_rec(Nodo<T>* n, T i) {
        if (n == NULL) return i;
        return n->get_dato() + suma_rec(n->get_next(), i);
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

// Constructor de Copia
template <class T>
Lista<T>::Lista(Lista<T>& otra) {
    if (otra.czo == NULL) {
        czo = NULL;
        return;
    }

    czo = new Nodo<T>(otra.czo->get_dato());
    Nodo<T>* actual_nuevo = czo;
    Nodo<T>* actual_otro = otra.czo->get_next();

    while (actual_otro != NULL) {
        Nodo<T>* nuevo_nodo = new Nodo<T>(actual_otro->get_dato());
        actual_nuevo->set_next(nuevo_nodo);
        actual_nuevo = nuevo_nodo;
        actual_otro = actual_otro->get_next();
    }
}

// Swap auxiliar
template <class T>
void Lista<T>::swap(Lista<T>& otra) {
    std::swap(this->czo, otra.czo);
}

// Operador de Asignación
template <class T>
Lista<T>& Lista<T>::operator=(Lista<T> otra) {
    this->swap(otra);
    return *this;
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
    if (czo != NULL)
    {
        return czo->get_next();
    }
    else
    {
        return NULL;
    }
    
}

// Tamaño de la lista
template <class T>
int Lista<T>::size() {
    return size_rec(czo);
}

// Muestra la lista
template <class T>
void Lista<T>::toPrint(string p) {
    //toPrint_rec(czo, p, 1);
    toPrint_norec( p);
}

// Suma recursiva
template <class T>
T Lista<T>::suma(T i) {
    return suma_rec(czo, i);
}

// ==========================================
// Función Principal
// ==========================================
int main() {
    Lista<string>* l = new Lista<string>();
    Lista<int>* r = new Lista<int>();
    
    l->add(" 11x");
    l->add(" 22y");
    l->add(" 33z");
    cout << "Cabeza " << l->cabeza() << endl;
    r->add(100);
    r->add(22);
    
    l->toPrint("Este nodo vale ");
    cout << endl;
    //cout << "suma l:" << l->suma(0) << endl;
    r->toPrint("");
    cout << endl;
    
    cout << "suma:" << r->suma(0) << endl;
    
    cout << " size l=" << l->size() << endl;
    delete l;
    delete r;

    system("PAUSE");
    return EXIT_SUCCESS;
}
