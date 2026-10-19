# BoM : TP1, jeu de tir

| Composant | Description | Qté |
|---|---|---|
| Microcontrôleur | ESP8266 (NodeMCU) | 1 |
| Registre à décalage | 74HC595 | 1 |
| Transistor NPN | S8050, pilote le buzzer | 1 |
| Buzzer passif | Signal sonore | 1 |
| DEL bleue | Cibles | 3 |
| DEL verte | Score | 3 |
| DEL rouge | Prises | 1 |
| Barre DEL | 10 segments (5 utilisés), puissance | 1 |
| Bouton poussoir | Charge et tir | 1 |
| Switch | coupe le son | 1 |
| Résistance | 220 Ω | 8 |
| Résistance | 1 kΩ | 7 |

## Détail des résistances

### Résistance de 220 Ω

| Composant / Rôle | Qté par élément | Total |
| :--- | :---: | :---: |
| **DEL bleues** (cibles) | 1 par DEL | 3 |
| **DEL vertes** (score) | 1 par DEL | 3 |
| **DEL rouge** (prises) | 1 par DEL | 1 |
| **Bouton** (en série) | 1 | 1 |
| **Total** | | **8** |

### Résistance de 1 kΩ

| Composant / Rôle | Qté par élément | Total |
| :--- | :---: | :---: |
| **Barre DEL** (puissance) | 1 par segment (5 au total) | 5 |
| **Base du transistor** (R9, S8050) | 1 | 1 |
| **Bouton** (pull-up) | 1 | 1 |
| **Total** | | **7** |