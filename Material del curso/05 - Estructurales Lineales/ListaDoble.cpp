#include <iostream>
#include <stdexcept>

class ListaDoble {
private:
    struct Nodo {
        int dato;
        Nodo* ant;
        Nodo* sig;
        

        Nodo(int val) 
        {
            dato = val;
            ant = nullptr;
            sig = nullptr;
            
        }
        //: dato(val), ant(nullptr), sig(nullptr) {}
    };

    Nodo* cabeza;
    Nodo* cola;
    size_t longitud;

public:
    ListaDoble()  {
        cabeza=nullptr;
        cola=nullptr;
        longitud = 0;
    }

    ~ListaDoble() {
        limpiar();
    }

    // Regla de los tres: deshabilitamos copia para evitar double free simple
    ListaDoble(const ListaDoble&) = delete;
    ListaDoble& operator=(const ListaDoble&) = delete;

    bool estaVacia() const {
        return cabeza == nullptr;
    }

    size_t tamano() const {
        return longitud;
    }

    // Inserción al frente: O(1)
    void insertarInicio(int val) {
        Nodo* nuevo = new Nodo(val);
        if (estaVacia()) {
            cabeza = cola = nuevo;
        } else {
            nuevo->sig = cabeza;
            cabeza->ant = nuevo;
            cabeza = nuevo;
        }
        longitud++;
    }

    // Inserción al final: O(1)
    void insertarFinal(int val) {
        Nodo* nuevo = new Nodo(val);
        if (estaVacia()) {
            cabeza = cola = nuevo;
        } else {
            nuevo->ant = cola;
            cola->sig = nuevo;
            cola = nuevo;
        }
        longitud++;
    }

    // Eliminación al frente: O(1)
    void eliminarInicio() {
        if (estaVacia()) {
            throw std::runtime_error("La lista esta vacia");
        }

        Nodo* temp = cabeza;
        if (cabeza == cola) {
            cabeza = cola = nullptr;
        } else {
            cabeza = cabeza->sig;
            cabeza->ant = nullptr;
        }
        delete temp;
        longitud--;
    }

    // Eliminación al final: O(1)
    void eliminarFinal() {
        if (estaVacia()) {
            throw std::runtime_error("La lista esta vacia");
        }

        Nodo* temp = cola;
        if (cabeza == cola) {
            cabeza = cola = nullptr;
        } else {
            cola = cola->ant;
            cola->sig = nullptr;
        }
        delete temp;
        longitud--;
    }

    // Eliminación de la primera aparición de un valor: O(N)
    bool eliminarValor(int val) {
        Nodo* actual = cabeza;

        while (actual != nullptr && actual->dato != val) {
            actual = actual->sig;
        }

        if (actual == nullptr) return false; // No encontrado

        if (actual == cabeza) {
            eliminarInicio();
        } else if (actual == cola) {
            eliminarFinal();
        } else {
            actual->ant->sig = actual->sig;
            actual->sig->ant = actual->ant;
            delete actual;
            longitud--;
        }
        return true;
    }

    // Recorrido hacia adelante: O(N)
    void imprimirAdelante() const {
        Nodo* actual = cabeza;
        std::cout << "[ ";
        while (actual != nullptr) {
            std::cout << actual->dato << " ";
            actual = actual->sig;
        }
        std::cout << "]\n";
    }

    // Recorrido hacia atrás: O(N)
    void imprimirAtras() const {
        Nodo* actual = cola;
        std::cout << "[ ";
        while (actual != nullptr) {
            std::cout << actual->dato << " ";
            actual = actual->ant;
        }
        std::cout << "]\n";
    }

    // Liberar todos los nodos
    void limpiar() {
        while (!estaVacia()) {
            eliminarInicio();
        }
    }
};

int main() {
    ListaDoble lista;

    // Inserciones
    lista.insertarFinal(10);
    lista.insertarFinal(20);
    lista.insertarFinal(30);
    lista.insertarInicio(5);

    std::cout << "Recorrido de inicio a fin: ";
    lista.imprimirAdelante(); // Salida: [ 5 10 20 30 ]

    std::cout << "Recorrido de fin a inicio: ";
    lista.imprimirAtras();    // Salida: [ 30 20 10 5 ]

    // Eliminaciones
    lista.eliminarInicio();   // Quita 5
    lista.eliminarFinal();    // Quita 30
    lista.eliminarValor(20);  // Quita nodo intermedio 20

    std::cout << "Despues de eliminar 5, 30 y 20: ";
    lista.imprimirAdelante(); // Salida: [ 10 ]

    return 0;
}
