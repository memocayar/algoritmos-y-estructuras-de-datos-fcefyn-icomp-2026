
#include <iostream>
#include <set>

using namespace std;

template <class T>
class Conjunto {
private:
    set<T> datos; // Contenedor interno STL

public:
    // -------------------------------------------------------------
    // Constructores
    // -------------------------------------------------------------
    Conjunto() = default;

    // -------------------------------------------------------------
    // Métodos delegados de la STL (Capacidad y Modificadores)
    // -------------------------------------------------------------
    void insertar(const T& elemento) {
        datos.insert(elemento); // Método insert
    }

    void eliminar(const T& elemento) {
        datos.erase(elemento); // Método erase
    }

    void vaciar() {
        datos.clear(); // Método clear
    }

    bool estaVacio() const {
        return datos.empty(); // Método empty
    }

    size_t tamano() const {
        return datos.size(); // Método size
    }

    // -------------------------------------------------------------
    // Métodos de Búsqueda (Operations)
    // -------------------------------------------------------------
    bool contiene(const T& elemento) const {
        return datos.count(elemento) > 0; // Método count
    }

    // -------------------------------------------------------------
    // OPERACIONES DE CONJUNTOS
    // -------------------------------------------------------------

    // 1. UNIÓN: A u B
    Conjunto<T> obtenerUnion(const Conjunto<T>& otro) const {
        Conjunto<T> resultado;

        for (const T& elem : this->datos) {
            resultado.insertar(elem);
        }

        for (const T& elem : otro.datos) {
            resultado.insertar(elem);
        }

        return resultado;
    }

    // 2. INTERSECCIÓN: A ∩ B
    Conjunto<T> obtenerInterseccion(const Conjunto<T>& otro) const {
        Conjunto<T> resultado;

        for (const T& elem : this->datos) {
            if (otro.contiene(elem)) { 
                resultado.insertar(elem);
            }
        }

        return resultado;
    }

    // 3. DIFERENCIA: A - B
    Conjunto<T> obtenerDiferencia(const Conjunto<T>& otro) const {
        Conjunto<T> resultado;

        for (const T& elem : this->datos) {
            if (!otro.contiene(elem)) {
                resultado.insertar(elem);
            }
        }

        return resultado;
    }
	
	void toPrint(){
		for (T x: this->datos) cout<< x<<"  ";
		cout<<endl;
	}
    // -------------------------------------------------------------
    // Sobrecarga opcional de operadores para sintaxis natural
    // -------------------------------------------------------------
    Conjunto<T> operator+(const Conjunto<T>& otro) const { return obtenerUnion(otro); }
    Conjunto<T> operator*(const Conjunto<T>& otro) const { return obtenerInterseccion(otro); }
    Conjunto<T> operator-(const Conjunto<T>& otro) const { return obtenerDiferencia(otro); }

    // Función amiga para imprimir por consola
    friend ostream& operator<<(ostream& os, const Conjunto<T>& c) {
        os << "{ ";
        for (auto it = c.datos.begin(); it != c.datos.end(); ++it) {
            os << *it << (next(it) != c.datos.end() ? ", " : " ");
        }
        os << "}";
        return os;
    }
};

int main() {
    Conjunto<int> A;
    Conjunto<int> B;

    // Poblando el conjunto A = {1, 2, 3, 4, 5}
    A.insertar(1);
    A.insertar(2);
    A.insertar(3);
    A.insertar(4);
    A.insertar(5);
    cout<<"conjunto A: ";
    A.toPrint();

    // Poblando el conjunto B = {4, 5, 6, 7, 8}
    B.insertar(4);
    B.insertar(5);
    B.insertar(6);
    B.insertar(7);
    B.insertar(8);
	cout<<"conjunto B: ";
    B.toPrint();
    
    cout << "Conjunto A: " << A << "\n";
    cout << "Conjunto B: " << B << "\n\n";

    // 1. Unión
    Conjunto<int> unionAB = A.obtenerUnion(B);
    cout << "Union (A u B): " << unionAB << "\n";

    // 2. Intersección
    Conjunto<int> interseccionAB = A.obtenerInterseccion(B);
    cout << "Interseccion (A ∩ B): " << interseccionAB << "\n";

    // 3. Diferencia
    Conjunto<int> diferenciaAB = A.obtenerDiferencia(B);
    cout << "Diferencia (A - B): " << diferenciaAB << "\n";

    // Uso mediante operadores sobrecargados (+, *, -)
    cout << "\n--- Probando Operadores ---\n";
    cout << "A + B: " << (A + B) << "\n";
    cout << "A * B: " << (A * B) << "\n";
    cout << "A - B: " << (A - B) << "\n";

    return 0;
}

