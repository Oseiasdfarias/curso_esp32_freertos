#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

QueueHandle_t fila_de_temperaturas;
float temperatura_media;

void leitura_de_sensor(void *params)
{
    float temperatura;
    while (1)
    {
        temperatura = 20.0 + ((float)rand() / (float)(RAND_MAX / 10));
        long resposta = xQueueSend(fila_de_temperaturas, &temperatura, 1000 / portTICK_PERIOD_MS);
        if (resposta)
        {
            ESP_LOGI("Leitura", "Temperatura adicionada a fila.");
        }
        else
        {
            ESP_LOGE("Leitura", "Falha ao adicionar temperatura a fila.");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void calcula_media_movel(void *params)
{
    float temperatura = 0.0;
    float temperatura_anterior = 0.0;
    while (1)
    {
        if (xQueueReceive(fila_de_temperaturas, &temperatura, 5000 / portTICK_PERIOD_MS))
        {
            temperatura_media = (temperatura_anterior + temperatura) / 2.0;
            temperatura_anterior = temperatura;
            printf("Temperatura média: %f\n", temperatura_media);
        }
        else
        {
            ESP_LOGE("Calculo", "Fila sem dados para processar.");
        }
    }
}

void app_main(void)
{
    fila_de_temperaturas = xQueueCreate(5, sizeof(float));
    xTaskCreate(&leitura_de_sensor, "ler Temperatura",
                2048, NULL, 1, NULL);

    xTaskCreate(&calcula_media_movel, "calcula média Temperatura",
                2048, NULL, 2, NULL);
}
