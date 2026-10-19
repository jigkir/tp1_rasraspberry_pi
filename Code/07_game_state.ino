// ============================================================================
// game_state : état partagé du jeu
// ============================================================================

namespace code {
  namespace game {

    // S_IDLE : rien ne se passe tant qu'on n'a pas cliqué sur le bouton
    enum State { S_IDLE, S_START, S_ROUND, S_WAIT_PRESS, S_CHARGING, S_RESULT };

    State    state        = S_IDLE;
    uint8_t  points       = 0;
    uint8_t  prises       = 0;
    uint8_t  target       = 0;
    uint8_t  power        = 0;
    bool     armed        = false;   // true quand le bouton a été vu relâché
    uint32_t lastStepTime = 0;
  }
}