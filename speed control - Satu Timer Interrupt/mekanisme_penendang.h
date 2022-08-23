#ifndef MEKANSME_PENENDANG_H_
#define MEKANISME_PENENDANG_H_
#define ON 1
#define OFF 0

#include "stdbool.h"

void init_handle();
void init_penendang();
void handleRotateIn();
void handleRotateOut();
void handleOff();
void kickBall();
void kick(bool mode);

#endif
