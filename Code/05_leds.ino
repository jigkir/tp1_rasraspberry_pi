// ============================================================================
// leds : toutes les DELs (cibles bleues, scores verts, prise rouge, barre)
// ============================================================================

namespace code {
  namespace leds {

    // Une sortie est soit un bit du 74HC595 (viaReg = true), soit un GPIO.
    struct Out { bool viaReg; uint8_t n; };

    const Out TARGETS[3] = {          // 3 DELs BLEUES : proche, moyenne, loin
      {false, cfg::TARGET_PINS[0]},
      {false, cfg::TARGET_PINS[1]},
      {false, cfg::TARGET_PINS[2]}
    };
    const Out SCORES[3] = {           // 3 DELs VERTES : un point chacune
      {true,  cfg::SCORE_REG_BIT},    // Q7 du 74HC595
      {false, cfg::SCORE_PIN_2},      // D8
      {false, cfg::SCORE_PIN_3}       // RX
    };

    void set(const Out& o, bool on) {
      if (o.viaReg) shiftreg::setBit(o.n, on);
      else          digitalWrite(o.n, on ? HIGH : LOW);
    }

    void setGroup(const Out* group, uint8_t n, bool on) {
      for (uint8_t i = 0; i < n; i++) set(group[i], on);
    }

    void setRed(bool on) {
      shiftreg::setBit(cfg::RED_BIT, on);
    }

    // Barre de puissance : n DELs allumées (0..NB_LEDS).
    // n > NB_LEDS (6e DEL virtuelle) : la barre reste pleine, on ne déborde
    // jamais sur Q7 (DEL verte du score).
    void bar(uint8_t n) {
      if (n > cfg::NB_LEDS) n = cfg::NB_LEDS;
      uint8_t mask  = (uint8_t)(((1 << cfg::NB_LEDS) - 1) << cfg::BAR_FIRST);
      uint8_t value = (uint8_t)(((1 << n) - 1) << cfg::BAR_FIRST);
      shiftreg::setField(mask, value);
    }

    // Tout éteindre (DELs seulement, pas le buzzer)
    void allOff() {
      shiftreg::clearLeds();
      setGroup(TARGETS, cfg::NB_TARGETS, false);
      setGroup(SCORES, cfg::POINTS_TO_WIN, false);
    }

    // Allume ou éteint un groupe (0 = barre, 1 = cibles, 2 = score, 3 = prise)
    void groupSet(uint8_t g, bool on) {
      switch (g) {
        case 0: bar(on ? cfg::NB_LEDS : 0);                      break;
        case 1: setGroup(TARGETS, cfg::NB_TARGETS, on);          break;
        case 2: setGroup(SCORES, cfg::POINTS_TO_WIN, on);        break;
        case 3: setRed(on);                                      break;
      }
    }

    // Broches inutilisées : sortie à LOW, jamais flottantes
    void parkUnusedPins() {
      pinMode(cfg::UNUSED_PIN, OUTPUT);
      digitalWrite(cfg::UNUSED_PIN, LOW);
    }

    void begin() {
      for (uint8_t i = 0; i < cfg::NB_TARGETS; i++)
        if (!TARGETS[i].viaReg) pinMode(TARGETS[i].n, OUTPUT);
      for (uint8_t i = 0; i < cfg::POINTS_TO_WIN; i++)
        if (!SCORES[i].viaReg) pinMode(SCORES[i].n, OUTPUT);
      parkUnusedPins();               // D5 -> OUTPUT LOW
      allOff();
    }
  }
}