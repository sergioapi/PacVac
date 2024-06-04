//
// Created by sergio on 29/09/22.
//
#include "lista.h"
#include "abb.h"
#include "vacunodromo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** Declaración de funciones privadas */

int _anhadirACola(TABB abb, VACUNODROMO *vacunodromo, char *nombreVacuna);

int crearColaVacunacion(VACUNODROMO *vacunodromo, TABB abb);

void _pedirVacunas(TIPOELEMENTOABB *paciente);

void _imprimirVacunas(TIPOELEMENTOABB paciente);

void _imprimirPacientes(TABB abb);

void _procesar_linea(char *linea, TABB *abb);

void _escribirVacunas(TIPOELEMENTOABB paciente, FILE *fp);

void _escribirPacientes(TABB abb, FILE *fp);

/* Busca en una lista de vacunas, una cuyo nombre sea nombreVacuna y devuelve 1 si la encuentra o 0 si no*/
int __comprobarVacuna(TLISTA listaVacunas, char *nombreVacuna) {
    if (!esListaVacia(listaVacunas)) {
        TPOSICION p = primeroLista(listaVacunas);
        TIPOELEMENTOLISTA vacuna;
        /*Recorremos la lista de vacunas comprobando si coincide algun nombre con la que queremos buscar*/
        while (p != finLista(listaVacunas)) {
            recuperarElementoLista(listaVacunas, p, &vacuna);
            if (strcmp(vacuna.nombreVacuna, nombreVacuna) == 0) return 1;
            p = siguienteLista(listaVacunas, p);
        }

    }
    return 0;
}

/* Función recursiva privada que recorre el ABB y añade a la cola de vacunación a los pacientes que tengan la vacuna.
 * Devuelve el número de pacientes añadidos*/
int _anhadirACola(TABB abb, VACUNODROMO *vacunodromo, char *nombreVacuna) {
    TIPOELEMENTOCOLA paciente;

    if (!esAbbVacio(abb)) {
        _anhadirACola(izqAbb(abb), vacunodromo, nombreVacuna);
        leerElementoAbb(abb, &paciente);
        /*Llamamos a la funcion comprobarVacuna que buscará en la lista de un paciente una vacuna cuyo nombre coincida
         * con nombreVacuna y devolvera 1 si la encuentra o 0 en caso contrario.*/
        if (__comprobarVacuna(paciente.listaVacunas, nombreVacuna) == 1) {
            /*Añadimos el paciente a la cola y aumentamos el nº de pacientes del vacunodromo*/
            anadirElementoCola(&vacunodromo->cola_pacientes, paciente);
            vacunodromo->num_pacientes++;
        }
        _anhadirACola(derAbb(abb), vacunodromo, nombreVacuna);
    }
}

/* Funcion que inicializa los campos de vacunodromo y crea la cola añadiendo los pacientes correspondientes.
 * Devuelve 1 si se creó la cola y 0 en caso contrario*/
int crearColaVacunacion(VACUNODROMO *vacunodromo, TABB abb) {

    /*Inicializamos los campos del struct vacunodromo*/
    printf("Introduzca el nombre de la vacuna: ");
    scanf(" %s", vacunodromo->nombre_vacuna);
    vacunodromo->num_pacientes = 0;
    crearCola(&vacunodromo->cola_pacientes);

    /*Llamamos a la funcion que añade los pacientes con cierta vacuna en su lista a la cola*/
    _anhadirACola(abb, vacunodromo, vacunodromo->nombre_vacuna);

    /*Si el nº de pacientes del vacunodromo es cero, quiere decir que no hay pacientes con esa vacuna en su lista
     por lo tanto no se debe crear la cola de vacunacion**/
    if (!vacunodromo->num_pacientes) {
        destruirCola(&vacunodromo->cola_pacientes);
        printf("No se pudo crear la cola\n");
        return 0;

    } else printf("Cola de pacientes creada correctamente\n");

    return 1;
}

