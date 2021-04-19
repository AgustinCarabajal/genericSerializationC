#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <commons/string.h>

enum {
    STRING = 0,
    INT,
    CHAR,
    FLOAT
};

int get_type(char *type) {
    return 
        !strcmp(type, "s") ? STRING :
        !strcmp(type, "d") ? INT :
        !strcmp(type, "c") ? CHAR :
        !strcmp(type, "f") ? FLOAT :
    -1;
}

/**
 * METHOD: _serialize
 * 
 * Permite serializar cualquier struct o valor dentro de un buffer
 * 
 * @params: 
 *      size -> tamanio total del buffer a enviar
 *          IMPORTANT:
 *              for type Int    ->  Use sizeof(int)
 *              for type String ->  Use strlen(your_string) + sizeof(int)
 *
 *      format -> Permite especificar que tipo de variables queremos agregar al buffer [notacion: %d, %s, etc]
 *      
 *      ... -> variables correspondientes al formato especificado
 *
 * @example:
 *      Call with _serialize(sizeof(int), "%d", 25);
 */
void *_serialize(int size, char *format, ...) {
    if (!string_starts_with(format, "%")) {
        printf("\nError: Incorrect Format\n");
        return NULL;
    }

    void *stream = malloc(size);
    int offset = 0;

    char *string;
    int stringLength;
    int value;

    int arg_c = 0;

    // Cuento la cantidad de argumentos
    for (int n = 0; format[n]; n++) {
        if (format[n] == '%') arg_c++;
    }
    // Separo los tipos de datos
    char **types = string_split(format, "%");

    // Declaro la lista de argumentos de la funcion
    va_list lista_argumentos;
    // Inicializo la lista
    va_start(lista_argumentos, format);

    for(int i = 1; i <= arg_c; i++) {
        printf("Getting type of %s..\n", types[i]);
        switch(get_type(types[i])) {

            case STRING:
                printf("Got a string..\n");
                string = va_arg(lista_argumentos, char *);
                printf("String: %s\n", string);

                stringLength = strlen(string);
                memcpy(stream + offset, &stringLength, sizeof(int));
                offset += sizeof(int);
                memcpy(stream + offset, string, stringLength);
                offset += stringLength;

                break;
            case INT:
                printf("Got an Int..\n");
                value = va_arg(lista_argumentos, int);
                printf("INT: %d\n", value);

                memcpy(stream + offset, &value, sizeof(int));
                offset += sizeof(int);

                break;

            default:
                printf("\nError: %s Format Does not exist\n", types[i]);
                return NULL;
        }

    }

    for(int j = 0; j <= arg_c; j++){
        printf("Freeing the memory allocated by Types: %d\n", j);
        free(types[j]);
    }
    free(types);
    va_end(lista_argumentos);

    return stream;
    
}

int main () {

    void *stream = _serialize(5 + sizeof(int), "%s", "holis");

    //send()

    // Libero la memoria del buffer
    free(stream);

    return 0;
}