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

    void ArbolBusq(T x, nodo<T>*& nuevo);
    void rid(nodo<T>* aux);
    void ird(nodo<T>* aux);
    void idr(nodo<T>* aux);
    void show(nodo<T>* aux, int n);
    void destruir(nodo<T>* aux);
    void mostrarHojas(nodo<T>* aux);
    bool esta(nodo<T>* aux, T x);
    nodo<T>* borrar(nodo<T>* aux, T x);
    nodo<T>* buscarMin(nodo<T>* aux);

public:
    arbol() : raiz(nullptr) {}
    ~arbol() { destruir(raiz); }

    // Deshabilitar copias directas para evitar shallow copy
    arbol(const arbol&) = delete;
    arbol& operator=(const arbol&) = delete;

    void CreaArbolBus(T x) { ArbolBusq(x, raiz); }
    void RID() { rid(raiz); }
    void IRD() { ird(raiz); }
    void IDR() { idr(raiz); }
    void VerArbol() { show(raiz, 0); }

    void MostrarHojas() { mostrarHojas(raiz); }
    bool Esta(T x) { return esta(raiz, x); }
    
    T Menor() {
        if (!raiz) throw runtime_error("El arbol esta vacio");
        nodo<T>* aux = raiz;
        while (aux->izq != nullptr) aux = aux->izq;
        return aux->info;
    }

    T Mayor() {
        if (!raiz) throw runtime_error("El arbol esta vacio");
        nodo<T>* aux = raiz;
        while (aux->der != nullptr) aux = aux->der;
        return aux->info;
    }

    void Borrar(T x) { raiz = borrar(raiz, x); }
};

template <class T>
void arbol<T>::destruir(nodo<T>* aux) {
    if (aux != nullptr) {
        destruir(aux->izq);
        destruir(aux->der);
        delete aux;
    }
}

template <class T>
void arbol<T>::ArbolBusq(T x, nodo<T>*& nuevo) {
    if (nuevo == nullptr) {
        nuevo = new nodo<T>(x);
    } else if (x > nuevo->info) {
        ArbolBusq(x, nuevo->der);
    } else if (x < nuevo->info) {
        ArbolBusq(x, nuevo->izq);
    }
    // Si x == nuevo->info se ignora (no duplicados)
}

template <class T>
void arbol<T>::ird(nodo<T>* aux) {
    if (aux != nullptr) {
        ird(aux->izq);
        cout << "\n" << aux->info;
        ird(aux->der);
    }
}

template <class T>
void arbol<T>::rid(nodo<T>* aux) {
    if (aux != nullptr) {
        cout << "\n" << aux->info;
        rid(aux->izq);
        rid(aux->der);
    }
}

template <class T>
void arbol<T>::idr(nodo<T>* aux) {
    if (aux != nullptr) {
        idr(aux->izq);
        idr(aux->der);
        cout << "\n" << aux->info;
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

template <class T>
nodo<T>* arbol<T>::buscarMin(nodo<T>* aux) {
    while (aux->izq != nullptr) aux = aux->izq;
    return aux;
}


int main(int argc, char* argv[])
{
int i,x;
arbol<int> A;
int vec[10] = { 50,12,18,240,222,45,410,325,38,95 };
//int vec[10]={50,120,180,240,252,450,460,475,488,595};
for (i = 0; i < 10; i++) {
A.CreaArbolBus(vec[i]);
//T.VerArbol();
//system("PAUSE");
}
A.VerArbol();
cout << "\n IRD--------------------------------";
A.IRD();
cout << "\n RID--------------------------------";
A.RID();
cout<<"\n IDR--------------------------------";
A.IDR();

cout << endl << endl;

system("PAUSE");

return EXIT_SUCCESS;

}



/*
Realizar los siguientes metodos de la clase arbol
void MostrarHojas() //muestra todas las hojas del arbol
bool Esta(x) //retorna true si x esta en el arbol
T Menor() // retorna el menor valor del arbol
T Mayor() // retorna el mayor valor del arbol
*/ 


