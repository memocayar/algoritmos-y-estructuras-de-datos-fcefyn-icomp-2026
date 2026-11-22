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
    Nodo<T>* next;

public:
    Nodo(T a) { 
        dato = a; 
        next = NULL; 
    }

    void set_dato(T a) { dato = a; }
    void set_next(Nodo<T>* n) { next = n; }
    T get_dato() { return dato; }
    Nodo<T>* get_next() { return next; }
};

// ==========================================
// Clase Lista
// ==========================================
template <class T>
class Lista {
protected:
    Nodo<T>* czo;

    int size_rec(Nodo<T>* n) {
        if (n == NULL) return 0;
        return 1 + size_rec(n->get_next());
    }

    void toPrint_rec(Nodo<T>* n) {
        if (n == NULL) return;
        cout << n->get_dato() << " ";
        toPrint_rec(n->get_next());
    }

    T suma_rec(Nodo<T>* n, T i) {
        if (n == NULL) return i;
        return n->get_dato() + suma_rec(n->get_next(), i);
    }
    
    T last_rec(Nodo<T>* n) {
        if (n->get_next() == NULL) {
            return n->get_dato();
        }
        return last_rec(n->get_next());
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
    Lista() { czo = NULL; }
    Lista(Nodo<T>* n) { czo = n; }
    virtual ~Lista();

    void add(T d);
    bool esvacia();
    T cabeza();
    T last();
    int size();
    void toPrint(string p);
    T suma(T i);

    void borrar(void);
    void borrar_last();
    void concat(Lista<T>* l1);
    Lista<T>* copy(void);
    void tomar(int n);
};

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

template <class T>
bool Lista<T>::esvacia() {
    return czo == NULL;
}

template <class T>
T Lista<T>::cabeza() {
    if (this->esvacia()) {
        cout << "Error: Lista vacia" << endl;
        exit(1);
    }
    return czo->get_dato();
}

template <class T>
int Lista<T>::size() {
    return size_rec(czo);
}

template <class T>
void Lista<T>::toPrint(string p) {
    toPrint_rec(czo);
    cout << p << endl;
}

template <class T>
T Lista<T>::suma(T i) {
    return suma_rec(czo, i);
}

template <class T>
void Lista<T>::borrar(void) {
    if (!this->esvacia()) {
        Nodo<T>* tmp = czo;
        czo = czo->get_next();
        delete tmp;
    }
}

template <class T>
void Lista<T>::borrar_last() {
    czo = borrar_last_rec(czo);
}

template <class T>
T Lista<T>::last() {
    if (this->esvacia()) {
        cout << "Error: Lista vacia" << endl;
        exit(1);
    }
    return last_rec(czo);
}

template <class T>
void Lista<T>::concat(Lista<T>* l1) {
    if (l1 == NULL || l1->esvacia()) return;

    if (this->esvacia()) {
        this->czo = duplicar_nodos_rec(l1->czo);
        return;
    }

    Nodo<T>* aux = this->czo;
    while (aux->get_next() != NULL) {
        aux = aux->get_next();
    }
    aux->set_next(duplicar_nodos_rec(l1->czo));
}

template <class T>
Lista<T>* Lista<T>::copy(void) {
    Lista<T>* aux = new Lista<T>();
    aux->concat(this);
    return aux;
}

template <class T>
void Lista<T>::tomar(int n) {
    if (this->size() > n) {
        this->borrar_last();
        this->tomar(n);
    }
}

// ==========================================
// Clase Cola
// ==========================================
template <class T>
class Cola : public Lista<T> {
public:
    Cola(void) {}

    bool colavacia(void) { 
        return this->esvacia(); 
    }

    // Ingresa por el frente (czo)
    void encolar(T a) { 
        this->add(a); 
    }

    // El elemento listo para salir es el mas viejo (el ultimo nodo)
    T tope() { 
        return this->last(); 
    }

    // Desencola eliminando el mas viejo
    void desencolar(void) { 
        this->borrar_last(); 
    }

    // Retorna el ultimo que ingreso a la cola
    T ultimo(void) { 
        return this->cabeza(); 
    }
};

// ==========================================
// Main
// ==========================================
int main() {
    Cola<int>* c = new Cola<int>();

    if (c->colavacia()) {
        cout << "La cola esta VACIA" << endl << endl;
    }

    c->encolar(15);
    c->encolar(2);
    c->encolar(3);

    c->toPrint(" >>>");
    cout << "en cola: " << c->tope() << endl;
    cout << "ultimo ingresado = " << c->ultimo() << endl;
    cout << "------------------------" << endl;

    if (!c->colavacia()) c->desencolar();
    cout << "en cola: " << c->tope() << endl;
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    if (!c->colavacia()) c->desencolar();
    cout << "en cola: " << c->tope() << endl;
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    // Desencola el ultimo elemento restante (15)
    if (!c->colavacia()) c->desencolar();

    // Comprobamos si esta vacia antes de consultar tope
    if (!c->colavacia()) {
        cout << "en cola: " << c->tope() << endl;
    } else {
        cout << "La cola quedo vacia." << endl;
    }
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    c->encolar(44);
    cout << "en cola: " << c->tope() << endl;
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    if (!c->colavacia()) c->desencolar();
    c->toPrint(" >>>");

    cout << endl;
    delete c;

    return 0;
}