/*Funcion que aumenta el nº de dosis administradas de la vacuna del vacunodromo al primer paciente de la cola*/
void vacunar(VACUNODROMO *vacunodromo, TABB *abb) {
    TIPOELEMENTOCOLA paciente;
    TIPOELEMENTOLISTA vacuna;
    TPOSICION p;

    consultarPrimerElementoCola(vacunodromo->cola_pacientes, &paciente);
    if (!esListaVacia(paciente.listaVacunas)) {
        p = primeroLista(paciente.listaVacunas);
        printf("Vacunando el paciente %s con %s\n", paciente.ApNombre, vacunodromo->nombre_vacuna);
        while (p != finLista(paciente.listaVacunas)) {
            recuperarElementoLista(paciente.listaVacunas, p, &vacuna);
            if (strcmp(vacuna.nombreVacuna, vacunodromo->nombre_vacuna) == 0) {

                /*Al coincidir los nombres de las vacunas aumentamos en uno el nº de dosis administradas*/
                vacuna.nDosis++;
                modificarElementoLista(&paciente.listaVacunas, p, vacuna);

                /*Suprimimos al paciente de la cola para añadirlo al final si tiene mas dosis pendientes o
                 * eliminarlo definitivamente si ya se ha vacunado*/
                suprimirElementoCola(&vacunodromo->cola_pacientes);
                if (vacuna.nDosis < vacuna.maxDosis) {
                    anadirElementoCola(&vacunodromo->cola_pacientes, paciente);
                    printf("Vacunacion exitosa\n");
                    printf("Dosis pendientes de %s: %d\n", vacuna.nombreVacuna, (vacuna.maxDosis - vacuna.nDosis));
                } else {
                    suprimirElementoLista(&paciente.listaVacunas, p);
                    printf("Nº de dosis máximas de %s alcanzado\n", vacuna.nombreVacuna);

                    /*Si no tiene mas vacunas pendientes lo eliminamos tambien del arbol*/
                    if (esListaVacia(paciente.listaVacunas)) {
                        suprimirElementoAbb(abb, paciente);
                        printf("El paciente ya no tiene vacunas pendientes, eliminandolo de la BDD\n");
                    }
                }

                /*Igualamos p a fin lista para salir del bucle*/
                p = finLista(paciente.listaVacunas);
            } else {
                printf("No se pudo vacunar al paciente %s con %s\n", paciente.ApNombre, vacuna.nombreVacuna);
                p = siguienteLista(paciente.listaVacunas, p);
            }
        }

    }
}

void _pedirVacunas(TIPOELEMENTOABB *paciente) {
    crearLista(&(paciente->listaVacunas));
    TIPOELEMENTOLISTA vacuna;

    do {
        printf("Introduzca el nombre de la vacuna (fin para terminar): ");
        scanf(" %[^\r\n]", vacuna.nombreVacuna);

        if (strcmp(vacuna.nombreVacuna, "fin") != 0) {
            printf("Introduzca el numero de dosis máximas: ");
            scanf("%d", &vacuna.maxDosis);

            if (vacuna.maxDosis < 1) {
                printf("nº de dosis incorrecto, la vacuna no se guardará\n");
            } else {

                vacuna.nDosis = 0;
                insertarElementoLista(&paciente->listaVacunas, finLista(paciente->listaVacunas), vacuna);
            }
        }
    } while (strcmp(vacuna.nombreVacuna, "fin") != 0);
}

void anhadirPaciente(TABB *abb) {
    TIPOELEMENTOABB paciente;

    printf("Introduzca el nombre del paciente: ");
    scanf(" %[^\r\n]\"", paciente.ApNombre);

    if (!esMiembroAbb(*abb, paciente)) {
        _pedirVacunas(&paciente);

        if (esListaVacia(paciente.listaVacunas)) {
            printf("No se puede guardar un paciente sin vacunas\n");
            destruirLista(&paciente.listaVacunas);

        } else insertarElementoAbb(abb, paciente);

    } else printf("El paciente ya está almacenado\n");

}

void _imprimirVacunas(TIPOELEMENTOABB paciente) {

    if (!esListaVacia(paciente.listaVacunas)) {
        TPOSICION p = primeroLista(paciente.listaVacunas);
        TIPOELEMENTOLISTA vacuna;

        while (p != finLista(paciente.listaVacunas)) {

            recuperarElementoLista(paciente.listaVacunas, p, &vacuna);
            printf("\tVacuna: %s\tDosis %d de %d\n", vacuna.nombreVacuna, vacuna.nDosis, vacuna.maxDosis);
            p = siguienteLista(paciente.listaVacunas, p);
        }
        printf("\n");

    } else printf("\nEl paciente no tiene vacunas\n");
}

/*Imprime los pacientes de un arbol que recorre en inorden*/
void _imprimirPacientes(TABB abb) {

    if (!esAbbVacio(abb)) {
        TIPOELEMENTOABB paciente;
        // Izquierda
        _imprimirPacientes(izqAbb(abb));
        // Raiz
        leerElementoAbb(abb, &paciente);
        printf("%s\n", paciente.ApNombre);
        _imprimirVacunas(paciente);
        // Derecha
        _imprimirPacientes(derAbb(abb));
    }
}

