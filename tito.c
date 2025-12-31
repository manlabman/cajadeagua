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
