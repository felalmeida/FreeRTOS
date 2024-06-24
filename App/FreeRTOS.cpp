/*******************************************************************************
* Modulo:       FreeRTOS.cpp                                                   *
* Inicio:       02-Jun-2024                                                    *
* Atualizacao:  06-Jun-2024                                                    *
* Autor:        Felipe Almeida                                                 *
* Descricao:    FreeRTOS Example                                               *
*******************************************************************************/
#include "FreeRTOS.hpp"

using namespace std;

int main(void) {
    int RETURN_STATUS = 0;
    uint nQueueStartValue = 0, nDelay = 1;
    char cQueueStartValue[20] = "";

    BaseType_t xTaskReturn = (long int) NULL;

    stdio_init_all();
    sleep_ms(5000);

    nQueue = xQueueCreate(1, sizeof(uint));
    cQueue = xQueueCreate(1, sizeof(char[20]));
    xMutex = xSemaphoreCreateMutex();

    xQueueSend(nQueue, &nQueueStartValue, portMAX_DELAY);
    xQueueSend(cQueue, &cQueueStartValue, portMAX_DELAY);

    task_params_t pTaskParamArr[10] {
        {1, "vTask01",(1 *nDelay), 1*1},
        {2, "vTask02",(2 *nDelay), 2*1},
        {3, "vTask03",(3 *nDelay), 3*1},
        {4, "vTask04",(4 *nDelay), 4*1},
        {5, "vTask05",(5 *nDelay), 5*1},
        {6, "vTask06",(6 *nDelay), 6*1},
        {7, "vTask07",(7 *nDelay), 7*1},
        {8, "vTask08",(8 *nDelay), 8*1},
        {9, "vTask09",(9 *nDelay), 9*1},
        {10,"vTask10",(10*nDelay),10*1}
    };

    for (int nTaskIdx = 0; nTaskIdx < (sizeof(pTaskParamArr)/sizeof(task_params_t)); nTaskIdx++) {
        xTaskReturn = xTaskCreate(vTask01, *(&pTaskParamArr[nTaskIdx].cTaskName), 1024, &pTaskParamArr[nTaskIdx], *(&pTaskParamArr[nTaskIdx].nTaskPriority), NULL);
        if(xTaskReturn != pdPASS) {
            printf("Task %s could not be created\n",pTaskParamArr[nTaskIdx].cTaskName);
            printf("xTaskReturn = %s\n",xTaskReturn);
            RETURN_STATUS = -1;
        }
    }

    if (RETURN_STATUS == 0) vTaskStartScheduler();

    return RETURN_STATUS;
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