void listadoPacientes(TABB abb) {

    if (!esAbbVacio(abb)) {
        _imprimirPacientes(abb);
    } else printf("No hay pacientes\n");

}

void eliminarPaciente(TABB *abb) {
    TIPOELEMENTOABB paciente;

    if (!esAbbVacio(*abb)) {

        paciente.listaVacunas = NULL;
        printf("Introduzca el nombre del paciente que desea eliminar: ");
        scanf(" %[^\r\n]", paciente.ApNombre);

        if (!esMiembroAbb(*abb, paciente)) {
            printf("El paciente no se encuentra\n");

        } else {
            printf("Paciente encontrado, eliminandolo...\n");
            suprimirElementoAbb(abb, paciente);
        }
    } else printf("No hay pacientes\n");
}

void destruirVacunodromo(TABB *abb) {

    if (!esAbbVacio(*abb)) {
        destruirAbb(abb);
    }
}

void _procesar_linea(char *linea, TABB *abb) {
    TIPOELEMENTOABB paciente;
    TIPOELEMENTOLISTA vacuna;
    int cont = 0;

    crearLista(&paciente.listaVacunas);

/*Leemos el nombre del paciente hasta el caracter | y lo asignamos*/
    char *nombre_paciente = strtok(linea, "|");
    strcpy(paciente.ApNombre, nombre_paciente);

/*Guardamos el resto de la linea, con todas las vacunas*/
    char *lista_vacunas = strtok(NULL, "|");

/*Leemos el nombre de la primera vacuna*/
    char *token = strtok(lista_vacunas, ";");

    while (token != NULL) {
        cont++;
        switch (cont) {
            case 1:
                /*Si cont es 1 en token hay el nombre de una vacuna*/
                strcpy(vacuna.nombreVacuna, token);
                break;
            case 2:
                /*Si cont es 2 en token hay el nº de dosis administradas de una vacuna*/
                vacuna.nDosis = atoi(token);
                break;
            case 3:
                /*Si cont es 3 en token hay el nº de dosis maximas de una vacuna*/
                vacuna.maxDosis = atoi(token);
                /*Insertamos la vacuna en la lista y ponemos cont a 0*/
                insertarElementoLista(&paciente.listaVacunas, finLista(paciente.listaVacunas), vacuna);
                cont = 0;
                break;
        }
        token = strtok(NULL, ";");
    }

    if (!esMiembroAbb(*abb, paciente)) {
        if (esListaVacia(paciente.listaVacunas)) {
            printf("No se puede guardar un paciente sin vacunas\n");
            destruirLista(&paciente.listaVacunas);

        } else insertarElementoAbb(abb, paciente);

    } else printf("El paciente ya está almacenado\n");
}

void leerFichero(char *nombreFichero, TABB *abb) {
    FILE *fp;
    char linea[100];

    fp = fopen(nombreFichero, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    while (fgets(linea, 100, fp)) {
        linea[strcspn(linea, "\r\n")] = 0;
        _procesar_linea(linea, abb);
    }
    fclose(fp);
}

void _escribirVacunas(TIPOELEMENTOABB paciente, FILE *fp) {
    TIPOELEMENTOLISTA vacuna;
    TPOSICION p = primeroLista(paciente.listaVacunas);

    while (p != finLista(paciente.listaVacunas)) {

        recuperarElementoLista(paciente.listaVacunas, p, &vacuna);
        fprintf(fp, "%s;%d;%d;", vacuna.nombreVacuna, vacuna.nDosis, vacuna.maxDosis);
        p = siguienteLista(paciente.listaVacunas, p);
    }
}

/*Escribimos los pacientes en preorden*/
void _escribirPacientes(TABB abb, FILE *fp) {
    if (esAbbVacio(abb) == 0) {
        TIPOELEMENTOABB paciente;
        // Raíz
        leerElementoAbb(abb, &paciente);
        fprintf(fp, "%s|", paciente.ApNombre);
        _escribirVacunas(paciente, fp);
        fprintf(fp, "\n");
        // Izquierda
        _escribirPacientes(izqAbb(abb), fp);
        // Derecha
        _escribirPacientes(derAbb(abb), fp);
    }
}

void escribirFichero(char *nombreFichero, TABB abb) {
    FILE *fp;

    fp = fopen(nombreFichero, "w");
    if (fp == NULL) {
        perror("Error opening file");
    } else {
        _escribirPacientes(abb, fp);
    }

    fclose(fp);
}
