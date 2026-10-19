// ============================================================================
// config : pins, bits du 74HC595 et paramètres du jeu (selon tp1.pdf)
// Note : l'enum Pin est défini dans tp1.ino (inclus avant ce fichier).
// L'interrupteur de son est MATÉRIEL (SPST entre 5V et buzzer) : aucune pin.
// ============================================================================

namespace code {
  namespace cfg {

    // --- 74HC595 (OE -> GND, SRCLR -> 5V) -------------------------------------
    const Pin DS_PIN   = Pin::D2;   // SER   (pin 14)
    const Pin SHCP_PIN = Pin::D3;   // SRCLK (pin 11)
    const Pin STCP_PIN = Pin::D4;   // RCLK  (pin 12)

    // --- Bouton : 3.3V -> 1k -> D6 -> 220 -> bouton -> GND --------------------
    const Pin      BTN_PIN       = Pin::D6;
    const bool     PRESSED_LEVEL = LOW;   // pull-up externe : enfoncé = LOW
    const uint16_t DEBOUNCE_MS   = 30;

    // --- Sorties du 74HC595 (bits 0..7 = Q0..Q7) ------------------------------
    const uint8_t RED_BIT       = 0;   // Q0 : DEL rouge (prise)
    const uint8_t BUZZER_BIT    = 1;   // Q1 : buzzer (via 1k + NPN S8050)
    const uint8_t BAR_FIRST     = 2;   // Q2..Q6 : barre DEL (5 DELs)
    const uint8_t SCORE_REG_BIT = 7;   // Q7 : DEL verte (score 1)

    // --- DELs sur GPIO directs ------------------------------------------------
    const Pin TARGET_PINS[3] = { Pin::D0, Pin::D1, Pin::D7 };  // BLEUES : proche, moyenne, loin
    const Pin SCORE_PIN_2    = Pin::D8;                        // VERTE (score 2)
    const Pin SCORE_PIN_3    = Pin::RX;                        // VERTE (score 3)
    const Pin UNUSED_PIN     = Pin::D5;                        // inutilisée : OUTPUT LOW (jamais flottante)

    // --- Paramètres du jeu ----------------------------------------------------
    const uint8_t  NB_LEDS        = 5;     // taille RÉELLE de la barre de puissance
    const uint8_t  MAX_POWER      = 6;     // 6e DEL "virtuelle" : l'atteindre = tir hors circuit
    const uint8_t  NB_TARGETS     = 3;
    const uint8_t  POINTS_TO_WIN  = 3;
    const uint8_t  MAX_PRISES     = 2;
    const uint16_t CHARGE_STEP_MS = 500;   // temps entre deux DELs de puissance
    const uint16_t TARGET_SHOW_MS = 1000;  // durée d'affichage de la nouvelle cible
    const uint16_t RESULT_SHOW_MS = 1500;  // on laisse voir le résultat du tir
    const uint16_t ROUND_GAP_MS   = 2000;  // pause tableau "vide" (cibles + barre éteintes)

    // Zone d'atterrissage selon la puissance (nombre de DELs allumées).
    // Les 5 DELs servent. Atteindre MAX_POWER (6) = tir hors circuit.
    const uint8_t TARGET_MIN[3] = {1, 3, 4};   // proche, moyenne, loin
    const uint8_t TARGET_MAX[3] = {2, 3, 5};

    // Nom de chaque type de cible (pour l'affichage série)
    const char* const TARGET_NAMES[3] = {"PROCHE", "MOYENNE", "LOIN"};
  }
}