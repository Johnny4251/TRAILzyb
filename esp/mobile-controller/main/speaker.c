#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

// Macros for all notes (frequencies in Hz)
#define NOTE_C0 16
#define NOTE_CS0 17
#define NOTE_D0 18
#define NOTE_DS0 19
#define NOTE_E0 21
#define NOTE_F0 22
#define NOTE_FS0 23
#define NOTE_G0 25
#define NOTE_GS0 26
#define NOTE_A0 28
#define NOTE_AS0 29
#define NOTE_B0 31

#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62

#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123

#define NOTE_C3 130
#define NOTE_CS3 138
#define NOTE_D3 146
#define NOTE_DS3 155
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247

#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494

#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988

#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976

#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951

#define NOTE_REST 0 // Silence

#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_TIMER LEDC_TIMER_0

typedef struct {
    int SIGNAL_PIN;
    int LEDC_RESOLUTION;
    uint16_t LEDC_DUTY;
} speaker_in;

typedef struct {
    int frequency;   // Frequency of the note in Hz
    int duration_ms; // Duration of the note in milliseconds
} Note;

static speaker_in spk_in;
ledc_timer_config_t ledc_timer;
ledc_channel_config_t ledc_channel;

void speaker_init(speaker_in speaker_info) {
    spk_in = speaker_info;

    if (spk_in.SIGNAL_PIN <= 0) {
        printf("Invalid SIGNAL_PIN provided\n");
        return;
    }
    spk_in.LEDC_DUTY = (1 << (spk_in.LEDC_RESOLUTION - 1)); // 50% 

    ledc_timer = (ledc_timer_config_t){
        .duty_resolution = spk_in.LEDC_RESOLUTION,
        .freq_hz = 1000, // Placeholder
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .clk_cfg = LEDC_AUTO_CLK
        };
    ledc_timer_config(&ledc_timer);

    ledc_channel = (ledc_channel_config_t){
        .channel = LEDC_CHANNEL,
        .duty = 0,
        .gpio_num = spk_in.SIGNAL_PIN,
        .speed_mode = LEDC_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER
    };
    ledc_channel_config(&ledc_channel);
}

void play_tone(int freq, int duration_ms) {
    if (freq > 0) {
        ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, spk_in.LEDC_DUTY);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    } else {
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0); // Silence
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    }
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
}

void play_song(const Note *song, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        play_tone(song[i].frequency, song[i].duration_ms);
    }
}

void stop_speaker() {
    ledc_stop(LEDC_MODE, LEDC_CHANNEL, 0);
}

/* Song examples */
const Note happy_birthday[] = {
    {NOTE_C4, 250}, {NOTE_C4, 250}, {NOTE_D4, 500}, {NOTE_C4, 500}, {NOTE_F4, 500}, {NOTE_E4, 1000}, {NOTE_REST, 250},
    {NOTE_C4, 250}, {NOTE_C4, 250}, {NOTE_D4, 500}, {NOTE_C4, 500}, {NOTE_G4, 500}, {NOTE_F4, 1000}, {NOTE_REST, 250},
    {NOTE_C4, 250}, {NOTE_C4, 250}, {NOTE_C5, 500}, {NOTE_A4, 500}, {NOTE_F4, 500}, {NOTE_E4, 500}, {NOTE_D4, 1000}, {NOTE_REST, 250},
    {NOTE_AS4, 250}, {NOTE_AS4, 250}, {NOTE_A4, 500}, {NOTE_F4, 500}, {NOTE_G4, 500}, {NOTE_F4, 1000}, {NOTE_REST, 250}
};
const size_t happy_birthday_len = sizeof(happy_birthday) / sizeof(Note);

const Note fur_elise[] = {
    {NOTE_E5, 300}, {NOTE_DS5, 300}, {NOTE_E5, 300}, {NOTE_DS5, 300}, {NOTE_E5, 300}, {NOTE_B4, 300}, {NOTE_D5, 300}, {NOTE_C5, 300},
    {NOTE_A4, 600}, {NOTE_REST, 300},

    {NOTE_C4, 300}, {NOTE_E4, 300}, {NOTE_A4, 300}, {NOTE_B4, 300},
    {NOTE_REST, 300}, {NOTE_E4, 300}, {NOTE_GS4, 300}, {NOTE_B4, 300}, {NOTE_C5, 600}, {NOTE_REST, 300},

    {NOTE_E5, 300}, {NOTE_DS5, 300}, {NOTE_E5, 300}, {NOTE_DS5, 300}, {NOTE_E5, 300}, {NOTE_B4, 300}, {NOTE_D5, 300}, {NOTE_C5, 300},
    {NOTE_A4, 600}, {NOTE_REST, 300},

    {NOTE_C4, 300}, {NOTE_E4, 300}, {NOTE_A4, 300}, {NOTE_B4, 300},
    {NOTE_REST, 300}, {NOTE_E4, 300}, {NOTE_C5, 300}, {NOTE_B4, 300}, {NOTE_A4, 600},
};
const size_t fur_elise_len = sizeof(fur_elise) / sizeof(Note);

