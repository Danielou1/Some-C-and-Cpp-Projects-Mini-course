/***************************************************************************
 * @file main.h
 * @brief Header-Datei fr das Steuerungsprogramm der LEDs und Zhler.
 *
 * Diese Datei enthlt die Funktionsprototypen und Makros fr die Steuerung
 * von LEDs, Zhlern und verschiedenen anderen LED-bezogenen Aufgaben.
 * Die Funktionen und Makros werden zur Implementierung von Programmen 
 * verwendet, die auf einem Mikrocontroller basieren und die LED-Steuerung
 * ber verschiedene Eingabemethoden ermglichen.
 * 
 * @author Mounsande Danielou Ubueng
 * @date 22.11.2024
 ******************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <cstdlib> // Changed from <stdlib.h> for C++

// Makros fr die Steuerung der LED-Pins
#define F_CPU 100000UL
#define RICHTUNG_RECHTS 1
#define RICHTUNG_LINKS 0
#define WAITING_FOR_DEBOUNCING 15000UL
#define  MOVING_LIGHT_SPEED 50000UL
#define  BINARY_COUNTER_SPEED 25000UL
#define PIN4_BIS_7 PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm
#define ALL_LEDS PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm

// Funktionsprototypen
extern void busy_wait(volatile unsigned long delay);
void light_up(); // Removed void from parameter list for C++
void blinky_one_led(); // Removed void from parameter list for C++
void blinky_two_leds(); // Removed void from parameter list for C++
void moving_light(); // Removed void from parameter list for C++
void binary_counter(); // Removed void from parameter list for C++
void gray_code(); // Removed void from parameter list for C++
void light_by_buton(); // Removed void from parameter list for C++
void light_all_leds_by_button(); // Removed void from parameter list for C++
void debounced_counter(); // Removed void from parameter list for C++
void binary_calculator(); // Removed void from parameter list for C++
void button_by_light(); // Removed void from parameter list for C++



#endif