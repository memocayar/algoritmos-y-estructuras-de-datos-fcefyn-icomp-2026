#include <iostream>
using namespace std;

class Base {
public:
    int x = 1;
protected:
    int y = 2;
private:
    int z = 3;
public:
	void mostrar() {
		cout<<x<<"  "<<y<<"  "<<z<<endl;
	} 
	void setZ(int valor){
		z=valor;
	}  
};


// 1. Herencia Pública
class DerivadaPublica : public Base {
  public:
    void prueba() {
        x = 10; // OK: es public
        y = 20; // OK: es protected
        // z = 30; // Error: privado en la Base
        //setZ(30);
    }
	void mostrar() {
		cout<<x<<"  "<<y<<"  "<<"z"<<endl;
	}   
};

// 2. Herencia Protegida
class DerivadaProtegida : protected Base {
  public:
    void prueba() {
        x = 15; // OK: ahora x se volvió protected aquí
        y = 25; // OK: sigue siendo protected
    }
    void mostrar() {
		cout<<x<<"  "<<y<<"  "<<"z"<<endl;
	}   
};

// 3. Herencia Privada
class DerivadaPrivada : private Base {
  public:
    void prueba() {
        x = 11; // OK: pero x pasa a ser private para el resto del mundo
        y = 22; // OK: y pasa a ser private
    }
    void mostrar() {
		cout<<x<<"  "<<y<<"  "<<"z"<<endl;
	}   
};

int main() {
	Base A;
	cout<<A.x<<endl;
	A.x=15;
	cout<<A.x<<endl;
	A.mostrar();
	
	Base B;
	cout<<B.x<<endl;
	cout<<"---------------------------"<<endl;
	
	
    DerivadaPublica d1;
    d1.x = 100; // OK: 'x' sigue siendo público desde fuera
    cout<<d1.x<<endl;
    d1.prueba();
    cout<<d1.x<<endl;
    d1.mostrar();
    d1.Base::mostrar();
    cout<<"---------------------------"<<endl;
    
    DerivadaProtegida d2;
    //d2.x = 100; // Error: 'x' ahora es protegido, inaccesible desde main()
    //cout<<d2.x<<endl;
    d2.prueba();
    //cout<<d2.x<<endl;
    d2.mostrar();
    //d2.Base::mostrar(); // no se puede acceder
    
	cout<<"---------------------------"<<endl;
	
    DerivadaPrivada d3;
    // d3.x = 100; // Error: 'x' ahora es privado, inaccesible desde main()
    d3.prueba();
    d3.mostrar();
    
}