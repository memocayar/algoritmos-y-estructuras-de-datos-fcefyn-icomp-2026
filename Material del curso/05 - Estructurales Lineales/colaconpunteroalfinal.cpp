#include <cstdlib>
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

public:
    Lista() { czo = NULL; }
    virtual ~Lista();

    void add(T d);
    bool esvacia();
    T cabeza();
    int size();
    void toPrint(string p);
    void borrar(void);
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
void Lista<T>::borrar(void) {
    if (!this->esvacia()) {
        Nodo<T>* tmp = czo;
        czo = czo->get_next();
        delete tmp;
    }
}

// ==========================================
// Clase Cola con puntero al ultimo nodo
// ==========================================
template <class T>
class Cola : public Lista<T> {
private:
    Nodo<T>* ultimo_nodo; // Puntero al final de la cola para O(1)

public:
    Cola(void) : Lista<T>() {
        ultimo_nodo = NULL;
    }

    bool colavacia(void) { 
        return this->esvacia(); 
    }

    // Encolar al final en O(1)
    void encolar(T a) {
        Nodo<T>* nuevo = new Nodo<T>(a);

        if (this->colavacia()) {
            this->czo = nuevo;
            ultimo_nodo = nuevo;
        } else {
            ultimo_nodo->set_next(nuevo);
            ultimo_nodo = nuevo;
        }
    }

    // Desencolar del frente en O(1)
    void desencolar(void) {
        if (this->colavacia()) {
            cout << "Error: Cola vacia" << endl;
            return;
        }

        // Se elimina la cabeza de la lista
        this->borrar();

        // Si la cola quedó vacía, se actualiza el puntero del final
        if (this->czo == NULL) {
            ultimo_nodo = NULL;
        }
    }

    // El elemento listo para salir (el frente / mas antiguo)
    T tope() {
        if (this->colavacia()) {
            cout << "Error: Cola vacia" << endl;
            exit(1);
        }
        return this->cabeza();
    }

    // El ultimo elemento que ingreso
    T ultimo(void) {
        if (this->colavacia()) {
            cout << "Error: Cola vacia" << endl;
            exit(1);
        }
        return ultimo_nodo->get_dato();
    }
};

// ==========================================
// Main
// ==========================================
int main() {
    Cola<int>* c = new Cola<int>();

    if (c->colavacia()) cout << "La cola esta VACIA" << endl << endl;

    c->encolar(15);
    c->encolar(2);
    c->encolar(3);

    c->toPrint(" >>>");
    cout << "en cola (sale primero): " << c->tope() << endl;
    cout << "ultimo ingresado: " << c->ultimo() << endl;
    cout << "------------------------" << endl;

    if (!c->colavacia()) c->desencolar();
    cout << "en cola: " << c->tope() << endl;
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    if (!c->colavacia()) c->desencolar();
    cout << "en cola: " << c->tope() << endl;
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    // Desencola el ultimo restante (3)
    if (!c->colavacia()) c->desencolar();

    if (!c->colavacia()) {
        cout << "en cola: " << c->tope() << endl;
    } else {
        cout << "La cola quedo vacia." << endl;
    }
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    c->encolar(44);
    cout << "en cola: " << c->tope() << endl;
    cout << "ultimo ingresado: " << c->ultimo() << endl;
    c->toPrint(" >>>");
    cout << "------------------------" << endl;

    if (!c->colavacia()) c->desencolar();
    c->toPrint(" >>>");

    cout << endl;
    delete c;

    return 0;
}
