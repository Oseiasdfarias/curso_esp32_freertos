#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_log.h"


EventGroupHandle_t eventGroupConectividade;
const int conexao_WiFi = BIT0;
const int conexao_MQTT = BIT1;

SemaphoreHandle_t ligarMQTT;


void conectaWifi(void *params)
{
    while (1)
    {
        ESP_LOGI("Wifi", "Conectado ao Wifi!");
        xEventGroupSetBits(eventGroupConectividade, conexao_WiFi);
        xSemaphoreGive(ligarMQTT);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}


void conectaMQTT(void *params)
{
    while (1)
    {
        /* Conecta ao servidor MQTT caso o semaforo esteja liberado,
        caso contrario esepera até que esteja*/
        xSemaphoreTake(ligarMQTT, portMAX_DELAY);
        ESP_LOGI("MQTT", "Conectado ao servidor MQTT");
        xEventGroupSetBits(eventGroupConectividade, conexao_MQTT);
    }
}


void processar_dados(void *params)
{
    while (1)
    {
        /* Aguarda a conexão WiFi e MQTT*/
        xEventGroupWaitBits(eventGroupConectividade,
                            conexao_WiFi | conexao_MQTT,
                            true, true, portMAX_DELAY);
        printf("\nProcessando dados\n\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP_LOGI("TASK", "Desconecta da internet\n");
    }
}


void app_main(void)
{
    eventGroupConectividade = xEventGroupCreate();
    ligarMQTT = xSemaphoreCreateBinary();

    xTaskCreate(&conectaWifi, "Conecta ao WiFi", 2048, NULL, 1, NULL);
    xTaskCreate(&conectaMQTT, "Conecta ao MQTT", 2048, NULL, 1, NULL);
    xTaskCreate(&processar_dados, "Processa Dados", 2048, NULL, 1, NULL);
}
