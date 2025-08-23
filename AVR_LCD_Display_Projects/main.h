/**
 * @file main.h
 * @brief Header-Datei fr LCD-basierte Projekte.
 *
 * @details
 * Diese Datei enthlt Makros, Includes und Funktionsprototypen fr verschiedene 
 * Projekte, die das LCD und die AVR-Hardware nutzen. Funktionen umfassen:
 * - Textausgabe auf dem LCD
 * - Zhler und einfache Animationen
 * - Binerer Taschenrechner mit Tastensteuerung.
 *
 * @author Danielou Mouns
 * @date 28. November 2024
 */

#ifndef MAIN_H
#define MAIN_H

#define F_CPU 4000000            /**< CPU-Taktfrequenz in Hz. */
#define WAIT 500                 /**< Wartezeit in Millisekunden. */
#define LCD_WIDTH 16             /**< Breite des LCD in Zeichen. */
#define WAITING_FOR_DEBOUNCING 100 /**< Entprellungszeit in Millisekunden. */
#define PIN4_BIS_7 PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm /**< Bitmaske fr die Tasten. */

#include <avr/io.h>
#include <I2C_LCD.h>
#include <util/delay.h>
#include <stdbool.h> // Keep for bool type if not using C++ <cstdbool>

/**
 * @brief Gibt "Hello Display" auf dem LCD aus.
 */
void print_hello(); // Removed void from parameter list for C++

/**
 * @brief Zeigt einen Zhler von 0 bis 1000 auf dem LCD an.
 */
void count_up(); // Removed void from parameter list for C++

/**
 * @brief Animation eines Ping-Pong-Effekts auf dem LCD.
 */
void ping_pong(); // Removed void from parameter list for C++

/**
 * @brief Konvertiert eine Ganzzahl in eine Zeichenkette.
 * 
 * @param buf Puffer fr die Ausgabe.
 * @param num Die Zahl, die umgewandelt werden soll.
 * @param base Die Basis (z. B. 10 fr Dezimal, 2 fr Biner).
 * @return Zeiger auf den Puffer.
 */
extern char* integer_to_string(char *buf, int32_t num, int base);

/**
 * @brief Binerer Taschenrechner mit LCD-Ausgabe und Tastensteuerung.
 */
void binary_calculator_lcd(); // Removed void from parameter list for C++

#endif