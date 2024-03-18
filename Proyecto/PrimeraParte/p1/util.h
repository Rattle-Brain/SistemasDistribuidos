#ifndef __UTIL_H__

#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <string.h>

#define FALSO 0
#define CIERTO 1
#define TAMMSG 1024
#define TAMPANTALLA 1024
#define MAXDOMNAMELENGTH 253

int valida_numero(char *str);
int valida_ip(char *ip);
double randRange(double min, double max);
void log_debug(char *msg);

#define __UTIL_H__
#endif
