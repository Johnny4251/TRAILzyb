#ifndef SPEAKER_H
#define SPEAKER_H

#include "driver/ledc.h"

/**
 * @file speaker.h
 * @brief Header file for the ESP32 speaker driver using LEDC (PWM) functionality.
 *
 * This library provides functionality to play musical notes and songs on a speaker
 * connected to the ESP32. It uses the LEDC peripheral to generate PWM signals
 * corresponding to various musical frequencies.
 */

/**
 * @brief Configuration structure for initializing the speaker.
 */
typedef struct {
    int SIGNAL_PIN;         /**< GPIO pin connected to the speaker. */
    int LEDC_RESOLUTION;    /**< Resolution of the LEDC timer (e.g., LEDC_TIMER_15_BIT). */
    uint16_t LEDC_DUTY;     /**< Duty cycle of the PWM signal, typically set to 50%. */
} speaker_in;

/**
 * @brief Represents a musical note with a frequency and duration.
 */
typedef struct {
    int frequency;          /**< Frequency of the note in Hz. Use 0 for silence. */
    int duration_ms;        /**< Duration of the note in milliseconds. */
} Note;

/**
 * @brief Initializes the speaker using the given configuration.
 *
 * Configures the LEDC timer and channel for generating PWM signals. The speaker
 * must be connected to the GPIO pin specified in the `speaker_in` structure.
 *
 * @param speaker_info Configuration structure containing speaker settings.
 */
void speaker_init(speaker_in speaker_info);

/**
 * @brief Plays a single tone on the speaker.
 *
 * Generates a PWM signal corresponding to the specified frequency and duration.
 * If the frequency is 0, the speaker will remain silent for the duration.
 *
 * @param freq Frequency of the tone in Hz. Use 0 for silence.
 * @param duration_ms Duration of the tone in milliseconds.
 */
void play_tone(int freq, int duration_ms);

/**
 * @brief Plays a sequence of notes (a song) on the speaker.
 *
 * Iterates through an array of `Note` structures and plays each note sequentially.
 *
 * @param song Pointer to an array of `Note` structures.
 * @param length Number of notes in the array.
 */
void play_song(const Note *song, size_t length);

/**
 * @brief Stops the speaker and disables the LEDC channel.
 *
 * This function stops the PWM signal generation and ensures the speaker is silent.
 */
void stop_speaker();

extern const Note happy_birthday[];      /**< The "Happy Birthday" song. */
extern const Note fur_elise[];           /**< A simplified version of "Für Elise" by Beethoven. */
extern const Note twinkle_twinkle[];     /**< The classic "Twinkle Twinkle Little Star." */
extern const Note basic_tune[];          /**< A basic melody for testing purposes. */


/**
 * @brief Lengths of predefined songs.
 */
extern const size_t happy_birthday_len;
extern const size_t fur_elise_len;
extern const size_t twinkle_twinkle_len;
extern const size_t basic_tune_len;


#endif // SPEAKER_H
