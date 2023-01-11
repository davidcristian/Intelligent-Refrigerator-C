#pragma once
#include "Service.h"

typedef struct
{
	Service* serv;
} UI;

UI* createUI(Service* serv);
void destroyUI(UI* ui);

void startUI(UI* ui);
