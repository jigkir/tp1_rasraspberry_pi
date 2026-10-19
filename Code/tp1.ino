// tp1.ino : pins de la carte + point d'entrée (setup / loop)
// Le jeu est dans 01_config.ino ... 09_game.ino.

// Noms D0..D8 de la NodeMCU -> numéros GPIO
enum Pin : uint8_t {
  D0 = 16,  // starts pulled high, no interrupt or pwm
  D1 =  5,
  D2 =  4,
  D3 =  0,  // always pulled high, fails to boot if low
  D4 =  2,  // always pulled high, fails to boot if low
  D5 = 14,
  D6 = 12,
  D7 = 13,
  D8 = 15,  // always pulled low, fails to boot if high
  RX =  3,
  TX =  1
};

// tp1.ino est lu en premier par Arduino : on déclare à l'avance les
// fonctions des composants (définies dans 01_ à 09_) utilisées ici.
namespace code {
  namespace shiftreg { void begin(); }
  namespace button   { void begin(); }
  namespace leds     { void begin(); void allOff(); void groupSet(uint8_t g, bool on); }
  namespace buzzer   { void update(); void beep(uint16_t f, uint16_t durationMs); }
  namespace game     { void begin(); void update(); }
}

using namespace code;

// --- Modes de test ----------------------------------------------------------
const bool TEST_ALL_LEDS         = false;  // allume toutes les DELs
const bool TEST_BUZZER           = false;  // 3 bips en boucle
const bool TEST_ONE_LED_TIME     = false;  // allume les DELs une par une

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nTP1 : jeu de tir");

  shiftreg::begin();
  button::begin();
  leds::begin();                  // configure les GPIO et éteint tout

  randomSeed(RANDOM_REG32);       // générateur matériel de l'ESP8266

  // Debug : pourquoi la carte a (re)démarré ?
  Serial.print("Reset reason: ");
  Serial.println(ESP.getResetReason());

  game::begin();
}

void loop() {
  // --- Modes de test ---
  if (TEST_BUZZER) {
    buzzer::beep(1000, 500);  delay(300);
    buzzer::beep(2000, 500);  delay(300);
    buzzer::beep(3000, 500);  delay(1000);
    return;
  }
  if (TEST_ALL_LEDS) {
    for (uint8_t g = 0; g < 4; g++) leds::groupSet(g, true);
    return;
  }
  if (TEST_ONE_LED_TIME) {
    leds::allOff();
    for (uint8_t g = 0; g < 4; g++) {
      Serial.print("Groupe allume = ");
      Serial.println(g);          // 0 barre, 1 cibles, 2 score, 3 rouge
      leds::groupSet(g, true);
      delay(1000);
      leds::groupSet(g, false);
    }
    return;
  }

  // --- Jeu ---
  buzzer::update();               // garde l'onde carrée du buzzer en marche
  game::update();
}