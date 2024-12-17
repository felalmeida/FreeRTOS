/*******************************************************************************
* Modulo:       FreeRTOS.hpp                                                   *
* Inicio:       02-Jun-2024                                                    *
* Atualizacao:  04-Jun-2024                                                    *
* Autor:        Felipe Almeida                                                 *
* Descricao:    FreeRTOS Header                                                *
*******************************************************************************/
#define PICO_PLATFORM   rp2040
#define PICO_BOARD      pico

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
#include <time.h>

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
#include "pico/time.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

/* Lib includes. */
#include "../Lib/WS2812.hpp"

using namespace std;

static QueueHandle_t nQueue = NULL;
static QueueHandle_t cQueue = NULL;
static SemaphoreHandle_t xMutex = NULL;

const uint WS2812B_PIN = 16;
const uint WS2812B_LENGTH = 1;
const char *cSep = "|";
uint nGlobalCount = 0, nDelay = 1;
WS2812 WS2812Led(WS2812B_PIN, WS2812B_LENGTH, pio0, 0, WS2812::FORMAT_GRB);

struct task_params_t {
    uint    nTaskID;
    char    cTaskName[10];
    uint    nTaskDelay;
    uint    nTaskPriority;
};

int GenerateRandomInt (int v_MaxValue);
void vTask01(void *v_param);
void RandColors(void *v_param);
void LoopColors(void *v_param);

#endif // FreeRTOS_Example
