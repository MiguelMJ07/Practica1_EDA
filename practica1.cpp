#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

vector<string> cargarDataset(string ruta) {
    ifstream file(ruta);
    if (!file.is_open()) {
        cout << "Error: no se pudo abrir " << ruta << endl;
        exit(1);
    }
    vector<string> palabras;
    string linea;
    while (getline(file, linea)) {
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (!linea.empty()) palabras.push_back(linea);
    }
    return palabras;
}

double tiempoMs(high_resolution_clock::time_point inicio) {
    return duration<double, milli>(high_resolution_clock::now() - inicio).count();
}

bool estaOrdenado(vector<string>& v) {
    for (int i = 1; i < (int)v.size(); i++)
        if (v[i] < v[i-1]) return false;
    return true;
}

void mediana3(vector<string>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid])  swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    swap(arr[mid], arr[high]);
}

int particion(vector<string>& arr, int low, int high) {
    if (high - low >= 2) mediana3(arr, low, high);
    string pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) swap(arr[++i], arr[j]);
    }
    swap(arr[i+1], arr[high]);
    return i + 1;
}

void quickSort(vector<string>& arr, int low, int high) {
    if (low < high) {
        int p = particion(arr, low, high);
        quickSort(arr, low, p - 1);
        quickSort(arr, p + 1, high);
    }
}

void heapifyDown(vector<string>& arr, int n, int i) {
    while (true) {
        int mayor = i;
        int izq = 2*i + 1;
        int der = 2*i + 2;
        if (izq < n && arr[izq] > arr[mayor]) mayor = izq;
        if (der < n && arr[der] > arr[mayor]) mayor = der;
        if (mayor == i) break;
        swap(arr[i], arr[mayor]);
        i = mayor;
    }
}

void heapSort(vector<string>& arr) {
    int n = arr.size();
    for (int i = n/2 - 1; i >= 0; i--)
        heapifyDown(arr, n, i);
    for (int i = n-1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapifyDown(arr, i, 0);
    }
}

struct Nodo {
    string clave;
    Nodo* izq;
    Nodo* der;
    int altura;
    Nodo(string k) : clave(k), izq(nullptr), der(nullptr), altura(1) {}
};

int altura(Nodo* n) { return n ? n->altura : 0; }

void actualizarAltura(Nodo* n) {
    n->altura = 1 + max(altura(n->izq), altura(n->der));
}

int balance(Nodo* n) { return n ? altura(n->izq) - altura(n->der) : 0; }

