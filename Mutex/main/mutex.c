#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static const char *TAGI2C = "I2C";
static const char *TAGLER = "Leitura";
static const char *TAGESC = "Escrita";

int ler = 1;
int escrever = 0;

float temperatura = 0;
SemaphoreHandle_t mutexI2C;

float acessa_i2c(int cmd)
{
    if (cmd == 1)
    {
        ESP_LOGI(TAGI2C, "Leitura do sensor de temperatura!");
        return 20.0 + ((float)rand() / (float)(RAND_MAX / 10));
    }
    else
    {
        ESP_LOGI(TAGI2C, "Escrita no LCD!");
        printf("Tela LCD - Temperatura = %f\n", temperatura);
    }
    return 0;
}

void ler_sensor(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS))
        {
            temperatura = acessa_i2c(*(int *)params);
            xSemaphoreGive(mutexI2C);
            ESP_LOGI(TAGLER, "Temperatura = %f", temperatura);
        }
        else
        {
            ESP_LOGE("NLER", "Leitura não efetuada");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void escrever_lcd(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS))
        {
            temperatura = acessa_i2c(*(int *)params);
            xSemaphoreGive(mutexI2C);
            ESP_LOGI(TAGESC, "Escreveu no LCD");
        }
        else
        {
            ESP_LOGE("NLER", "Escrita no LCD não efetuada");
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    mutexI2C = xSemaphoreCreateMutex();
    xTaskCreate(&ler_sensor, "Leitura sensor", 2048, &ler, 1, NULL);
    xTaskCreatePinnedToCore(&escrever_lcd, "Escreve LCD", 2048, &escrever, 2, NULL, 1);
}
