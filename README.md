# ST0245 - Práctica I: Ordenamiento de Dataset

**Curso:** Estructuras de Datos y Algoritmos  
**Universidad:** EAFIT  
**Valor:** 15% de la nota final

---

## Descripción

Programa en C++ que ordena 100,000 palabras usando tres algoritmos diferentes y compara su rendimiento en tres escenarios: datos aleatorios, ya ordenados y en orden inverso.

---

## Algoritmos implementados

| Algoritmo | Mejor caso | Peor caso | Memoria |
|-----------|-----------|-----------|---------|
| QuickSort | O(n log n) | O(n²) | O(log n) |
| HeapSort  | O(n log n) | O(n log n) | O(1) |
| AVL Tree  | O(n log n) | O(n log n) | O(n) |

---

## Requisitos

- Compilador g++ con soporte C++17
- Archivo `dataset.txt` con 100,000 palabras en la misma carpeta

---

## Compilar y ejecutar

```bash
g++ -std=c++17 -O2 -o practica1 acortado.cpp
./practica1 dataset.txt
```

---

## Resultados obtenidos

| Algoritmo | Aleatorio | Ordenado | Inverso |
|-----------|-----------|----------|---------|
| QuickSort | ~25 ms    | ~14 ms   | ~26 ms  |
| HeapSort  | ~39 ms    | ~24 ms   | ~26 ms  |
| AVL Tree  | ~217 ms   | ~136 ms  | ~131 ms |

---

## Estructura del código

```
acortado.cpp
├── cargarDataset()     → lee dataset.txt
├── QuickSort           → particion() + mediana3() + quickSort()
├── HeapSort            → heapifyDown() + heapSort()
├── AVL Tree            → insertar() + inorder() + destruir()
├── correrBenchmarks()  → ejecuta los 3 algoritmos en 3 escenarios
└── main()              → carga datos, corre benchmarks, imprime resultados
```

---

## Conclusión

QuickSort fue el más rápido gracias al pivote mediana-de-tres. HeapSort garantiza O(n log n) siempre pero es más lento por los saltos de memoria. AVL Tree es el mejor si el dataset cambia constantemente por sus operaciones en O(log n).
