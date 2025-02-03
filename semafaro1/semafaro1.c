#include <stdio.h>
#include "pico/stdlib.h"   // Inclui a biblioteca padrão para funcionalidades básicas como GPIO, temporização e comunicação serial.
#include "hardware/timer.h" // Inclui a biblioteca para gerenciamento de temporizadores de hardware.

#define VERDE 11 
#define AMARELO 12
#define VERMELHO 13

int estado_atual = 1;
int tempo = 3000;
// Função de callback que será chamada repetidamente pelo temporizador
// O tipo bool indica que a função deve retornar verdadeiro ou falso para continuar ou parar o temporizador.
void verifica_estado(){
  switch(estado_atual){
    case 1:
      gpio_put(VERMELHO, 1);
      gpio_put(AMARELO, 0);
      gpio_put(VERDE, 0);
      break;
    case 2:
      gpio_put(VERMELHO, 0);
      gpio_put(AMARELO, 1);
      gpio_put(VERDE, 0);
      break;
    case 3:
      gpio_put(VERMELHO, 0);
      gpio_put(AMARELO, 0);
      gpio_put(VERDE, 1);
      break;
}}
bool repeating_timer_callback(struct repeating_timer *t) {
    estado_atual++;
    if(estado_atual > 3){
      estado_atual = 1;
    }
    verifica_estado();
    // Retorna true para manter o temporizador repetindo. Se retornar false, o temporizador para.
    return true;
}

int main() {
    stdio_init_all();
    // Inicializar os pinos GPIO11, GPIO12, GPIO13
    gpio_init(VERDE);
    gpio_set_dir(VERDE,true);
    gpio_init(AMARELO);
    gpio_set_dir(AMARELO,true);
    gpio_init(VERMELHO);
    gpio_set_dir(VERMELHO,true);
    verifica_estado();
    struct repeating_timer timer;

    // Configura o temporizador para chamar a função de callback a cada 3 segundos.
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop infinito que mantém o programa em execução.
    while (true) {
        printf("Passou-se 1 segundo\n");
        sleep_ms(1000);
    }
    return 0;
}