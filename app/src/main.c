/*
 * main.c
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

const int led0 = 0;
const int led1 = 1;
const int led2 = 2;
const int led3 = 3;

void update_LED_display(int count){
    LED_set(led0, (count & 0b1000) ? LED_ON : LED_OFF);
    LED_set(led1, (count & 0b0100) ? LED_ON : LED_OFF);
    LED_set(led2, (count & 0b0010) ? LED_ON : LED_OFF);
    LED_set(led3, (count & 0b0001) ? LED_ON : LED_OFF);   
}

int main(void){
    int counter = 0;
    
    if(0 > BTN_init() || 0 > LED_init()){
        return 0;
    }

    LED_set(led0, LED_OFF);
    LED_set(led1, LED_OFF);
    LED_set(led2, LED_OFF);
    LED_set(led3, LED_OFF);


    while(1){
    if(BTN_check_clear_pressed(BTN0)){
        counter++;
        if (counter > 15) {
            counter = 0;
        }
        update_LED_display(counter);
    } 
    k_msleep(SLEEP_MS);
    }
    return 0;
}