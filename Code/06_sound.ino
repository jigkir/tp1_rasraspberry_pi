// ============================================================================
// sound : effets sonores + lumineux (fanfares, vol, succès, échec)
// ============================================================================

namespace code {
  namespace sound {

    // Début de partie : tous les indicateurs en alternance + petite fanfare
    // (montée en arpège rapide, répétée 2 fois)
    void startSequence() {
      const uint16_t notes[4] = {523, 659, 784, 1047};
      for (uint8_t rep = 0; rep < 2; rep++) {
        for (uint8_t g = 0; g < 4; g++) {
          leds::groupSet(g, true);
          buzzer::beep(notes[g], 160);
          leds::groupSet(g, false);
          delay(40);
        }
      }
    }

    // Son de vol : glissando descendant rapide
    void flight() {
      for (int f = 2200; f >= 600; f -= 100) buzzer::beep(f, 22);
    }

    // Succès : la DEL verte clignote avec des bips aigus montants, puis reste allumée
    void success(const leds::Out& led) {
      for (uint8_t i = 0; i < 3; i++) {
        leds::set(led, true);
        buzzer::beep(1200 + i * 500, 110);
        leds::set(led, false);
        delay(90);
      }
      leds::set(led, true);
    }

    // Échec : la DEL rouge clignote avec des bips graves descendants, puis reste allumée
    void fail() {
      for (uint8_t i = 0; i < 3; i++) {
        leds::setRed(true);
        buzzer::beep(260 - i * 50, 220);
        leds::setRed(false);
        delay(90);
      }
      leds::setRed(true);
    }

    // Victoire : lumières vertes en alternance + fanfare différente du début :
    // rythme pointé (notes répétées) puis longue note finale tenue.
    void victory() {
      leds::allOff();
      const uint16_t notes[6] = {784, 784, 988, 988, 1319, 1568};
      const uint16_t dur[6]   = {120, 120, 120, 120, 200, 700};
      for (uint8_t i = 0; i < 6; i++) {
        bool even = (i % 2 == 0);
        leds::set(leds::SCORES[0], even);
        leds::set(leds::SCORES[1], !even);
        leds::set(leds::SCORES[2], even);
        buzzer::beep(notes[i], dur[i]);
        delay(30);
      }
      leds::allOff();
    }

    // Défaite : tout éteint, la lumière rouge clignote + fanfare descendante
    void defeat() {
      leds::allOff();
      const uint16_t notes[4] = {400, 330, 260, 180};
      for (uint8_t i = 0; i < 4; i++) {
        leds::setRed(true);
        buzzer::beep(notes[i], 300);
        leds::setRed(false);
        delay(150);
      }
    }
  }
}