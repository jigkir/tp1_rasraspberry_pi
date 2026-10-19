// ============================================================================
// buzzer : buzzer passif piloté par une onde carrée via le 74HC595
// Appeler buzzer::update() le plus souvent possible (dans loop()).
// ============================================================================

namespace code {
  namespace buzzer {

    uint16_t freq         = 0;       // 0 = silence
    uint32_t lastToggleUs = 0;
    bool     level        = false;

    void set(uint16_t f) {
      freq = f;
      if (f == 0) {
        level = false;
        shiftreg::setBuzzerLevel(false);
      }
    }

    void update() {
      if (freq == 0) return;
      uint32_t halfPeriodUs = 500000UL / freq;
      if (micros() - lastToggleUs >= halfPeriodUs) {
        lastToggleUs = micros();
        level = !level;
        shiftreg::setBuzzerLevel(level);
      }
    }

    // Joue une note en bloquant pendant durationMs
    void beep(uint16_t f, uint16_t durationMs) {
      set(f);
      uint32_t end = millis() + durationMs;
      while ((int32_t)(millis() - end) < 0) {
        update();
        yield();
      }
      set(0);
    }

    // Son de chargement : monte avec la puissance (1..5)
    uint16_t chargeFreq(uint8_t p) {
      return 500 + p * 400;
    }
  }
}