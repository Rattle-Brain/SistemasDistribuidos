#ifndef __UTIL_H__

#include "srvdns.h"

#define CIERTO            1
#define FALSO             0
#define TAMLINEA       1024
#define TAMMSG         1024
#define NOENCONTRADO     -1

int valida_numero(char *str);
int valida_ip(char *ip);
double randRange(double min, double max);
void log_debug(char *msg);
void mostrar_recuento_consultas(int nfilas, int ncols, Lista *nomdominios,Lista *nomtiposrec,int **valores);
int es_MX_o_NS(char *tiporecord);

// Funciones auxiliares para el manejo de las listas enlazadas
int obtener_longitud_lista(Lista *p);
char *obtener_dato_en_posicion(int n, Lista *p);
int posicion_en_lista(char *cad, Lista *p);

#define __UTIL_H__
#endif
