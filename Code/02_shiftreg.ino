// ============================================================================
// shiftreg : pilote du 74HC595 (DELs + buzzer sur le même registre)
// ============================================================================

namespace code {
  namespace shiftreg {

    uint8_t ledBits   = 0;       // état des DELs (le bit du buzzer est ignoré ici)
    bool    buzzLevel = false;   // niveau courant du buzzer

    void begin() {
      pinMode(cfg::DS_PIN, OUTPUT);
      pinMode(cfg::SHCP_PIN, OUTPUT);
      pinMode(cfg::STCP_PIN, OUTPUT);
    }

    // Envoie les 8 bits au 74HC595
    void flush() {
      uint8_t pattern = ledBits & ~(1 << cfg::BUZZER_BIT);
      if (buzzLevel) pattern |= (1 << cfg::BUZZER_BIT);

      digitalWrite(cfg::STCP_PIN, LOW);
      shiftOut(cfg::DS_PIN, cfg::SHCP_PIN, MSBFIRST, pattern);
      digitalWrite(cfg::STCP_PIN, HIGH);
    }

    // Allume / éteint un seul bit (DEL)
    void setBit(uint8_t bit, bool on) {
      if (on) ledBits |=  (1 << bit);
      else    ledBits &= ~(1 << bit);
      flush();
    }

    // Remplace les bits sélectionnés par 'mask' avec ceux de 'value'
    void setField(uint8_t mask, uint8_t value) {
      ledBits = (uint8_t)((ledBits & ~mask) | (value & mask));
      flush();
    }

    // Éteint toutes les DELs (pas le buzzer)
    void clearLeds() {
      ledBits = 0;
      flush();
    }

    // Niveau du buzzer (onde carrée générée par buzzer)
    void setBuzzerLevel(bool level) {
      buzzLevel = level;
      flush();
    }
  }
}