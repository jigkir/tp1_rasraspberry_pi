// ============================================================================
// game : logique du jeu (machine à états)
// ============================================================================

namespace code {
  namespace game {

    // Barre de puissance pendant le chargement, avec affichage série
    // (n peut valoir 6 : la 6e DEL est virtuelle, la barre reste pleine)
    void showPower(uint8_t n) {
      leds::bar(n);
      dbg::printPower(n);
    }

    // Quelle cible est atteinte avec cette puissance ? (-1 = aucune)
    int8_t targetForPower(uint8_t p) {
      for (uint8_t t = 0; t < cfg::NB_TARGETS; t++) {
        if (p >= cfg::TARGET_MIN[t] && p <= cfg::TARGET_MAX[t]) return t;
      }
      return -1;
    }

    // Fin de partie : tout éteint, pause, puis retour AU DÉBUT DE PARTIE
    // (la prochaine partie démarre toute seule, sans clic)
    void endGame() {
      leds::allOff();
      delay(3000);
      button::sync();
      armed = false;
      state = S_START;
      dbg::printStatus("RETOUR AU DEBUT DE PARTIE");
    }

    // Évalue la partie après une ronde
    void afterRound() {
      if (points >= cfg::POINTS_TO_WIN) {
        dbg::printStatus("PARTIE GAGNEE");
        sound::victory();
        endGame();
      } else if (prises >= cfg::MAX_PRISES) {
        dbg::printStatus("PARTIE PERDUE");
        sound::defeat();
        endGame();
      } else {
        delay(cfg::RESULT_SHOW_MS);                               // on laisse voir le résultat
        leds::setGroup(leds::TARGETS, cfg::NB_TARGETS, false);    // cibles bleues éteintes
        leds::bar(0);                                             // barre éteinte
        // (on garde les DELs de score vertes et la rouge : le score reste affiché)
        dbg::printStatus("TABLEAU VIDE (pause avant la prochaine cible)");
        delay(cfg::ROUND_GAP_MS);                                 // quelques secondes "vide"
        state = S_ROUND;
        dbg::printStatus("PARTIE CONTINUE");
      }
    }

    // Tir : vol, cible touchée, succès ou échec
    void resolveShot(uint8_t finalPower, bool outOfRange) {
      state = S_RESULT;
      buzzer::set(0);                            // fin du son de chargement
      dbg::printStatus(outOfRange ? "TIR HORS CIRCUIT" : "TIR");

      sound::flight();
      leds::bar(0);                              // la barre s'éteint après le tir

      int8_t landed = outOfRange ? -1 : targetForPower(finalPower);
      Serial.print("  Cible visee   = ");
      Serial.println(dbg::targetName(target));
      Serial.print("  Cible touchee = ");
      if (landed >= 0) Serial.println(dbg::targetName(landed));
      else             Serial.println("AUCUNE");
      if (landed >= 0) leds::set(leds::TARGETS[landed], true);   // cible touchée
      delay(300);

      if (landed == (int8_t)target) {
        sound::success(leds::SCORES[points]);
        points++;
        dbg::printStatus("SUCCES (+1 point)");
      } else {
        sound::fail();
        prises++;
        dbg::printStatus("ECHEC (+1 prise)");
      }

      afterRound();
    }

    void begin() {
      state = S_IDLE;
      armed = false;
      dbg::printStatus("PRET (clique sur le bouton pour demarrer)");
    }

    // À appeler à chaque tour de loop()
    void update() {
      bool pressed = button::pressed();

      switch (state) {

        case S_IDLE:
          // Seulement au premier démarrage : on attend le premier clic.
          // Le bouton doit d'abord être vu relâché (évite un démarrage si
          // le bouton est déjà enfoncé au démarrage).
          if (!pressed) {
            armed = true;
          } else if (armed) {
            armed = false;
            state = S_START;
          }
          break;

        case S_START:
          points = 0;
          prises = 0;
          power  = 0;
          dbg::printStatus("=== DEBUT DE PARTIE ===");
          sound::startSequence();     // indicateurs en alternance + fanfare
          leds::allOff();             // les lumières s'éteignent
          delay(1500);                // pause : le jeu reste inactif
          state = S_ROUND;            // puis la première ronde commence
          break;

        case S_ROUND:
          power  = 0;
          target = random(cfg::NB_TARGETS);
          dbg::printStatus("NOUVELLE RONDE (cible affichee 1 s, entrees ignorees)");
          leds::setGroup(leds::TARGETS, cfg::NB_TARGETS, false);   // sécurité : aucune bleue allumée
          leds::set(leds::TARGETS[target], true);                  // la nouvelle cible seulement
          delay(cfg::TARGET_SHOW_MS); // entrées ignorées pendant ce temps
          leds::set(leds::TARGETS[target], false);
          button::sync();             // on oublie ce qui s'est passé pendant l'affichage
          armed = false;
          state = S_WAIT_PRESS;
          dbg::printStatus("A TOI DE JOUER (maintiens le bouton)");
          break;

        case S_WAIT_PRESS:
          if (!pressed) {
            armed = true;             // bouton vu relâché : on peut accepter un appui
          } else if (armed) {
            power = 1;
            lastStepTime = millis();
            state = S_CHARGING;
            dbg::printStatus("DEBUT DU CHARGEMENT");
            showPower(power);
            buzzer::set(buzzer::chargeFreq(power));
          }
          break;

        case S_CHARGING:
          if (!pressed) {             // relâché -> tir avec la puissance actuelle
            Serial.println(">> RELACHE -> TIR");
            resolveShot(power, false);
            break;
          }
          if (millis() - lastStepTime >= cfg::CHARGE_STEP_MS) {
            lastStepTime = millis();
            power++;
            showPower(power);         // à 6 : la barre reste pleine (5 DELs)
            if (power >= cfg::MAX_POWER) {
              // 6e DEL "virtuelle" atteinte -> tir automatique hors circuit (prise)
              Serial.println(">> PUISSANCE MAX ATTEINTE (6e DEL virtuelle) -> TIR HORS CIRCUIT");
              resolveShot(power, true);
            } else {
              buzzer::set(buzzer::chargeFreq(power));   // le son monte avec la puissance
            }
          }
          break;

        case S_RESULT:
          break;                      // transitoire : géré dans resolveShot()
      }
    }
  }
}