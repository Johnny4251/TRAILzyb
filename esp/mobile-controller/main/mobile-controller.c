#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "speaker.h"

#define MAIN_TAG "MAIN"
#define MACHINE_TAG "STATE MACHINE"

#define PICTURE_INTERVAL_MIN 0.5//1 // 12 * 60
#define RELAY_INTERVAL_MIN   0.5//1 // 5
#define ZYB_FIRING_MIN       0.5//1 // 20

#define TASK_DELAY 5
#define DEBOUNCE_DELAY_MS 30

#define V_LOW  0
#define V_HIGH 1

#define ABORT_BUTTON_PIN GPIO_NUM_2
#define START_BUTTON_PIN GPIO_NUM_4

#define RELAY_PIN    GPIO_NUM_19
#define ZYB_FIRE_PIN GPIO_NUM_5
#define NOT_LED      GPIO_NUM_22
#define SPEAKER_PIN  GPIO_NUM_23

/* States */
typedef enum {
    START,          // initial power on state
    WAITING,        
    POWER_RELAY,    // state to give relay power
    ZYB_FIRING      // state to send FIRE signal to station

} mState_t;

/* Events */
typedef enum {
    START_BTN_DOWN,     // initial power on state
    ABORT_BTN_DOWN,     // go back to start
    ZYB_TIME,           // time to fire zyb
    RELAY_ON,           // activate relay => turns on PI
    ZYB_FIRED,          // done transmitting
    NOTHING
    
} mEvent_t;

/* State Machine */
typedef struct state_machine_t {
    mState_t state;
} state_machine_t;

/* Prototypes */
mState_t transition(mState_t state, mEvent_t event);
mEvent_t event_check();
void machine(void *pvParams);
int check_for_input(const int PIN);
void reset_pins(void);
void app_main(void);

mState_t transition(mState_t state, mEvent_t event) {
    /* If nothing happened, then do nothing :) */
    if (event == NOTHING) {
        return state;
    }
    
    const char* TAG = pcTaskGetName(NULL);
    mState_t next_state = state;

    switch(state) {
        case START:
            if(event == START_BTN_DOWN) {
                next_state = WAITING;
            } else if(event == ABORT_BTN_DOWN) {
                /* Do nothing */
            } else if(event == ZYB_TIME) {
                /* Do nothing */
            } else if(event == RELAY_ON) {
                /* Do nothing */
            } else if(event == ZYB_FIRED) {
                /* Do nothing */
            } else {
                /* Do nothing */
            }
            break;
        case WAITING:
            if(event == START_BTN_DOWN) {
                /* Do nothing */
            } else if(event == ABORT_BTN_DOWN) {
                next_state = START;
            } else if(event == ZYB_TIME) {
                next_state = POWER_RELAY;
            } else if(event == RELAY_ON) {
                /* Do nothing */
            } else if(event == ZYB_FIRED) {
                /* Do nothing */
            } else {
                /* Do nothing */
            }
            break;
        case POWER_RELAY:
            if(event == START_BTN_DOWN) {
                /* Do nothing */
            } else if(event == ABORT_BTN_DOWN) {
                next_state = START;
            } else if(event == ZYB_TIME) {
                /* Do nothing */
            } else if(event == RELAY_ON) {
                next_state = ZYB_FIRING;
            } else if(event == ZYB_FIRED) {
                /* Do nothing */
            } else {
                /* Do nothing */
            }
            break;
        case ZYB_FIRING:
            if(event == START_BTN_DOWN) {
                /* Do nothing */
            } else if(event == ABORT_BTN_DOWN) {
                /* Do nothing */
            } else if(event == ZYB_TIME) {
                /* Do nothing */
            } else if(event == RELAY_ON) {
                /* Do nothing */
            } else if(event == ZYB_FIRED) {
                next_state = WAITING;
            } else {
                /* Do nothing */
            }
            break;
        default:
            ESP_LOGE(TAG, "Not a valid state");
            return next_state;
    }
    ESP_LOGI(TAG, "Transitioning from %d to %d due to event %d", state, next_state, event);
    return next_state;
}

