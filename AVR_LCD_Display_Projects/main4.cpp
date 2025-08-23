/**
 * @file binary_calculator_lcd.c
 * @brief Implementierung eines binren Taschenrechners mit LCD-Anzeige.
 *
 * @details
 * Der Taschenrechner untersttzt folgende Operationen ber Tasten:
 * - **Increment:** Erhht den Wert um 1.
 * - **Decrement:** Verringert den Wert um 1.
 * - **Linksshift:** Verschiebt den Wert um 1 Bit nach links.
 * - **Rechtsshift:** Verschiebt den Wert um 1 Bit nach rechts.
 *
 * Die Ergebnisse werden im Dezimalsystem auf einem LCD angezeigt.
 * Der Zhlerwert kann sowohl positiv als auch negativ sein.
 *
 * @author Danielou Mounsande
 * @date 28. November 2024
 */

#include "main.h"

/**
 * @brief Fhrt einen bineren Taschenrechner mit LCD-Anzeige aus.
 *
 * @details
 * Der Benutzer kann den Zhlerwert durch Drcken von Tasten manipulieren:
 * - **Taste 1 (PC4):** Zhler um 1 erhhen.
 * - **Taste 2 (PC5):** Zhler um 1 verringern.
 * - **Taste 3 (PC6):** Linksshift (Multiplikation mit 2).
 * - **Taste 4 (PC7):** Rechtsshift (Division durch 2).
 *
 * Der aktuelle Wert wird in Dezimalform auf dem LCD angezeigt.
 *
 * @note Ein Entprellungsmechanismus stellt sicher, dass jede Eingabe eindeutig registriert wird.
 */
void binary_calculator_lcd() {
    static int32_t counter = 0;    /**< Zhlerwert (positiv oder negativ). */
    static uint8_t last_button = 0; /**< Speichert den letzten Tasterstatus. */

    char display[LCD_WIDTH + 1]; /**< Puffer fr die Anzeige auf dem LCD. */
    display[LCD_WIDTH] = '\0';   /**< Nullterminierung des Puffers. */

    // Konfiguration der Tasten als Eingnge
    PORTC.DIRCLR = PIN4_BIS_7;
    PORTC.PIN4CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN5CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN6CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t
    PORTC.PIN7CTRL = static_cast<uint8_t>(~PORT_PULLUPEN_bm); // Cast to uint8_t

    while (true) { // Use true instead of 1 for C++
        // Erhhen des Zhlerwerts
        if ((PORTC.IN & PIN4_bm) == 0 && last_button == 0) {
            _delay_ms(WAITING_FOR_DEBOUNCING);
            lcd_clear();
            if ((PORTC.IN & PIN4_bm) == 0) {
                counter++;
                last_button = PIN4_bm;
            }
        }
        // Verringern des Zhlerwerts
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

        // Zurcksetzen des Tasterstatus, wenn keine Taste gedrckt ist
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

int main() { // Changed from main() to int main()
    lcd_init();          // Initialisiere LCD
    lcd_enable(true);    
    lcd_backlight(true); 
    binary_calculator_lcd(); // Taschenrechner starten

    return 0;
}
