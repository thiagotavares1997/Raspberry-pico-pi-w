#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/adc.h"

#define SPI_PORT spi0
#define BAUD_RATE 115200            // Velocidade de operação da SPI em Hz
#define CS_PIN 17                   // Pino GPIO para o Chip Select (CS)
#define ADC_PIN 26                  // Pino GPIO para a leitura do ADC

// Função para inicializar a comunicação SPI
void init_spi() {
    spi_init(SPI_PORT, BAUD_RATE);                                          // Inicializa a SPI com a taxa de baud especificada
    gpio_set_function(CS_PIN, GPIO_FUNC_SPI);                               // Configura o pino CS como pino SPI
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);     // Configura o formato da comunicação SPI
    spi_set_slave(SPI_PORT, false);                                         // Configura o Raspberry Pi Pico como mestre
    spi_set_slave(SPI_PORT, true);                                          // Ativa a comunicação SPI
}

// Função para ler a temperatura da CPU
float read_temp() {
    adc_init();                                         // Inicializa o módulo ADC (Conversor Analógico-Digital)
    adc_gpio_init(ADC_PIN);                             // Configura o pino GPIO para a leitura do ADC
    adc_select_input(4);                                // Seleciona o canal de entrada do ADC
    
    int raw_value = adc_read();                         // Lê o valor analógico do sensor
    
    float tensao = (float)raw_value / 4095.0 * 3.3;     // Converte o valor em tensão
    float temp = (tensao - 0.5) * 100.0;                // Converte a tensão em temperatura

    return temp;
}

bool per_timer_callback(struct repeating_timer *t) {
    float temp = read_temp();                           // Chama a função para ler a temperatura
    printf("Temperatura da CPU: %.2f°C\n", temp);       // Exibe a temperatura no console    
    return true;
}

int main() {
    stdio_init_all();                                       // Inicializa as funções de E/S padrão
    init_spi();                                             // Chama a função para configurar a SPI

    int tempo = 1000;                                                   //tempo da interrupção = 1 s
    struct repeating_timer timer;                                       //ponteiro do temporizador
    add_repeating_timer_ms(tempo, &per_timer_callback, NULL, &timer);   //função da temporização

    while (1) {
        tight_loop_contents();      //Garante que o código dentro do loop seja executado o mais rápido possível
    }

    return 0;
}
