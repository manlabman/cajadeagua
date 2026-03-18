S_IRUSR es una constante simbólica definida en el estándar POSIX (Portable Operating System Interface) y se utiliza comúnmente en sistemas operativos tipo Unix (como Linux, macOS) para la programación en C/C++.

Su significado literal es:

· S\_: Prefijo estándar para constantes de estado (stat) o permisos.
· IR: "Read" (lectura).
· USR: "User" (usuario), refiriéndose al propietario del archivo.

En resumen: S_IRUSR representa el permiso de lectura para el propietario del archivo.

¿Para qué se usa?

Se utiliza principalmente al trabajar con funciones que manejan permisos de archivos, como:

1. open(): Al crear un archivo, se usa para establecer los permisos iniciales.
2. chmod() o fchmod(): Para cambiar los permisos de un archivo existente.
3. stat(): Para examinar los permisos actuales de un archivo.

Ejemplo en C

Imagina que quieres crear un archivo que solo pueda ser leído y escrito por su dueño:

```c
#include <fcntl.h>   // Para open y flags
#include <sys/stat.h> // Para las constantes S_IRUSR, S_IWUSR
#include <unistd.h>

int main() {
    // Crear un archivo con permisos:
    // - Dueño: Lectura (S_IRUSR) y Escritura (S_IWUSR)
    // - Grupo y otros: Sin ningún permiso.
    mode_t modo = S_IRUSR | S_IWUSR; // Unión (OR) de bits

    int fd = open("mi_archivo.txt", O_CREAT | O_WRONLY, modo);
    if (fd == -1) {
        perror("Error al crear el archivo");
        return 1;
    }
    close(fd);
    return 0;
}
```

¿Cómo se combina?

Los permisos se manejan mediante máscaras de bits. Puedes combinarlos usando el operador OR (|):

· S_IRUSR: Lectura para el dueño.
· S_IWUSR: Escritura para el dueño.
· S_IXUSR: Ejecución para el dueño.
· Existen también para el grupo (S_IRGRP, S_IWGRP, S_IXGRP) y para otros (S_IROTH, S_IWOTH, S_IXOTH).

Valor numérico

Aunque no deberías usar el número directamente, es útil para entenderlo: S_IRUSR suele tener el valor 0400 en octal (que es 256 en decimal). En octal, el primer dígito (4) indica que es el bit de lectura para el propietario.

En resumen, cuando veas S_IRUSR, piensa: "permiso de lectura para el usuario propietario".
