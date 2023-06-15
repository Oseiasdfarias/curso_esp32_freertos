#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t receptorHandle = NULL;

void emissor(void *params)
{
    while (1)
    {
        xTaskNotifyGive(receptorHandle);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void receptor(void *params)
{
    while (1)
    {
        // Aguarda a notificação
        int quantidade = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Notificações recebidas: %d\n", quantidade);
    }
}

void app_main(void)
{
    xTaskCreate(&receptor, "Receptor", 2048, NULL, 2, &receptorHandle);
    xTaskCreate(&emissor, "Emissor", 2048, NULL, 2, NULL);
    xTaskCreate(&emissor, "Emissor 2", 2048, NULL, 2, NULL);
}
