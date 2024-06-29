/*******************************************************************************
* Modulo:       FreeRTOS.cpp                                                   *
* Inicio:       02-Jun-2024                                                    *
* Atualizacao:  06-Jun-2024                                                    *
* Autor:        Felipe Almeida                                                 *
* Descricao:    FreeRTOS                                                       *
*******************************************************************************/
#include "FreeRTOS.hpp"

using namespace std;

int main(void) {
    int RETURN_STATUS = 0;
    uint nQueueStartValue = 0;
    char cQueueStartValue[20] = "";

    BaseType_t xTaskReturn = (long int) NULL;

    stdio_init_all();
    sleep_ms(5000);

    /*
    nQueue = xQueueCreate(1, sizeof(uint));
    cQueue = xQueueCreate(1, sizeof(char[20]));
    xMutex = xSemaphoreCreateMutex();

    xQueueSend(nQueue, &nQueueStartValue, portMAX_DELAY);
    xQueueSend(cQueue, &cQueueStartValue, portMAX_DELAY);

    task_params_t pTaskParamArr[10] {
        {1, "vTask01",(1 *10*Delay), 1*1},
        {2, "vTask02",(2 *10*Delay), 2*1},
        {3, "vTask03",(3 *10*Delay), 3*1},
        {4, "vTask04",(4 *10*Delay), 4*1},
        {5, "vTask05",(5 *10*Delay), 5*1},
        {6, "vTask06",(6 *10*Delay), 6*1},
        {7, "vTask07",(7 *10*Delay), 7*1},
        {8, "vTask08",(8 *10*Delay), 8*1},
        {9, "vTask09",(9 *10*Delay), 9*1},
        {10,"vTask10",(10*10*Delay),10*1}
    };

    for (int nTaskIdx = 0; nTaskIdx < (sizeof(pTaskParamArr)/sizeof(task_params_t)); nTaskIdx++) {
        xTaskReturn = xTaskCreate(vTask01, *(&pTaskParamArr[nTaskIdx].cTaskName), 1024, &pTaskParamArr[nTaskIdx], *(&pTaskParamArr[nTaskIdx].nTaskPriority), NULL);
        if(xTaskReturn != pdPASS) {
            printf("Task %s could not be created\n",pTaskParamArr[nTaskIdx].cTaskName);
            printf("xTaskReturn = %s\n",xTaskReturn);
            RETURN_STATUS = -1;
        }
    }
    */

    // xTaskCreate(RandColors, "RandColors", 1024, NULL, 1, NULL);
    xTaskCreate(LoopColors, "LoopColors", 1024, NULL, 1, NULL);

    if (RETURN_STATUS == 0) vTaskStartScheduler();

    return RETURN_STATUS;
}

int GenerateRandomInt (int v_MaxValue) {
    uint32_t abs_time = get_absolute_time();

    srand((unsigned) time(NULL)+abs_time);
    return((int)((0.0+v_MaxValue)*rand()/(RAND_MAX+0.0)));
}

void vTask01(void *v_param) {
    task_params_t pTaskParam = *(task_params_t *) v_param;

    uint nQueueValue_Send = 0, nQueueValue_Recv = 0;
    char cQueueValue_Send[20] = "", cQueueValue_Recv[20] = "";
    uint nLocalCount = 0;

    while (true) {
        nLocalCount++;
        nQueueValue_Send = nLocalCount;
        sprintf(cQueueValue_Send,"%s(%d)",pTaskParam.cTaskName,nQueueValue_Send);
        if (xSemaphoreTake(xMutex, ( TickType_t ) 1) == pdTRUE) {
            nGlobalCount++;
            xQueueReceive(nQueue, &nQueueValue_Recv, portMAX_DELAY);
            xQueueReceive(cQueue, &cQueueValue_Recv, portMAX_DELAY);
            xQueueSend(nQueue,    &nQueueValue_Send, portMAX_DELAY);
            xQueueSend(cQueue,    &cQueueValue_Send, portMAX_DELAY);
            xSemaphoreGive(xMutex);
        }
        printf("%d%s%s(%d)%s%s\n",
            nGlobalCount,cSep,
            pTaskParam.cTaskName,
            nLocalCount,cSep,
            cQueueValue_Recv
        );
        vTaskDelay(pdMS_TO_TICKS(pTaskParam.nTaskDelay));
    }
    vTaskDelete(NULL);
}

void RandColors(void *v_param) {
    int nRed, nGreen, nBlue = 0;

    while (true) {
        nRed    = GenerateRandomInt(255);
        nGreen  = GenerateRandomInt(255);
        nBlue   = GenerateRandomInt(255);

        printf("Color (%d,%d,%d)\n", nRed, nGreen, nBlue);
        WS2812Led.fill(WS2812::RGB(nRed,nGreen,nBlue));
        WS2812Led.show();
        vTaskDelay(pdMS_TO_TICKS(nDelay*100));
    }
    vTaskDelete(NULL);
}

void LoopColors(void *v_param) {
    int nColorArray[3] = {0,0,0};
    while (true) {
        for (int nColorIdx = 0; nColorIdx < 3; nColorIdx++) {
            nColorArray[0] = 0;
            nColorArray[1] = 0;
            nColorArray[2] = 0;
            for (int nColor = 0; nColor < 256; nColor++) {
                nColorArray[nColorIdx] = nColor;
                printf("Color (%d,%d,%d)\n", nColorArray[0], nColorArray[1], nColorArray[2]);
                WS2812Led.fill(WS2812::RGB(nColorArray[0], nColorArray[1], nColorArray[2]));
                WS2812Led.show();
                vTaskDelay(pdMS_TO_TICKS(nDelay*10));
            }
        }
    }
    vTaskDelete(NULL);
}
