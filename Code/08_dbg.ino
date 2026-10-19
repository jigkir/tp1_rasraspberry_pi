// ============================================================================
// dbg : affichage série (debug)
// ============================================================================

namespace code {
  namespace dbg {

    const char* stateName() {
      switch (game::state) {
        case game::S_IDLE:       return "INACTIF";
        case game::S_START:      return "DEBUT_PARTIE";
        case game::S_ROUND:      return "NOUVELLE_RONDE";
        case game::S_WAIT_PRESS: return "ATTENTE_APPUI";
        case game::S_CHARGING:   return "CHARGEMENT";
        case game::S_RESULT:     return "RESULTAT_TIR";
      }
      return "?";
    }

    // Type de la cible : PROCHE / MOYENNE / LOIN
    const char* targetName(uint8_t t) {
      return cfg::TARGET_NAMES[t];
    }

    // Affiche le type de cible ET la puissance requise, avec une barre visuelle
    // ex: cible = MOYENNE | target_bar = 3  [..#..]
    void printTargetBar() {
      uint8_t t = game::target;
      Serial.print("cible = ");
      Serial.print(targetName(t));
      Serial.print(" | target_bar = ");
      Serial.print(cfg::TARGET_MIN[t]);
      if (cfg::TARGET_MAX[t] != cfg::TARGET_MIN[t]) {
        Serial.print("-");
        Serial.print(cfg::TARGET_MAX[t]);
      }
      Serial.print("  [");
      for (uint8_t i = 1; i <= cfg::NB_LEDS; i++) {
        Serial.print((i >= cfg::TARGET_MIN[t] && i <= cfg::TARGET_MAX[t]) ? '#' : '.');
      }
      Serial.println("]");
    }

    // Une ligne résumant tout l'état du jeu
    void printStatus(const char* event) {
      Serial.print("[STATUS] ");
      Serial.print(event);
      Serial.print(" | etat=");
      Serial.print(stateName());
      Serial.print(" | points=");
      Serial.print(game::points);
      Serial.print("/");
      Serial.print(cfg::POINTS_TO_WIN);
      Serial.print(" | prises=");
      Serial.print(game::prises);
      Serial.print("/");
      Serial.print(cfg::MAX_PRISES);
      Serial.print(" | power=");
      Serial.print(game::power);
      Serial.print("/");
      Serial.print(cfg::NB_LEDS);
      Serial.print(" | ");
      printTargetBar();
    }

    // Puissance pendant le chargement
    void printPower(uint8_t n) {
      Serial.print("  Puissance = ");
      Serial.print(n);
      Serial.print("  vs  ");
      printTargetBar();
    }
  }
}