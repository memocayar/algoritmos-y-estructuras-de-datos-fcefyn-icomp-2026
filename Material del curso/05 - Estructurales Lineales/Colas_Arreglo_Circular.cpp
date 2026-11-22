// Cola Circular con Arreglo.cpp : 
#include <cstdlib>
#include <iostream>
#include <sstream>
#define MAX 4

using namespace std;
// almacena MAX-1 elementos
// cola circular
template <class T> class Cola {
private:
    T Q[MAX];
    int head, tail;
public:
    Cola(void) { head = 0; tail = MAX - 1; };
    ~Cola(void);
    T tope() { return Q[head]; };
    bool colavacia() { return head == (tail + 1) % MAX; };
    bool colallena() { return head == (tail + 2) % MAX; };
    void encolar(T a);
    void desencolar();
    T ultimo();
    string imprimir(string s);
};
//-------- Metodos de Cola --------------------
template <class T> void Cola<T>::encolar(T a)
{
    if (!colallena()) {
        //if (tail == MAX - 2) tail = MAX - 1;
        tail = (tail + 1) % MAX;
        Q[tail] = a;
    }
    else cout << "Cola llena" << endl;
}
template <class T> void Cola<T>::desencolar(void)
{
    if (!colavacia())head = (head + 1) % MAX;
    else cout << "Cola vacia" << endl;
}
template <class T> T Cola<T>::ultimo(void)
{
    return Q[tail];
}
template <class T> string Cola<T>::imprimir(string s)
{
    if (!colavacia()) {
       for (int i = head; i != (tail + 1) % MAX; i = (i + 1) % MAX)
            cout << Q[i] << "--";
    }

    return " <<< Q";
}

//------------------------------------------------------
int main()
{
    Cola<int>* c = new Cola<int>();
    if (c->colavacia()) cout << endl << "La cola esta VACIA" << endl << endl;
    c->encolar(15);
    c->encolar(2);
    c->encolar(3);
    cout << c->imprimir(" >>>") << endl;
    if (!c->colavacia())cout << "en cola:" << c->tope() << endl;
    cout << "ultimo ingresado= " << c->ultimo() << endl;
    if (!c->colavacia())c->desencolar();
   
    if (!c->colavacia())cout << "en cola:" << c->tope() << endl;
    cout << c->imprimir(" >>>") << endl;
    if (!c->colavacia())c->desencolar();

    if (!c->colavacia())cout << "en cola:" << c->tope() << endl;
    cout << c->imprimir(" >>>") << endl;
   
    if (!c->colavacia())c->desencolar();
    if (!c->colavacia())cout << "en cola:" << c->tope() << endl;
    cout << c->imprimir(" >>>") << endl;
    
    c->encolar(44);
    if (!c->colavacia())cout << "en cola:" << c->tope() << endl;
    cout << c->imprimir(" >>>") << endl;
    c->encolar(55);
    if (!c->colavacia())cout << "en cola:" << c->tope() << endl;
    cout << c->imprimir(" >>>") << endl;
    

    system("PAUSE");
    return EXIT_SUCCESS;

}
