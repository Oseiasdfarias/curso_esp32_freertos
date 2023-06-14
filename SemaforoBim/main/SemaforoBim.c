#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

SemaphoreHandle_t semaforoBinario;

void conexao_servidor(void *params)
{
    while (true)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        xSemaphoreGive(semaforoBinario);
        ESP_LOGI("Servidor", "Conectado ao servidor");
    }
}

void processa_dados(void *params)
{
    while (true)
    {
        // Aguada conexão com a internet
        xSemaphoreTake(semaforoBinario, portMAX_DELAY);
        printf("Página carregada!\n");
    }
}

void app_main(void)
{
    semaforoBinario = xSemaphoreCreateBinary();
    xTaskCreate(&conexao_servidor, "conexao com servidor", 2048, NULL, 1, NULL);
    xTaskCreate(&processa_dados, "processa dados", 2048, NULL, 1, NULL);
}
