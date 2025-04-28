#include <stdio.h>

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#include "esp_log.h"
#include "driver/gpio.h"

/* Macros */
#define DEBOUNCE_DELAY_MS 30
#define TASK_DELAY 5

#define V_LOW  0
#define V_HIGH 1

#define A_BUTTON_PIN GPIO_NUM_4
#define B_BUTTON_PIN GPIO_NUM_2
#define RED_PIN      GPIO_NUM_19
#define YELLOW_PIN   GPIO_NUM_18
#define GREEN_PIN    GPIO_NUM_5
#define BUZZER_PIN   GPIO_NUM_23

#define machine_TASK "STATE MACHINE"

/* States */
typedef enum {
    S0,     // off
    S1,     // G
    S2,     // Y
    S3,     // YG
    S4,     // R
    S5,     // RG
    S6,     // RY
    S7     // RYG
} led_state_t;

/* Events */
typedef enum {
    BUTTON_A,
    BUTTON_B,
    NOTHING
} button_event_t;

typedef struct state_machine_t {
    led_state_t state;
} state_machine_t;

/* Prototypes */
led_state_t transition(led_state_t state, button_event_t event);
led_state_t transition_ORIGINAL(led_state_t state, button_event_t event);
int check_for_input(const int PIN);
button_event_t event_check();
void machine(void *pvParams);
void change_colors(const int code);
void app_main(void);

led_state_t transition_ORIGINAL(led_state_t state, button_event_t event) {
    /* If nothing happened, then do nothing :) */
    if (event == NOTHING) {
        change_colors(state);
        return state;
    }

    const char* TAG = pcTaskGetName(NULL);
    led_state_t next_state = state;
    switch(state) {
        case S0:
            if(event == BUTTON_A) {
                next_state = S1;
            } else if(event == BUTTON_B) {
                next_state = S2;
            } else {
                /* Do nothing */
            }
            break;
        case S1:
            if(event == BUTTON_A) {
                next_state = S0;
            } else if(event == BUTTON_B) {
                next_state = S2;
            } else {
                /* Do nothing */
            }
            break;
        case S2:
            if(event == BUTTON_A) {
                next_state = S1;
            } else if(event == BUTTON_B) {
                next_state = S3;
            } else {
                /* Do nothing */
            }
            break;
        case S3:
            if(event == BUTTON_A) {
                next_state = S4;
            } else if(event == BUTTON_B) {
                next_state = S1;
            } else {
                /* Do nothing */
            }
            break;
        case S4:
            if(event == BUTTON_A) {
                next_state = S5;
            } else if(event == BUTTON_B) {
                next_state = S4;
            } else {
                /* Do nothing */
            }
            break;
        case S5:
            if(event == BUTTON_A) {
                next_state = S6;
            } else if(event == BUTTON_B) {
                next_state = S0;
            } else {
                /* Do nothing */
            }
            break;
        case S6:
            if(event == BUTTON_A) {
                next_state = S5;
            } else if(event == BUTTON_B) {
                next_state = S7;
            } else {
                /* Do nothing */
            }
            break;
        case S7:
            if(event == BUTTON_A) {
                next_state = S7;
            } else if(event == BUTTON_B) {
                next_state = S7;
            } else {
                /* Do nothing */
            }
            break;
        default:
            ESP_LOGE(TAG, "Not a valid state");
            return next_state;
    }
    change_colors(next_state);
    ESP_LOGI(TAG, "Transitioning from %d to %d due to event %d", state, next_state, event);
    return next_state;
}

