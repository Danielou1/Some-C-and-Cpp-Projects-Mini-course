/****************************************************************************
 * @file main.h
 * @brief Header-Datei fr das Steuerprogramm eines Timers und Ampelsystems.
 * 
 * Diese Datei enthlt Definitionen und Makros fr das Timer- und Ampelsystem. 
 * Sie definiert die Pins fr die RGB-LED, den Prescaler fr den Timer sowie 
 * die Ampelzustnde. Auerdem sind Funktionen fr die Umwandlung von 
 * Ganzzahlen in Strings und die LCD-Ansteuerung enthalten.
 * 
 * @author Danielou Mouns
 * @date 02.12.2024
 ******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

//#define F_CPU 4000000UL

/** @brief Inklusion der notwendigen AVR-Bibliotheken. */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cstdio> // Changed from <stdio.h> for C++
#include "I2C_LCD.h"

/** @brief RGB-LED-Pins. */
#define LED_PINS (PIN0_bm | PIN1_bm | PIN2_bm)

/** @brief Prescaler fr den Timer (1 Sekunde). */
#define PRESCALER 15625

/** @brief Entprellzeit fr den Taster (50 ms). */
#define WAITING_FOR_DEBOUNCING 50

/** @brief Prescaler fr die Ampelsteuerung (3 Sekunden). */
#define AMPEL_PRESCALER 46875

/** @brief Zustnde der Ampelsteuerung. */
#define RED 0
#define YELLOW_TO_GREEN 1
#define GREEN 2
#define YELLOW_TO_RED 3

/** @brief Funktionsprototyp fr die Ganzzahl-zu-String-Konvertierung. */
char* integer_to_string(char *buf, int32_t num, int base); // Removed void from parameter list for C++

#endif