#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAGT1 = "TASK1";
static const char *TAGT2 = "TASK2";

/* Tarefa 1 - Será criada uma task para executa-la*/
void task1(void *params)
{
    while (true)
    {
        ESP_LOGI(TAGT1, "Leitura de sensor!");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/* Tarefa 2 - Será criada uma task para executa-la*/
void task2(void *params)
{
    while (true)
    {
        ESP_LOGI(TAGT2, "Escrever no Display!");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(&task1, "leitura", 2048, "Task 1", 1, NULL, 1);
    xTaskCreate(&task2, "escrita", 2048, "Task 2", 1, NULL);
}
