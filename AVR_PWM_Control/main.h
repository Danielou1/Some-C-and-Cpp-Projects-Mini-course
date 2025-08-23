/**
 * @file main.h
 * @brief Headerdatei fr PWM- und Servomotorsteuerung auf dem AVR128DB48.
 *
 * Diese Datei enthlt Makrodefinitionen, Bibliothekseinbindungen und Funktionsprototypen 
 * fr die Steuerung von LEDs und Servomotoren mittels PWM-Signalen.
 *
 * @author Danielou Mounsande
 * @date 09.12.2024
 */
#ifndef _MAIN_H
#define _MAIN_H
#define F_CPU 4000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <cstdio> // Changed from <stdio.h> for C++
#define RED_PIN PIN0_bm
#define GREEN_PIN PIN1_bm
#define BLUE_PIN PIN2_bm
 
/** @brief PWM-Periode in Taktzyklen (20 ms fr 50 Hz PWM). */
#define PERIOD_CYCLES 20000 

#define SERVO_CENTER 1500 // 1.5 ms 
#define SERVO_LEFT   1000 // 1.0 ms 
#define SERVO_RIGHT  2000 // 2.0 ms 
#define STEP_DELAY 20
#define TRANSITION_STEPS 100
#define DEFAULT_PERCENT 25

/** Funktion Prototypes*/
void set_brightness(uint8_t percentage); // Removed void from parameter list for C++
void setColor(uint8_t red, uint8_t green, uint8_t blue); // Removed void from parameter list for C++
void setServoPosition(uint16_t pulseWidth); // Removed void from parameter list for C++
void configurePWM(); // Removed void from parameter list for C++

#endif