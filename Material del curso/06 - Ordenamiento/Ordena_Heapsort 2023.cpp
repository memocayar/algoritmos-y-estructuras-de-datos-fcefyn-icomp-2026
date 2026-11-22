#include <iostream>
using namespace std;

// Función para intercambiar dos elementos en un arreglo
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Función para ajustar un subárbol con raíz en el índice i en un arreglo
void heapify(int arr[], int n, int i) {
    int largest = i; // Inicializa el índice de la raíz como el mayor
    int left = 2 * i + 1; // Índice del hijo izquierdo
    int right = 2 * i + 2; // Índice del hijo derecho

    // Si el hijo izquierdo es mayor que la raíz
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // Si el hijo derecho es mayor que la raíz
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Si el mayor no es la raíz
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Ajusta el subárbol afectado
        heapify(arr, n, largest);
    }
}

// Función principal para ordenar un arreglo utilizando heapsort
void heapsort(int arr[], int n) {
    // Construye un montículo (heap)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extrae los elementos del montículo uno por uno
    for (int i = n - 1; i >= 0; i--) {
        // Mueve la raíz actual al final
        swap(arr[0], arr[i]);

        // Ajusta el montículo reducido
        heapify(arr, i, 0);
    }
}

// Función para imprimir un arreglo
void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

// Función principal
int main() {
    int arr[] = { 12, 11, 13, 5, 6, 7 };
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Arreglo original: ";
    printArray(arr, n);

    heapsort(arr, n);

    cout << "Arreglo ordenado: ";
    printArray(arr, n);
    
    system("PAUSE");	
    return 0;
}

