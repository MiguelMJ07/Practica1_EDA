# ST0245 — Práctica I: Sorting Large Dataset

> Estructuras de Datos y Algoritmos · EAFIT University · Feb 2026  
> Docente: Alexander Narváez Berrío

---

## Descripción

Programa en C++ que ordena un dataset de 100,000 palabras en inglés usando tres estrategias de ordenamiento distintas, cada una basada en una estructura de datos diferente. El objetivo es comparar el rendimiento real de cada algoritmo y contrastarlo con su complejidad teórica (Big O).

Cada algoritmo se prueba en tres escenarios: datos aleatorios, datos ordenados y datos en orden inverso.

---

## Algoritmos implementados

### QuickSort
Implementado sobre `std::vector<string>` con pivote **mediana-de-tres** y partición de Lomuto. El pivote mediana-de-tres evita el peor caso `O(n²)` cuando los datos ya están ordenados.

| Caso | Complejidad |
|------|-------------|
| Mejor | `O(n log n)` |
| Promedio | `O(n log n)` |
| Peor | `O(n²)` |
| Memoria | `O(log n)` |

### HeapSort
Heap binario construido **in-place** sobre el vector. La construcción del Max-Heap se hace en `O(n)` usando el método bottom-up, seguido de extracción ordenada en `O(n log n)`.

| Caso | Complejidad |
|------|-------------|
| Mejor | `O(n log n)` |
| Promedio | `O(n log n)` |
| Peor | `O(n log n)` |
| Memoria | `O(1)` |

### AVL Tree
Árbol Binario de Búsqueda auto-balanceado (Adelson-Velsky & Landis, 1962). Implementa las 4 rotaciones (LL, RR, LR, RL) y obtiene el ordenamiento mediante **recorrido inorder**. La altura máxima del árbol es `1.44 * log2(n)`.

| Caso | Complejidad |
|------|-------------|
| Mejor | `O(n log n)` |
| Promedio | `O(n log n)` |
| Peor | `O(n log n)` |
| Memoria | `O(n)` |

---

## Estructura del proyecto

```
Practica1/
├── main_unified_v3.cpp   # Código fuente principal
├── dataset.txt           # Dataset de 100,000 palabras
└── README.md
```
---

## Ejemplo de salida

```
--- ST0245 Practica I: Sorting Large Dataset ---

Dataset cargado: 100000 palabras

Algoritmo   Mejor         Promedio      Peor          Memoria   Estable
--------------------------------------------------------------------
QuickSort   O(n log n)    O(n log n)    O(n^2)        O(log n)  No
HeapSort    O(n log n)    O(n log n)    O(n log n)    O(1)      No
AVL Tree    O(n log n)    O(n log n)    O(n log n)    O(n)      Si

Ejecutando benchmarks...

Algoritmo   Escenario   Tiempo (ms)   Correcto
--------------------------------------------------
QuickSort   Aleatorio       39.02     OK
QuickSort   Ordenado        11.61     OK
QuickSort   Inverso         47.92     OK
HeapSort    Aleatorio       69.81     OK
HeapSort    Ordenado        52.88     OK
HeapSort    Inverso         59.56     OK
AVL Tree    Aleatorio       81.79     OK
AVL Tree    Ordenado        42.61     OK
AVL Tree    Inverso         40.69     OK

--- Analisis comparativo ---
QuickSort: el mas rapido en promedio por mejor localidad de cache.
  Con pivote mediana-de-tres se evita el peor caso O(n^2) en datos ordenados.
  Desventaja: no garantiza O(n log n) en el peor caso absoluto.

HeapSort: O(n log n) garantizado siempre, sin importar el orden inicial.
  In-place: usa O(1) memoria extra. Mas lento que QuickSort en la practica
  porque los accesos al heap saltan posiciones (malos cache misses).

AVL Tree: tambien O(n log n) garantizado. Altura maxima = 1.44*log2(n).
  Usa mas memoria que los otros (~56 bytes/nodo vs ~32 bytes/string en vector).
  Su ventaja real es mantener el orden con inserciones/busquedas dinamicas.

Conclusion: QuickSort es la mejor opcion para ordenar en memoria una sola vez.
HeapSort si la memoria es critica. AVL si el dataset cambia continuamente.

Listo.
```

---

## Conclusiones

**QuickSort** resulta el más rápido en la práctica para datos aleatorios gracias a su mejor localidad de caché. El pivote mediana-de-tres reduce drásticamente el riesgo del peor caso `O(n²)` en datos ordenados o inversos, lo que se refleja en los tiempos medidos.

**HeapSort** cumple su promesa teórica: `O(n log n)` garantizado sin importar el orden de entrada, y sin usar memoria extra. Sin embargo, los accesos no secuenciales al heap generan más cache misses que QuickSort, lo que lo hace más lento en la práctica a pesar de tener la misma complejidad asintótica.

**AVL Tree** también garantiza `O(n log n)`, pero tiene el mayor overhead de los tres debido a la asignación dinámica de memoria por nodo y las rotaciones de balanceo. Su ventaja real no es ordenar una sola vez, sino mantener el conjunto ordenado mientras se insertan y buscan elementos de forma continua.

Para el problema específico de ordenar 100,000 strings en memoria una sola vez, **QuickSort es la opción óptima**.
