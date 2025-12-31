Aquí tienes un programa en C que usa malloc y punteros para calcular cuántos bytes ocupan diez enteros (int) en memoria:

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Asignar memoria dinámica para 10 enteros
    int *ptr = (int *)malloc(10 * sizeof(int));

    if (ptr == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1; // Salir con código de error
    }

    // Calcular los bytes ocupados
    size_t bytes = 10 * sizeof(int);

    printf("Diez enteros ocupan %zu bytes en memoria.\n", bytes);

    // Liberar la memoria asignada
    free(ptr);

    return 0;
}
```

Explicación del código:

1. Inclusión de bibliotecas:
   · stdio.h para entrada/salida.
   · stdlib.h para malloc y free.
2. Asignación dinámica:
   · malloc(10 * sizeof(int)) reserva memoria para 10 enteros.
   · El resultado se convierte a int * y se guarda en ptr.
3. Manejo de errores:
   · Si malloc falla (devuelve NULL), se muestra un error y el programa termina.
4. Cálculo de bytes:
   · 10 * sizeof(int) calcula el total de bytes ocupados por 10 enteros.
   · sizeof(int) devuelve el tamaño en bytes de un entero en tu sistema.
5. Liberación de memoria:
   · free(ptr) libera la memoria asignada para evitar fugas.

Ejecución del programa:

· El programa imprimirá algo como:
    Diez enteros ocupan 40 bytes en memoria.
    (Suponiendo que sizeof(int) == 4, lo cual es común en muchos sistemas).

Notas:

· El valor de sizeof(int) depende del compilador y sistema (normalmente 4 bytes en sistemas de 32/64 bits modernos).
· %zu es el especificador de formato para size_t (tipo retornado por sizeof).
