#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////// ESTRUCTURAS DE DATOS

struct celda {
    TIPOELEMENTOABB info;
    struct celda *izq, *der;
};

//////////////////////// FUNCIONES


/////////////////////////////////////////////////////////////
/////////////////////////// INICIO PARTE MODIFICABLE

/*Extraer la clave de una celda */ //Si la clave es orden alfabético
TIPOCLAVE _clave_elem(TIPOELEMENTOABB *E) {
    return E->ApNombre;
}

/* Esta funcion puente nos permite modificar el tipo de
 * de datos del TAD sin tener que cambiar todas las 
 * comparaciones del resto de la biblioteca y en su lugar
 * cambiando solo esta. */
int _comparar_claves(TIPOCLAVE cl1, TIPOCLAVE cl2) {
    return !strcmp(cl1, cl2) ? 0 : strcmp(cl1, cl2) > 0 ? 1 : -1;
}

/* Si tipoelem tiene alguna estructura que necesite 
 * destruirse ha de hacerse aqui. El uso de esta funcion
 * permite hacer mas eficiente la destruccion del arbol.*/
void _destruir_elem(TIPOELEMENTOABB *E) {
    destruirLista(&E->listaVacunas);
}

/////////////////////////// FIN PARTE MODIFICABLE
/////////////////////////////////////////////////////////////

//OPERACIONES DE CREACIÓN Y DESTRUCCIÓN

void crearAbb(TABB *A) {
    *A = NULL;
}

void destruirAbb(TABB *A) {
    if (*A != NULL) {
        destruirAbb(&(*A)->izq);
        destruirAbb(&(*A)->der);
        _destruir_elem(&((*A)->info));
        free(*A);
        *A = NULL;
    }
}

//OPERACIONES DE INFORMACIÓN

unsigned esAbbVacio(TABB A) {
    return A == NULL;
}

TABB izqAbb(TABB A) {
    return A->izq;
}

TABB derAbb(TABB A) {
    return A->der;
}

void leerElementoAbb(TABB A, TIPOELEMENTOABB *E) {
    *E = A->info;
}
// Función privada para comparar las claves

int _comparar_clave_elem(TIPOCLAVE cl, TIPOELEMENTOABB E) {
    return _comparar_claves(cl, _clave_elem(&E));
}
//Función privada para informar si una clave está en el árbol

unsigned _es_miembro_clave(TABB A, TIPOCLAVE cl) {
    if (esAbbVacio(A)) {
        return 0;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { //cl == A->info
        return 1;
    }
    if (comp > 0) { //cl > A->info
        return _es_miembro_clave(derAbb(A), cl);
    }
    //cl < A->info
    return _es_miembro_clave(izqAbb(A), cl);
}

//Funciones públicas

unsigned esMiembroAbb(TABB A, TIPOELEMENTOABB E) {
    return _es_miembro_clave(A, _clave_elem(&E));
}

void buscarNodoAbb(TABB A, TIPOCLAVE cl, TIPOELEMENTOABB *nodo) {
    if (esAbbVacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { // cl == A->info
        *nodo = A->info;
    } else if (comp < 0) { // cl < A->info
        buscarNodoAbb(A->izq, cl, nodo);
    } else { // cl > A->info
        buscarNodoAbb(A->der, cl, nodo);
    }
}
//OPERACIONES DE MODIFICACIÓN

/* Funcion recursiva para insertar un nuevo nodo 
   en el arbol. Se presupone que no existe un nodo
   con la misma clave en el arbol. */
void insertarElementoAbb(TABB *A, TIPOELEMENTOABB E) {
    if (esAbbVacio(*A)) {
        *A = (TABB) malloc(sizeof(struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }
    TIPOCLAVE cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp > 0) {
        insertarElementoAbb(&(*A)->der, E);
    } else {
        insertarElementoAbb(&(*A)->izq, E);
    }
}

/* Funcion privada que devuelve mínimo de subárbol dcho */
TIPOELEMENTOABB _suprimir_min(TABB *A) {//Se devuelve el elemento más a la izquierda
    TABB aux;
    TIPOELEMENTOABB ele;
    if (esAbbVacio((*A)->izq)) {//Si izquierda vacía, se devuelve valor nodo actual A
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq); //se vuelve a buscar mínimo rama izquierda
    }
}

/* Funcion que permite eliminar un nodo del arbol */
void suprimirElementoAbb(TABB *A, TIPOELEMENTOABB E) {
    TABB aux;
    if (esAbbVacio(*A)) {
        return;
    }

    TIPOCLAVE cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp < 0) { //if (E < (*A)->info) {
        suprimirElementoAbb(&(*A)->izq, E);
    } else if (comp > 0) { //(E > (*A)->info) {
        suprimirElementoAbb(&(*A)->der, E);
    } else if (esAbbVacio((*A)->izq) && esAbbVacio((*A)->der)) {
        _destruir_elem(&((*A)->info));
        free(*A);
        *A = NULL;
    } else if (esAbbVacio((*A)->izq)) { // pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        _destruir_elem(&aux->info);
        free(aux);
    } else if (esAbbVacio((*A)->der)) { //pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        _destruir_elem(&aux->info);
        free(aux);
    } else { //ni derecha ni izquierda esta vacio, busco mínimo subárbol derecho
        _destruir_elem(&(*A)->info); //elimino la info pero no libero el nodo,
        //pues en su sitio voy a poner el mínimo del subárbol derecho
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}

/* Funcion privada para pasar la clave y no tener que
   extraerla del nodo en las llamadas recursivas.*/
void _modificar(TABB A, TIPOCLAVE cl, TIPOELEMENTOABB nodo) {
    if (esAbbVacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);
    if (comp == 0) {
        A->info = nodo;
    } else if (comp < 0) {
        _modificar(A->izq, cl, nodo);
    } else {
        _modificar(A->der, cl, nodo);
    }
}


/* Permite modificar el nodo extrayendo del mismo la clave */
void modificarElementoAbb(TABB A, TIPOELEMENTOABB nodo) {
    TIPOCLAVE cl = _clave_elem(&nodo);
    _modificar(A, cl, nodo);
}
