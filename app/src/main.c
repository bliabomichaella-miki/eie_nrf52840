/*
 * main.c
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <string.h> //for memset function to clear array

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 10
#define PASSKEY_LENGTH 6

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

const int led0 = 0;
const int led1 = 1;
const int led2 = 2;
const int led3 = 3;

typedef enum{
    STATE_LOCKED = 0,
    STATE_WAITING
}state_id;

int main(void){
    state_id state_id_var = STATE_LOCKED;

    const int passwordlist[PASSKEY_LENGTH] = {1, 2, 3, 1, 2, 3};

    int usrinputlist[PASSKEY_LENGTH];
    int input_index = 0;

    if(0 > BTN_init || 0 > LED_init){
        printk("Failed to initialize modules\n");
        return 0;
    }

    LED_set(led0, LED_ON);
    memset(usrinputlist, 0, sizeof(usrinputlist));
    printk("System Locked, Enter Pasword\n");

    while(1){
        if(state_id_var == STATE_LOCKED){

            if(input_index < PASSKEY_LENGTH){
                if(BTN_check_clear_pressed(BTN0)){
                    usrinputlist[input_index] = 1;
                    printk("Input 1\n");
                    input_index++;
                }else if (BTN_check_clear_pressed(BTN1)) {
                    usrinputlist[input_index] = 2;
                    printk("Input: 2\n");
                    input_index++;
                }else if (BTN_check_clear_pressed(BTN2)) {
                    usrinputlist[input_index] = 3;
                    printk("Input: 3\n");
                    input_index++;
                }
            }

            if(BTN_check_clear_pressed(BTN3)){

                if(input_index == PASSKEY_LENGTH){
                    int correct = 1;

                    for(int j = 0; j < PASSKEY_LENGTH; j++){
                        if(passwordlist[j] != usrinputlist[j]){
                            correct = 0;
                            break;
                        }
                    }
                    if(correct){
                        printk("Correct! System Unlocked\n");
                        LED_set(led0, LED_OFF);
                        state_id_var = STATE_WAITING;
                    } else{
                        printk("Incorrect\n");
                    }

                } else{
                    printk("Not enough input\n");
                }

                memset(usrinputlist, 0, sizeof(usrinputlist));
                input_index = 0;
            }
        } else if(state_id_var == STATE_WAITING){
            
            if (BTN_check_clear_pressed(BTN0) ||
                BTN_check_clear_pressed(BTN1) ||
                BTN_check_clear_pressed(BTN2) ||
                BTN_check_clear_pressed(BTN3)) 
            {
                printk("Resetting to Locked State.\n");
                LED_set(led0, LED_ON); // Turn LED0 back ON
                state_id_var = STATE_LOCKED;  // Go back to locked state
            }
        }
        k_msleep(SLEEP_MS);
    }
    return 0;
}