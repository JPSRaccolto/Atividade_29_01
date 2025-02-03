#include <stdio.h> 
#include "pico/stdlib.h" 
#include "pico/time.h"   

const uint vermelho = 12;    // Define o pino GPIO 12 para o LED vermelho.
const uint azul = 11;        // Define o pino GPIO 11 para o LED azul.
const uint verde = 13;       // Define o pino GPIO 13 para o LED verde.
const uint BUTTON_PIN = 5;   // Define o pino GPIO 5 para ler o estado do botão.

bool led_active = false;     // Indica se o ciclo de LEDs está ativo.
int estado_atual = 1;        // Controla o estado atual dos LEDs.

void verifica_estado() {
    switch (estado_atual) {
        case 1:
            gpio_put(azul, 1);
            gpio_put(vermelho, 1);
            gpio_put(verde, 1);
            break;
        case 2:
            gpio_put(azul, 0);
            gpio_put(vermelho, 1);
            gpio_put(verde, 1);
            break;
        case 3:
            gpio_put(azul, 0);
            gpio_put(vermelho, 0);
            gpio_put(verde, 1);
            break;
        case 4:
            gpio_put(azul, 0);
            gpio_put(vermelho, 0);
            gpio_put(verde, 0);
            break;
    }
}

// Função de callback para mudar o estado dos LEDs a cada 3 segundos
int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    estado_atual++;  // Muda para o próximo estado dos LEDs
    if (estado_atual > 4) {
        led_active = false;  // Permite que o botão funcione novamente quando todos os LEDs estiverem apagados
        return 0; // O alarme não se repete
    }
    verifica_estado();  // Atualiza os LEDs para o próximo estado
    add_alarm_in_ms(3000, turn_off_callback, NULL, false);  // Reagenda o alarme para 3 segundos
    return 0;
}

int main() {
    stdio_init_all();  // Inicializa a comunicação serial para debug

    // Inicializa os pinos dos LEDs
    gpio_init(vermelho);
    gpio_set_dir(vermelho, GPIO_OUT);
    gpio_init(azul);
    gpio_set_dir(azul, GPIO_OUT);
    gpio_init(verde);
    gpio_set_dir(verde, GPIO_OUT);

    // Inicializa o pino do botão
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  // Ativa o resistor pull-up interno

    while (true) {
        // Verifica se o botão foi pressionado e os LEDs estão apagados
        if (gpio_get(BUTTON_PIN) == 0 && !led_active) {
            sleep_ms(50);  // Debounce

            // Verifica novamente o estado do botão após o debounce
            if (gpio_get(BUTTON_PIN) == 0) {
                led_active = true;  // Marca que o ciclo dos LEDs começou
                estado_atual = 1;   // Inicia o ciclo de LEDs com todos acesos
                verifica_estado();   // Atualiza os LEDs para o primeiro estado
                add_alarm_in_ms(3000, turn_off_callback, NULL, false);  // Inicia o temporizador de 3 segundos
            }
        }

        sleep_ms(10);  // Pequeno atraso para reduzir o uso da CPU
    }

    return 0;
}
