#include <iostream>
#include <cstdlib>

using namespace std;

template <class T>
class nodo {
public:
    T info;
    nodo* der;
    nodo* izq;
    nodo(T val) : info(val), der(nullptr), izq(nullptr) {}
};

template <class T>
class arbol {
private:
    nodo<T>* raiz;
    nodo<T>* q; // Puntero auxiliar utilizado por el algoritmo de borrado

    void ArbolBusq(T x, nodo<T>*& nuevo);
    void rid(nodo<T>* aux);
    void ird(nodo<T>* aux);
    void idr(nodo<T>* aux);
    void show(nodo<T>* aux, int n);
    void destruir(nodo<T>* aux);

    // Métodos auxiliares
    void mostrarHojas(nodo<T>* aux);
    bool esta(nodo<T>* aux, T x);
    void borrar(nodo<T>*& p, T x);
    void bor(nodo<T>*& d);

public:
    arbol() : raiz(nullptr), q(nullptr) {}
    ~arbol() { destruir(raiz); }

    // Evita copias superficiales (shallow copy) que romperían la memoria
    arbol(const arbol&) = delete;
    arbol& operator=(const arbol&) = delete;

    void CreaArbolBus(T x) { ArbolBusq(x, raiz); }
    void RID() { rid(raiz); }
    void IRD() { ird(raiz); }
    void IDR() { idr(raiz); }
    void VerArbol() { show(raiz, 0); }

    void MostrarHojas() { mostrarHojas(raiz); cout << endl; }
    bool Esta(T x) { return esta(raiz, x); }

    T Menor() {
        if (raiz == nullptr) throw runtime_error("El arbol esta vacio");
        nodo<T>* aux = raiz;
        while (aux->izq != nullptr) aux = aux->izq;
        return aux->info;
    }

    T Mayor() {
        if (raiz == nullptr) throw runtime_error("El arbol esta vacio");
        nodo<T>* aux = raiz;
        while (aux->der != nullptr) aux = aux->der;
        return aux->info;
    }

    void Borrar(T x) { borrar(raiz, x); }
};

// --- Destructor recursivo (elimina memory leaks) ---
template <class T>
void arbol<T>::destruir(nodo<T>* aux) {
    if (aux != nullptr) {
        destruir(aux->izq);
        destruir(aux->der);
        delete aux;
    }
}

// --- Inserción ---
template <class T>
void arbol<T>::ArbolBusq(T x, nodo<T>*& nuevo) {
    if (nuevo == nullptr) {
        nuevo = new nodo<T>(x);
    } else if (x > nuevo->info) {
        ArbolBusq(x, nuevo->der);
    } else if (x < nuevo->info) {
        ArbolBusq(x, nuevo->izq);
    }
}

// --- Recorridos ---
template <class T>
void arbol<T>::ird(nodo<T>* aux) {
    if (aux != nullptr) {
        ird(aux->izq);
        cout << aux->info << " ";
        ird(aux->der);
    }
}

template <class T>
void arbol<T>::rid(nodo<T>* aux) {
    if (aux != nullptr) {
        cout << aux->info << " ";
        rid(aux->izq);
        rid(aux->der);
    }
}

template <class T>
void arbol<T>::idr(nodo<T>* aux) {
    if (aux != nullptr) {
        idr(aux->izq);
        idr(aux->der);
        cout << aux->info << " ";
    }
}

template <class T>
void arbol<T>::show(nodo<T>* aux, int n) {
    if (aux != nullptr) {
        show(aux->der, n + 1);
        for (int i = 1; i <= n; i++) cout << "     ";
        cout << aux->info << "\n";
        show(aux->izq, n + 1);
    }
}

// --- Métodos adicionales ---
template <class T>
void arbol<T>::mostrarHojas(nodo<T>* aux) {
    if (aux != nullptr) {
        if (aux->izq == nullptr && aux->der == nullptr) {
            cout << aux->info << " ";
        }
        mostrarHojas(aux->izq);
        mostrarHojas(aux->der);
    }
}

template <class T>
bool arbol<T>::esta(nodo<T>* aux, T x) {
    if (aux == nullptr) return false;
    if (aux->info == x) return true;
    if (x < aux->info) return esta(aux->izq, x);
    return esta(aux->der, x);
}

// --- Eliminación solicitada ---
template <class T>
void arbol<T>::borrar(nodo<T>*& p, T x) {
    if (p == nullptr) {
        cout << "\n El dato NO esta\n\n";
    } else if (x > p->info) {
        borrar(p->der, x);
    } else if (x < p->info) {
        borrar(p->izq, x);
    } else {
        q = p;
        if (q->der == nullptr) {
            p = q->izq;
        } else if (q->izq == nullptr) {
            p = q->der;
        } else {
            bor(q->izq);
        }
        delete q;
    }
}

template <class T>
void arbol<T>::bor(nodo<T>*& d) {
    if (d->der != nullptr) {
        bor(d->der);
    } else {
        q->info = d->info;
        q = d;
        d = d->izq;
    }
}

int main() {
    try {
        arbol<int> A;
        int vec[10] = { 50, 12, 18, 240, 222, 45, 410, 325, 38, 95 };

        for (int i = 0; i < 10; i++) {
            A.CreaArbolBus(vec[i]);
        }

        cout << "Estructura inicial del arbol:\n";
        A.VerArbol();

        cout << "\nIRD (Inorden): ";
        A.IRD();
        cout << "\nRID (Preorden): ";
        A.RID();
        cout << "\nIDR (Postorden): ";
        A.IDR();

        cout << "\n\nMostrar Hojas: ";
        A.MostrarHojas();

        cout << "El menor del arbol = " << A.Menor() << endl;
        cout << "El mayor del arbol = " << A.Mayor() << endl;

        cout << "\nBusqueda de 95: " << (A.Esta(95) ? "Existe" : "No existe") << endl;
        cout << "Busqueda de 100: " << (A.Esta(100) ? "Existe" : "No existe") << endl;

        // Caso 1: Borrar una hoja (0 hijos)
        cout << "\n--- Borrando nodo 38 (hoja, 0 hijos) ---" << endl;
        A.Borrar(38);
        A.VerArbol();

        // Caso 2: Borrar nodo con 1 hijo
        cout << "\n--- Borrando nodo 12 (1 solo hijo: 18) ---" << endl;
        A.Borrar(12);
        A.VerArbol();

        // Caso 3: Borrar nodo con 2 hijos
        cout << "\n--- Borrando nodo 240 (2 hijos: 222 y 410) ---" << endl;
        A.Borrar(240);
        A.VerArbol();

        // Caso 4: Borrar la raíz
        cout << "\n--- Borrando la raiz 50 ---" << endl;
        A.Borrar(50);
        A.VerArbol();

        // Caso 5: Intentar borrar un dato inexistente
        cout << "\n--- Intentando borrar dato inexistente (999) ---" << endl;
        A.Borrar(999);

    } catch (const exception& e) {
        cerr << "Error en la ejecucion: " << e.what() << endl;
    }

    return 0;
}