mEvent_t event_check() {
    mEvent_t event = NOTHING;
    if (check_for_input(ABORT_BUTTON_PIN)) {
        event = ABORT_BTN_DOWN;
    }
    if (check_for_input(START_BUTTON_PIN)) {
        event = START_BTN_DOWN;
    }
    return event;
}

void machine(void *pvParams) {
    const char* TAG = pcTaskGetName(NULL);
    ESP_LOGI(TAG, "State machine started!");

    state_machine_t *sm = (state_machine_t *) pvParams;
    mState_t prev_state = sm->state;
    TickType_t state_entry_time = xTaskGetTickCount();
    for (;;) {
        mEvent_t event = event_check();
        if (sm->state != prev_state) {
            ESP_LOGI(TAG, "Entering state: %d", sm->state);
            state_entry_time = xTaskGetTickCount();
            switch (sm->state) {
                case START:
                    reset_pins();
                    break;
                case WAITING:
                    reset_pins();
                    break;
                case POWER_RELAY:
                    gpio_set_level(RELAY_PIN, V_HIGH);
                    play_song(fur_elise, fur_elise_len);
                    stop_speaker();
                    break;
                case ZYB_FIRING:
                    gpio_set_level(ZYB_FIRE_PIN, V_HIGH);
                    gpio_set_level(NOT_LED, V_HIGH);
                    play_song(twinkle_twinkle, twinkle_twinkle_len);
                    stop_speaker();
                    break;
                default:
                    ESP_LOGE(TAG, "Unknown state entered: %d", sm->state);
                    break;
            }
            prev_state = sm->state;  
        }

        TickType_t elapsed_ms = (xTaskGetTickCount() - state_entry_time) * portTICK_PERIOD_MS;
        switch (sm->state) {
            case WAITING:
            if (elapsed_ms >= PICTURE_INTERVAL_MIN * 60 * 1000) { 
                    event = ZYB_TIME;
                }
                break;
            case POWER_RELAY:
                if (elapsed_ms >= RELAY_INTERVAL_MIN * 60 * 1000) {
                    event = RELAY_ON;
                }
                break;
            case ZYB_FIRING:
                if (elapsed_ms >= RELAY_INTERVAL_MIN * 60 * 1000) {
                    event = ZYB_FIRED;
                }
                break;
            default:
                break;
        }
        mState_t next_state = transition(sm->state, event);
        if (next_state != sm->state) {
            ESP_LOGI(TAG, "State changed from %d to %d", sm->state, next_state);
            sm->state = next_state;
        }
        vTaskDelay(TASK_DELAY);
    }
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

void reset_pins(void) {
    gpio_reset_pin(RELAY_PIN);
    gpio_set_level(RELAY_PIN, V_LOW);
    gpio_reset_pin(NOT_LED);
    gpio_set_level(NOT_LED, V_LOW);
    gpio_reset_pin(ZYB_FIRE_PIN);
    gpio_set_level(ZYB_FIRE_PIN, V_LOW);
    
    gpio_reset_pin(START_BUTTON_PIN);
    gpio_reset_pin(ABORT_BUTTON_PIN);

    speaker_in s_in;
    s_in.SIGNAL_PIN = SPEAKER_PIN;
    s_in.LEDC_RESOLUTION = LEDC_TIMER_15_BIT;
    speaker_init(s_in);

    /* Configure LED GPIO */
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ZYB_FIRE_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(NOT_LED, GPIO_MODE_OUTPUT);

    /* Configure BUTTON GPIO */
    gpio_set_direction(START_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(START_BUTTON_PIN, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(ABORT_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(ABORT_BUTTON_PIN, GPIO_PULLDOWN_ONLY);
}

void app_main(void) {
    
    reset_pins();

    /* Create & Start the state machine */
    state_machine_t sm = {START};
    xTaskCreate(&machine, MACHINE_TAG, 2048, (void *) &sm, 5, NULL);

    for ( ;; ) {
        vTaskDelay(TASK_DELAY);
    }
}