Nodo* rotDer(Nodo* y) {
    Nodo* x = y->izq;
    y->izq = x->der;
    x->der = y;
    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

Nodo* rotIzq(Nodo* x) {
    Nodo* y = x->der;
    x->der = y->izq;
    y->izq = x;
    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

Nodo* insertar(Nodo* nodo, string clave) {
    if (!nodo) return new Nodo(clave);
    if (clave < nodo->clave)      nodo->izq = insertar(nodo->izq, clave);
    else if (clave > nodo->clave) nodo->der = insertar(nodo->der, clave);
    else return nodo;

    actualizarAltura(nodo);
    int fb = balance(nodo);

    if (fb > 1 && clave < nodo->izq->clave)  return rotDer(nodo);
    if (fb < -1 && clave > nodo->der->clave) return rotIzq(nodo);
    if (fb > 1 && clave > nodo->izq->clave)  { nodo->izq = rotIzq(nodo->izq); return rotDer(nodo); }
    if (fb < -1 && clave < nodo->der->clave) { nodo->der = rotDer(nodo->der); return rotIzq(nodo); }

    return nodo;
}

void inorder(Nodo* nodo, vector<string>& salida) {
    if (!nodo) return;
    inorder(nodo->izq, salida);
    salida.push_back(nodo->clave);
    inorder(nodo->der, salida);
}

void destruir(Nodo* nodo) {
    if (!nodo) return;
    destruir(nodo->izq);
    destruir(nodo->der);
    delete nodo;
}

struct Resultado {
    string algoritmo, escenario;
    double tiempoMs;
    bool correcto;
};

vector<Resultado> correrBenchmarks(vector<string>& original) {
    vector<Resultado> resultados;

    vector<string> aleatorio = original;
    vector<string> ordenado  = original;
    sort(ordenado.begin(), ordenado.end());
    vector<string> inverso = ordenado;
    reverse(inverso.begin(), inverso.end());

    vector<pair<string, vector<string>*>> escenarios = {
        {"Aleatorio", &aleatorio},
        {"Ordenado",  &ordenado},
        {"Inverso",   &inverso}
    };

    for (auto& [nombre, datos] : escenarios) {
        vector<string> arr = *datos;
        auto t = high_resolution_clock::now();
        quickSort(arr, 0, arr.size()-1);
        resultados.push_back({"QuickSort", nombre, tiempoMs(t), estaOrdenado(arr)});
    }

    for (auto& [nombre, datos] : escenarios) {
        vector<string> arr = *datos;
        auto t = high_resolution_clock::now();
        heapSort(arr);
        resultados.push_back({"HeapSort", nombre, tiempoMs(t), estaOrdenado(arr)});
    }

    for (auto& [nombre, datos] : escenarios) {
        auto t = high_resolution_clock::now();
        Nodo* raiz = nullptr;
        for (auto& palabra : *datos)
            raiz = insertar(raiz, palabra);
        vector<string> sorted;
        inorder(raiz, sorted);
        destruir(raiz);
        resultados.push_back({"AVL Tree", nombre, tiempoMs(t), estaOrdenado(sorted)});
    }

    return resultados;
}

int main(int argc, char* argv[]) {
    string ruta = argc > 1 ? argv[1] : "dataset.txt";

    cout << "\n--- ST0245 Practica I: Sorting Large Dataset ---\n\n";

    vector<string> dataset = cargarDataset(ruta);
    cout << "Dataset cargado: " << dataset.size() << " palabras\n\n";

    cout << left
         << setw(12) << "Algoritmo"
         << setw(14) << "Mejor"
         << setw(14) << "Promedio"
         << setw(14) << "Peor"
         << setw(10) << "Memoria"
         << "Estable\n";
    cout << string(68, '-') << "\n";
    cout << setw(12) << "QuickSort" << setw(14) << "O(n log n)" << setw(14) << "O(n log n)" << setw(14) << "O(n^2)"     << setw(10) << "O(log n)" << "No\n";
    cout << setw(12) << "HeapSort"  << setw(14) << "O(n log n)" << setw(14) << "O(n log n)" << setw(14) << "O(n log n)" << setw(10) << "O(1)"     << "No\n";
    cout << setw(12) << "AVL Tree"  << setw(14) << "O(n log n)" << setw(14) << "O(n log n)" << setw(14) << "O(n log n)" << setw(10) << "O(n)"     << "Si\n";
    cout << "\n";

    cout << "Ejecutando benchmarks...\n\n";
    vector<Resultado> resultados = correrBenchmarks(dataset);

    cout << left
         << setw(12) << "Algoritmo"
         << setw(12) << "Escenario"
         << setw(14) << "Tiempo (ms)"
         << "Correcto\n";
    cout << string(50, '-') << "\n";
    for (auto& r : resultados) {
        cout << setw(12) << r.algoritmo
             << setw(12) << r.escenario
             << setw(14) << fixed << setprecision(2) << r.tiempoMs
             << (r.correcto ? "OK" : "ERROR") << "\n";
    }

    cout << "\n--- Analisis comparativo ---\n";
    cout <<
        "QuickSort: el mas rapido en promedio por mejor localidad de cache.\n"
        "  Con pivote mediana-de-tres se evita el peor caso O(n^2) en datos ordenados.\n"
        "  Desventaja: no garantiza O(n log n) en el peor caso absoluto.\n\n"

        "HeapSort: O(n log n) garantizado siempre, sin importar el orden inicial.\n"
        "  In-place: usa O(1) memoria extra. Mas lento que QuickSort en la practica\n"
        "  porque los accesos al heap saltan posiciones (malos cache misses).\n\n"

        "AVL Tree: tambien O(n log n) garantizado. Altura maxima = 1.44*log2(n).\n"
        "  Usa mas memoria que los otros (~56 bytes/nodo vs ~32 bytes/string en vector).\n"
        "  Su ventaja real es mantener el orden con inserciones/busquedas dinamicas.\n\n"

        "Conclusion: QuickSort es la mejor opcion para ordenar en memoria una sola vez.\n"
        "HeapSort si la memoria es critica. AVL si el dataset cambia continuamente.\n";

    cout << "\nListo.\n\n";
    return 0;
}
