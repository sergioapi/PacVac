
#ifndef SESION3_VACUNODROMO_H
#define SESION3_VACUNODROMO_H

#include "abb.h"
#include "cola.h"

typedef struct vacunodromo {
    char nombre_vacuna[MAX_VACUNA];
    TCOLA cola_pacientes;
    unsigned int num_pacientes;
} VACUNODROMO;

int crearColaVacunacion(VACUNODROMO *vacunodromo, TABB abb);

void vacunar(VACUNODROMO *vacunodromo, TABB *abb);

void anhadirPaciente(TABB *abb);

void listadoPacientes(TABB abb);

void eliminarPaciente(TABB *abb);

void destruirVacunodromo(TABB *abb);

void leerFichero(char *nombreFichero, TABB *abb);

void escribirFichero(char *nombreFichero, TABB abb);

#endif //SESION3_VACUNODROMO_H
