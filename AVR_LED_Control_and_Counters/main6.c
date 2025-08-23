/********************************************************************************
 * @file main6.c
 * @brief Kontrolliert LEDs basierend auf Tasteneingaben.
 * 
 * @author Danielou Mounsande
 * @date 22. November 2024
 * 
 * @details
 * Diese Datei enth�lt zwei Funktionen, die LEDs entsprechend den Eingaben
 * an Tasten (Buttons) steuern. Eine Funktion steuert eine einzelne LED,
 * w�hrend die andere Funktion alle LEDs basierend auf den Tasten steuert.
 *******************************************************************************/

#include "main.h"

/**
 * @brief Steuert eine einzelne LED basierend auf einem Button.
 * 
 * Diese Funktion aktiviert eine LED (verbunden mit PIN0), wenn die zugeh�rige Taste 
 * (verbunden mit PIN4) gedr�ckt wird. Die Taste verwendet einen Pull-Up-Widerstand, 
 * sodass ein gedr�ckter Zustand als LOW gelesen wird.
 * 
 * @details
 * - Konfiguriert PIN4 als Eingang und aktiviert den internen Pull-Up-Widerstand.
 * - Wenn die Taste gedr�ckt ist, wird die LED an PIN0 aktiviert.
 * - Die LED wird ausgeschaltet, wenn die Taste losgelassen wird.
 * 
 * @note Diese Funktion muss in einer Schleife aufgerufen werden, um kontinuierlich
 *       auf die Taste zu reagieren.
 */
void light_by_buton() {
    PORTD.DIRSET = PIN0_bm; /**< Konfiguriert PIN0 als Ausgang. */
    PORTC.DIRCLR = PIN4_bm; /**< Konfiguriert PIN4 als Eingang. */
    PORTC.PIN4CTRL = ~PORT_PULLUPEN_bm; /**< Aktiviert den internen Pull-Up-Widerstand f�r PIN4. */
    
    if ((PORTC.IN & PIN4_bm) == 0) { /**< �berpr�ft, ob die Taste gedr�ckt ist. */
        PORTD.OUT = PIN0_bm; /**< Aktiviert die LED an PIN0. */
    }
    PORTD.OUTCLR = PIN0_bm; /**< Schaltet die LED aus. */
}

/**
 * @brief Steuert alle LEDs basierend auf mehreren Buttons.
 * 
 * Diese Funktion aktiviert alle LEDs, wenn eine der Tasten (verbunden mit PIN4 bis PIN7)
 * gedr�ckt wird. Andernfalls bleiben die LEDs ausgeschaltet.
 * 
 * @details
 * - Konfiguriert die Pins PIN4 bis PIN7 als Eing�nge und aktiviert deren Pull-Up-Widerst�nde.
 * - �berpr�ft, ob eine der Tasten gedr�ckt ist.
 * - Aktiviert oder deaktiviert die LEDs entsprechend.
 * 
 * @note Diese Funktion muss in einer Schleife aufgerufen werden, um kontinuierlich
 *       auf die Tasten zu reagieren.
 */
void light_all_leds_by_button() {
    PORTC.DIRCLR = PIN4_BIS_7; /**< Konfiguriert PIN4 bis PIN7 als Eing�nge und Aktiviert den internen Pull-Up f�r jeweils */
    PORTC.PIN4CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN5CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN6CTRL = ~PORT_PULLUPEN_bm; 
    PORTC.PIN7CTRL = ~PORT_PULLUPEN_bm; 
    
    if ((PORTC.IN & PIN4_bm) == 0 || 
        (PORTC.IN & PIN5_bm) == 0 || 
        (PORTC.IN & PIN6_bm) == 0 || 
        (PORTC.IN & PIN7_bm) == 0) { /**< �berpr�ft, ob eine der Tasten gedr�ckt ist. */
        PORTD.OUTSET = ALL_LEDS; /**< Aktiviert alle LEDs. */
    } else {
        PORTD.OUTCLR = ALL_LEDS; /**< Schaltet alle LEDs aus. */
    }
}

/**
 * @brief Hauptfunktion des Programms.
 * 
 * @details
 * - Konfiguriert alle LEDs als Ausg�nge.
 * - F�hrt eine der beiden Funktionen (`light_by_buton` oder `light_all_leds_by_button`)
 *   in einer Endlosschleife aus.
 * 
 * @note Entfernen Sie die Kommentare um `main`, um die gew�nschte Funktionalit�t zu aktivieren.
 */
/*
main() {
    PORTD.DIRSET = ALL_LEDS;  

    while (1) {
        //light_by_buton(); // Aktiviert die Funktion f�r eine einzelne LED. 
        light_all_leds_by_button(); //< Aktiviert die Funktion f�r alle LEDs. 
    }
}*/
