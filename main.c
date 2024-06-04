#include <stdio.h>
#include <string.h>
#include "vacunodromo.h"

/*Funcion que comprueba si se introdujo por linea de comandos un nombre de fichero y devuelve 1 o 0 en caso contrario*/
int hayFichero(int argc, char **argv);

/*Funcion que carga el menu que permite crear la cola de vacunacion y vacunar a los pacientes*/
void menuVacunacion();

/*Funcion que muestra el menu de gestion de la base de datpos*/
void menuPacientes();

TABB A;

int main(int argc, char **argv) {
    char opcion;
    char nombre_fichero[30];

    printf("Bienvenid@ al programa de gestion PacVac\n");

/*Creamos el arbol binario de busqueda que representara a la base de datos*/
    crearAbb(&A);   
    if (hayFichero(argc, argv))
        leerFichero(argv[2], &A);
        
    do {
        printf("Seleccione una opción:\na) Gestión del vacunodromo\nb) Gestión de la base de datos\nc) Salír "
               "\n Inserte una opción: ");
        scanf(" %c", &opcion);
        switch (opcion) {
            case 'a':
                menuVacunacion();
                break;
            case 'b':
                menuPacientes(argc, argv);
                break;
            case 'c':
                printf("Saliendo del programa\n");

                /*Comprobamos si hay un fichero cargado para escribir los datos*/
                if (hayFichero(argc, argv))
                    escribirFichero(argv[2], A);
                else {

                    /*Pedimos al usuario un nombre de fichero para escribir los datos*/
                    printf("Introduzca la ruta del fichero en el que quiere imprimir los datos: \n");
                    scanf(" %[^\r\n]", nombre_fichero);
                    escribirFichero(nombre_fichero, A);
                }
                destruirVacunodromo(&A);
                break;
            default:
                printf("Opción incorrecta\n");
                break;
        }
    } while (opcion != 'c');

    return 0;
}

int hayFichero(int argc, char **argv) {

    /*Comprobamos que haya 3 argumentos*/
    if (argc == 3)

        /*Comprobamos que el segundo argumento sea -f*/
        if (!strcmp(argv[1], "-f"))

            /*Comprobamos que el tercer argumento (el nombre del fichero sea distinto de null)*/
            if (argv[2] != NULL) {
                return 1;
            }
    return 0;
}

void menuVacunacion() {
    char opcion;
    VACUNODROMO vacunodromo;
    int colaCreada = 0;

    do {
        printf("Seleccione una opción:\na) Crear cola de vacunación\nb) Vacunar\nc) Salír\n Inserte una opción: ");
        scanf(" %c", &opcion);
        switch (opcion) {
            case 'a':
                /*Comprobamos que haya pacientes en la base de datos (árbol) para poder crear la cola
                 * y ponemos la variable colaCreada a 0*/
                if (!esAbbVacio(A)) {
                    /*Si la cola ya esta creada la destruimos*/
                    if (colaCreada > 0) {
                        destruirCola(&vacunodromo.cola_pacientes);
                        colaCreada--;
                    }
                    /*Creamos la cola de vacunacion y ponemos la variable colaCreada a 1*/
                    if (crearColaVacunacion(&vacunodromo, A))
                        colaCreada++;
                } else printf("BDD vacia, inserte primero los pacientes\n");
                break;
            case 'b':
                if (esAbbVacio(A))
                    printf("BDD vacia, inserte primero los pacientes\n");
                else if (colaCreada) {

                    /*Llamamos a la funcion de vacunodromo que vacunara al primer elemento de la cola si es posible*/
                    vacunar(&vacunodromo, &A);

                    /*Si no quedan pacientes en la cola de vacunacion, la eliminamos y ponemos la variable colaCreada a 0*/
                    if (esColaVacia(vacunodromo.cola_pacientes)) {
                        destruirCola(&vacunodromo.cola_pacientes);
                        colaCreada--;
                    }
                } else printf("El vacunodromo no cuenta con una cola de vacunación, creela primero\n");
                break;
            case 'c':
                /*Al salir de este menú, si esta creada, eliminamos la cola de vacunacion actual
                 * y ponemos la variable colaCreada a 0*/
                if (colaCreada > 0) {
                    destruirCola(&vacunodromo.cola_pacientes);
                    colaCreada--;
                }
                printf("********************************************\n");
                break;
            default:
                printf("Opción incorrecta\n");
                break;
        }
    } while (opcion != 'c');
}

void menuPacientes() {
    char opcion;

    do {
        printf("Seleccione una opción:\na) Añadir paciente\nb) Listado de pacientes\nc) Eliminar paciente "
               "\nd) Salír\n Inserte una opción: ");
        scanf(" %c", &opcion);
        switch (opcion) {
            case 'a':
                anhadirPaciente(&A);
                break;
            case 'b':
                listadoPacientes(A);
                break;
            case 'c':
                eliminarPaciente(&A);
                break;
            case 'd':
                printf("********************************************\n");
                break;
            default:
                printf("Opción incorrecta\n");
                break;
        }
    } while (opcion != 'd');
}
