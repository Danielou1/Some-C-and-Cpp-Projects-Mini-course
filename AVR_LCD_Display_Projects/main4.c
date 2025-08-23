/**
 * @file binary_calculator_lcd.c
 * @brief Implementierung eines bin�ren Taschenrechners mit LCD-Anzeige.
 *
 * @details
 * Der Taschenrechner unterst�tzt folgende Operationen �ber Tasten:
 * - **Increment:** Erh�ht den Wert um 1.
 * - **Decrement:** Verringert den Wert um 1.
 * - **Linksshift:** Verschiebt den Wert um 1 Bit nach links.
 * - **Rechtsshift:** Verschiebt den Wert um 1 Bit nach rechts.
 *
 * Die Ergebnisse werden im Dezimalsystem auf einem LCD angezeigt.
 * Der Z�hlerwert kann sowohl positiv als auch negativ sein.
 *
 * @author Danielou Mounsande
 * @date 28. November 2024
 */

#include "main.h"

/**
 * @brief F�hrt einen bin�ren Taschenrechner mit LCD-Anzeige aus.
 *
 * @details
 * Der Benutzer kann den Z�hlerwert durch Dr�cken von Tasten manipulieren:
 * - **Taste 1 (PC4):** Z�hler um 1 erh�hen.
 * - **Taste 2 (PC5):** Z�hler um 1 verringern.
 * - **Taste 3 (PC6):** Linksshift (Multiplikation mit 2).
 * - **Taste 4 (PC7):** Rechtsshift (Division durch 2).
 *
 * Der aktuelle Wert wird in Dezimalform auf dem LCD angezeigt.
 *
 * @note Ein Entprellungsmechanismus stellt sicher, dass jede Eingabe eindeutig registriert wird.
 */
void binary_calculator_lcd() {
    static int32_t counter = 0;    /**< Z�hlerwert (positiv oder negativ). */
    static uint8_t last_button = 0; /**< Speichert den letzten Tasterstatus. */

    char display[LCD_WIDTH + 1]; /**< Puffer f�r die Anzeige auf dem LCD. */
    display[LCD_WIDTH] = '\0';   /**< Nullterminierung des Puffers. */

    // Konfiguration der Tasten als Eing�nge
    PORTC.DIRCLR = PIN4_BIS_7;
    PORTC.PIN4CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = ~PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL = ~PORT_PULLUPEN_bm;

    while (1) {
        // Erh�hen des Z�hlerwerts
        if ((PORTC.IN & PIN4_bm) == 0 && last_button == 0) {
            _delay_ms(WAITING_FOR_DEBOUNCING);
            lcd_clear();
            if ((PORTC.IN & PIN4_bm) == 0) {
                counter++;
                last_button = PIN4_bm;
            }
        }
        // Verringern des Z�hlerwerts
        else if ((PORTC.IN & PIN5_bm) == 0 && last_button == 0) {
            _delay_ms(WAITING_FOR_DEBOUNCING);
            lcd_clear();
            if ((PORTC.IN & PIN5_bm) == 0) {
                counter--;
                last_button = PIN5_bm;
            }
        }
        // Linksshift
        else if ((PORTC.IN & PIN6_bm) == 0 && last_button == 0) {
            _delay_ms(WAITING_FOR_DEBOUNCING);
            lcd_clear();
            if ((PORTC.IN & PIN6_bm) == 0) {
                counter <<= 1;
                last_button = PIN6_bm;
            }
        }
        // Rechtsshift
        else if ((PORTC.IN & PIN7_bm) == 0 && last_button == 0) {
            _delay_ms(WAITING_FOR_DEBOUNCING);
            lcd_clear();
            if ((PORTC.IN & PIN7_bm) == 0) {
                counter >>= 1;
                last_button = PIN7_bm;
            }
        }

        // Zur�cksetzen des Tasterstatus, wenn keine Taste gedr�ckt ist
        if ((PORTC.IN & (PIN4_BIS_7)) == (PIN4_BIS_7)) {
            last_button = 0;
        }

        // Anzeige des aktuellen Werts auf dem LCD
        integer_to_string(display, counter, 10);
        lcd_moveCursor(0, 0);
        lcd_putString("Dec: ");
        lcd_putString(display);
    }
}

/**
 * @brief Hauptfunktion zur Initialisierung und Start des Taschenrechners.
 *
 * @return 0 bei erfolgreichem Abschluss.
 */

int main() {
    lcd_init();          // Initialisiere LCD
    lcd_enable(true);    
    lcd_backlight(true); 
    binary_calculator_lcd(); // Taschenrechner starten

    return 0;
}