const Note twinkle_twinkle[] = {
    {NOTE_C4, 500}, {NOTE_C4, 500}, {NOTE_G4, 500}, {NOTE_G4, 500}, {NOTE_A4, 500}, {NOTE_A4, 500}, {NOTE_G4, 1000},
    {NOTE_F4, 500}, {NOTE_F4, 500}, {NOTE_E4, 500}, {NOTE_E4, 500}, {NOTE_D4, 500}, {NOTE_D4, 500}, {NOTE_C4, 1000},
    {NOTE_G4, 500}, {NOTE_G4, 500}, {NOTE_F4, 500}, {NOTE_F4, 500}, {NOTE_E4, 500}, {NOTE_E4, 500}, {NOTE_D4, 1000},
    {NOTE_G4, 500}, {NOTE_G4, 500}, {NOTE_F4, 500}, {NOTE_F4, 500}, {NOTE_E4, 500}, {NOTE_E4, 500}, {NOTE_D4, 1000},
    {NOTE_C4, 500}, {NOTE_C4, 500}, {NOTE_G4, 500}, {NOTE_G4, 500}, {NOTE_A4, 500}, {NOTE_A4, 500}, {NOTE_G4, 1000},
    {NOTE_F4, 500}, {NOTE_F4, 500}, {NOTE_E4, 500}, {NOTE_E4, 500}, {NOTE_D4, 500}, {NOTE_D4, 500}, {NOTE_C4, 1000}
};
const size_t twinkle_twinkle_len = sizeof(twinkle_twinkle) / sizeof(Note);

const Note basic_tune[] = {
    {NOTE_C4, 300}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_C5, 900}, {NOTE_B4, 300}, {NOTE_G4, 300}, {NOTE_E4, 300},
    {NOTE_C4, 900}, {NOTE_G3, 300}, {NOTE_B3, 300}, {NOTE_C4, 900}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_B4, 900},
    {NOTE_A4, 300}, {NOTE_C5, 300}, {NOTE_B4, 300}, {NOTE_G4, 900}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_C4, 900},
    {NOTE_C4, 300}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_C5, 900}, {NOTE_B4, 300}, {NOTE_G4, 300}, {NOTE_E4, 300},
    {NOTE_C4, 900}, {NOTE_G3, 300}, {NOTE_B3, 300}, {NOTE_C4, 900}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_B4, 900},
    {NOTE_A4, 300}, {NOTE_C5, 300}, {NOTE_B4, 300}, {NOTE_G4, 900}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_C4, 900},
    {NOTE_F4, 300}, {NOTE_A4, 300}, {NOTE_C5, 300}, {NOTE_F5, 900}, {NOTE_E5, 300}, {NOTE_C5, 300}, {NOTE_A4, 300},
    {NOTE_F4, 900}, {NOTE_D4, 300}, {NOTE_F4, 300}, {NOTE_A4, 900}, {NOTE_C5, 300}, {NOTE_F5, 300}, {NOTE_E5, 900},
    {NOTE_D5, 300}, {NOTE_F5, 300}, {NOTE_E5, 300}, {NOTE_C5, 900}, {NOTE_A4, 300}, {NOTE_C5, 300}, {NOTE_F4, 900},
    {NOTE_C4, 300}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_C5, 900}, {NOTE_B4, 300}, {NOTE_G4, 300}, {NOTE_E4, 300},
    {NOTE_C4, 900}, {NOTE_G3, 300}, {NOTE_B3, 300}, {NOTE_C4, 900}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_B4, 900},
    {NOTE_A4, 300}, {NOTE_C5, 300}, {NOTE_B4, 300}, {NOTE_G4, 900}, {NOTE_E4, 300}, {NOTE_G4, 300}, {NOTE_C4, 900}
};
const size_t basic_tune_len = sizeof(basic_tune) / sizeof(Note);