led_state_t transition(led_state_t state, button_event_t event) {
    /* If nothing happened, then do nothing :) */
    if (event == NOTHING) {
        change_colors(state);
        return state;
    }

    const char* TAG = pcTaskGetName(NULL);
    led_state_t next_state = state;
    switch(state) {
        case S0:
            if(event == BUTTON_A) {
                next_state = S2;
            } else if(event == BUTTON_B) {
                next_state = S1;
            } else {
                /* Do nothing */
            }
            break;
        case S1:
            if(event == BUTTON_A) {
                next_state = S2;
            } else if(event == BUTTON_B) {
                next_state = S3;
            } else {
                /* Do nothing */
            }
            break;
        case S2:
            if(event == BUTTON_A) {
                next_state = S2;
            } else if(event == BUTTON_B) {
                next_state = S1;
            } else {
                /* Do nothing */
            }
            break;
        case S3:
            if(event == BUTTON_A) {
                next_state = S4;
            } else if(event == BUTTON_B) {
                next_state = S3;
            } else {
                /* Do nothing */
            }
            break;
        case S4:
            if(event == BUTTON_A) {
                next_state = S2;
            } else if(event == BUTTON_B) {
                next_state = S5;
            } else {
                /* Do nothing */
            }
            break;
        case S5:
            if(event == BUTTON_A) {
                next_state = S2;
            } else if(event == BUTTON_B) {
                next_state = S1;
            } else {
                /* Do nothing */
            }
            break;
        default:
            ESP_LOGE(TAG, "Not a valid state");
            return next_state;
    }
    change_colors(next_state);
    ESP_LOGI(TAG, "Transitioning from %d to %d due to event %d", state, next_state, event);
    return next_state;
}

int check_for_input(const int PIN) {
    static int last_state = V_LOW;
    int current_state = gpio_get_level(PIN);

    if (current_state == V_HIGH && last_state == V_LOW) {
        vTaskDelay(DEBOUNCE_DELAY_MS / portTICK_PERIOD_MS); 
        if (gpio_get_level(PIN) == V_HIGH) {
            last_state = V_HIGH;
            while (gpio_get_level(PIN) == V_HIGH) {
                vTaskDelay(10 / portTICK_PERIOD_MS); 
            }
            last_state = V_LOW; 
            return 1; 
        }
    } else if (current_state == V_LOW) {
        last_state = V_LOW; 
    }
    return 0; 
}


button_event_t event_check() {
    button_event_t event = NOTHING;
    if (check_for_input(A_BUTTON_PIN)) {
        event = BUTTON_A;
    }
    if (check_for_input(B_BUTTON_PIN)) {
        event = BUTTON_B;
    }
    return event;
}

void machine(void *pvParams) {
    const char* TAG = pcTaskGetName(NULL);
    ESP_LOGI(TAG, "State machine started!");
    /* Initial setup */
    state_machine_t *sm = (state_machine_t *) pvParams;
    button_event_t event = NOTHING;
    for ( ;; ) {
        /* Check for event => transition */
        event = event_check();
        sm->state = transition(sm->state, event);
        //sm->state = transition_ORIGINAL(sm->state, event);
        vTaskDelay(TASK_DELAY);
    }
}

void change_colors(const int code) {
    int R = (code >> 2) & 1;
    int Y = (code >> 1) & 1;
    int G = code & 1;

    gpio_set_level(RED_PIN, R);
    gpio_set_level(YELLOW_PIN, Y);
    gpio_set_level(GREEN_PIN, G);
}

void app_main(void) {
    /* Reset pin configurations */
    gpio_reset_pin(A_BUTTON_PIN);
    gpio_reset_pin(B_BUTTON_PIN);
    gpio_reset_pin(RED_PIN);
    gpio_reset_pin(GREEN_PIN);
    gpio_reset_pin(YELLOW_PIN);
    gpio_reset_pin(BUZZER_PIN);

    /* Configure LED GPIO */
    gpio_set_direction(RED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);

    /* Configure BUTTON GPIO */
    gpio_set_direction(A_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(A_BUTTON_PIN, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(B_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(B_BUTTON_PIN, GPIO_PULLDOWN_ONLY);

    state_machine_t sm;
    sm.state = S0;
    /* Start state machine */
    TaskHandle_t m_task = NULL;
    xTaskCreate(&machine, machine_TASK, 2048, (void *) &sm, 5, &m_task);
    for ( ;; ) {
        if (sm.state == S5) {
            gpio_set_level(BUZZER_PIN, 1);
        } else {
            gpio_set_level(BUZZER_PIN, 0);
        }
        vTaskDelay(20);
    }
}
