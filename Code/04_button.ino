// ============================================================================
// button : bouton poussoir avec anti-rebond
// ============================================================================

namespace code {
  namespace button {

    bool     lastReading       = !cfg::PRESSED_LEVEL;
    bool     stableState       = !cfg::PRESSED_LEVEL;
    uint32_t lastDebounce      = 0;
    bool     lastStablePrinted = false;

    void begin() {
      pinMode(cfg::BTN_PIN, INPUT);   // pull-up externe (3.3V -> 1k -> D6)
    }

    // true si le bouton est enfoncé (après anti-rebond)
    bool pressed() {
      bool reading = digitalRead(cfg::BTN_PIN);

      if (reading != lastReading) {
        lastDebounce = millis();
        lastReading = reading;
      }
      if (millis() - lastDebounce > cfg::DEBOUNCE_MS) stableState = reading;

      bool isPressed = (stableState == cfg::PRESSED_LEVEL);
      if (isPressed != lastStablePrinted) {
        Serial.print("BTN  ");
        Serial.println(isPressed ? "ENFONCE" : "RELACHE");
        lastStablePrinted = isPressed;
      }
      return isPressed;
    }

    // Après une section bloquante : on resynchronise l'état du bouton
    void sync() {
      bool reading = digitalRead(cfg::BTN_PIN);
      lastReading = reading;
      stableState = reading;
      lastDebounce = millis();
    }
  }
}