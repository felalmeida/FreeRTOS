/*******************************************************************************
* Modulo:       FreeRTOS.hpp                                                   *
* Inicio:       02-Jun-2024                                                    *
* Atualizacao:  04-Jun-2024                                                    *
* Autor:        Felipe Almeida                                                 *
* Descricao:    FreeRTOS Example Header                                        *
*******************************************************************************/
#ifndef FreeRTOS_Example
#define FreeRTOS_Example

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdint>

/* Standard includes. */
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

/* Pico includes. */
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

using namespace std;

static QueueHandle_t nQueue = NULL;
static QueueHandle_t cQueue = NULL;
static SemaphoreHandle_t xMutex = NULL;

const char *cSep = "|";
uint nGlobalCount = 0;

struct task_params_t {
    uint    nTaskID;
    char    cTaskName[10];
    uint    nTaskDelay;
    uint    nTaskPriority;
};

void vTask01(void *v_param);

#endif // FreeRTOS_Example
