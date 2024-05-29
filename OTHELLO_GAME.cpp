#include <iostream>
#include <string>
#include "config.h"

using namespace std;

int spielfeld[GROESSE_Y][GROESSE_X];
int aktuellerSpieler;
int possible_path[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

void initialisiereSpielfeld() {
    for (int i = 0; i < GROESSE_Y; i++) {
        for (int j = 0; j < GROESSE_X; j++) {
            spielfeld[i][j] = 0;
        }
    }
    spielfeld[3][3] = 2;
    spielfeld[3][4] = 1;
    spielfeld[4][3] = 1;
    spielfeld[4][4] = 2;
}

void zeigeSpielfeld() {
    cout << "   A   B   C   D   E   F   G   H" << endl;
    cout << " +---+---+---+---+---+---+---+---+" << endl;
    for (int i = 0; i < GROESSE_Y; i++) {
        cout << i + 1 << " | ";
        for (int j = 0; j < GROESSE_X; j++) {
            if (spielfeld[i][j] == 1) {
                cout << "X | ";
            } else if (spielfeld[i][j] == 2) {
                cout << "O | ";
            } else {
                cout << "  | ";
            }
        }
        cout << endl;
        cout << " +---+---+---+---+---+---+---+---+" << endl;
    }
}

void cng_player() {
    aktuellerSpieler = (aktuellerSpieler == 1) ? 2 : 1; // Switch between player 1 and player 2
}

bool aufSpielfeld(int x, int y) {
    return (x >= 0 && x < GROESSE_X && y >= 0 && y < GROESSE_Y && spielfeld[y][x] == 0);
}

bool zugGueltig(int spielfeld[GROESSE_Y][GROESSE_X], int aktuellerSpieler, int x, int y) {
    if (!aufSpielfeld(x, y)) {
        return false;
    }

    for (int k = 0; k < 8; k++) {
        int dx = possible_path[k][0];
        int dy = possible_path[k][1];
        int nx = x + dx;
        int ny = y + dy;
        bool opponent_coin_found = false;

        while (nx >= 0 && nx < GROESSE_X && ny >= 0 && ny < GROESSE_Y && spielfeld[ny][nx] != 0 && spielfeld[ny][nx] != aktuellerSpieler) {
            opponent_coin_found = true;
            nx += dx;

            ny += dy;
        }

        if (opponent_coin_found && nx >= 0 && nx < GROESSE_X && ny >= 0 && ny < GROESSE_Y && spielfeld[ny][nx] == aktuellerSpieler) {
            return true;
        }
    }

    return false;
}

void zugAusfuehren(int spielfeld[GROESSE_Y][GROESSE_X], int aktuellerSpieler, int x, int y) {
    if (zugGueltig(spielfeld, aktuellerSpieler, x, y)) {
        spielfeld[y][x] = aktuellerSpieler;
        for (int k = 0; k < 8; k++) {
            int dx = possible_path[k][0];
            int dy = possible_path[k][1];
            int nx = x + dx;
            int ny = y + dy;
            bool opponent_coin_found = false;

            while (nx >= 0 && nx < GROESSE_X && ny >= 0 && ny < GROESSE_Y && spielfeld[ny][nx] != 0 && spielfeld[ny][nx] != aktuellerSpieler) {
                nx += dx;
                ny += dy;
                opponent_coin_found = true;
            }

            if (opponent_coin_found && nx >= 0 && nx < GROESSE_X && ny >= 0 && ny < GROESSE_Y && spielfeld[ny][nx] == aktuellerSpieler) {
                int rx = x, ry = y;
                while (rx != nx || ry != ny) {
                    spielfeld[ry][rx] = aktuellerSpieler;
                    rx += dx;
                    ry += dy;
                }
            }
        }
        cng_player();
    }
}

int gewinner() {
    int count_1 = 0, count_2 = 0;
    for (int i = 0; i < GROESSE_Y; i++) {
        for (int j = 0; j < GROESSE_X; j++) {
            if (spielfeld[i][j] == 1) {
                count_1++;
            } else if (spielfeld[i][j] == 2) {
                count_2++;
            }
        }
    }
    if (AUSFUEHRLICH) {
        cout << "Spieler 1 hat " << count_1 << " Steine." << endl;
        cout << "Spieler 2 hat " << count_2 << " Steine." << endl;
    }
    if (count_1 > count_2) {
        return 1;
    } else if (count_2 > count_1) {
        return 2;
    } else {
        return 0; // if it results in a draw
    }
}

int moeglicheZuege(int spielfeld[GROESSE_Y][GROESSE_X], int aktuellerSpieler) {
    int count = 0;
    int gegnerSpieler = (aktuellerSpieler == 1) ? 2 : 1; // Get the opponent player type
    for (int y = 0; y < GROESSE_Y; y++) {
        for (int x = 0; x < GROESSE_X; x++) {
            if (zugGueltig(spielfeld, aktuellerSpieler, x, y) || zugGueltig(spielfeld, gegnerSpieler, x, y)) { // Check for both current and opponent player's possible moves
                count++;
            }
        }
    }
    return count;
}

bool menschlicherZug(int spielfeld[GROESSE_Y][GROESSE_X], int aktuellerSpieler) {
    if (moeglicheZuege(spielfeld, aktuellerSpieler) == 0) {
        if (AUSFUEHRLICH) {
            cout << "Keine moeglichen Zuege fuer Spieler " << aktuellerSpieler << endl;
        }
        return false;
    }

    int posX;
    int posY;
    string symbolSpieler = (aktuellerSpieler == 1) ? "X" : "O";

    while (true) {
        string eingabe;
        cout << endl << "Du bist " << symbolSpieler << ". Dein Zug (z.B. A1, a1): ";
        cin >> eingabe;
        posX = toupper(eingabe[0]) - 'A';
        posY = eingabe[1] - '1';

        if (zugGueltig(spielfeld, aktuellerSpieler, posX, posY)) {
            break;
        } else {
            cout << endl << "Ungueltige Eingabe !" << endl;
        }
    }

    zugAusfuehren(spielfeld, aktuellerSpieler, posX, posY);
    cout << endl << "\n\nSpieler " << aktuellerSpieler << " setzt auf " << (char)(posX + 'A') << (posY + 1) << "\n\n\n";

    return true;
}

bool computerZug(int spielfeld[GROESSE_Y][GROESSE_X], const int aktuellerSpieler) {
    if (moeglicheZuege(spielfeld, aktuellerSpieler) == 0) {
        return false;
    }
    int voraussehen[GROESSE_Y][GROESSE_X];
    int min_x = 0;
    int min_y = 0;
    int min = GROESSE_X * GROESSE_Y;

    for (int j = 0; j < GROESSE_Y; j++) {
        for (int i = 0; i < GROESSE_X; i++) {
            // Spielfeld zuruecksetzen
            for (int cj = 0; cj < GROESSE_Y; cj++) {
                for (int ci = 0; ci < GROESSE_X; ci++) {
                    voraussehen[cj][ci] = spielfeld[cj][ci];
                }
            }

            if (zugGueltig(voraussehen, aktuellerSpieler, i, j)) {
                zugAusfuehren(voraussehen, aktuellerSpieler, i, j);
                if (moeglicheZuege(voraussehen, 3 - aktuellerSpieler) < min) {
                    min = moeglicheZuege(voraussehen, 3 - aktuellerSpieler);
                    min_x = i;
                    min_y = j;
                }
            }
        }
    }

    zugAusfuehren(spielfeld, aktuellerSpieler, min_x, min_y);
    cout << endl << "\n\nSpieler " << aktuellerSpieler << " setzt auf " << (char)(min_x + 65) << (min_y + 1) << "\n\n\n\n";

    return true;
}

void spielenMitComputer() {
    int spielerTyp[2] = { 2, 1 }; // Initialize the array with player types (1: Human, 2: Computer)
    initialisiereSpielfeld();
    aktuellerSpieler = spielerTyp[0]; // Set the current player to the first player type
    bool game_over = false;

    while (!game_over) {
        zeigeSpielfeld();
        bool current_player_can_move = (moeglicheZuege(spielfeld, aktuellerSpieler) > 0);

        if (!current_player_can_move) {
            cout << "Spieler " << aktuellerSpieler << " hat keine möglichen Züge. Überspringen..." << endl;
            cng_player();
            aktuellerSpieler = spielerTyp[aktuellerSpieler - 1];
            if (moeglicheZuege(spielfeld, aktuellerSpieler) == 0) {
                game_over = true; // Neither player can move
            }
            continue;
        }

        if (aktuellerSpieler == 1) {
            if (!menschlicherZug(spielfeld, aktuellerSpieler)) {
                cng_player();
                aktuellerSpieler = spielerTyp[1];
            }
        } else {
            computerZug(spielfeld, aktuellerSpieler);
            cng_player();
            aktuellerSpieler = spielerTyp[1];
        }
    }

    int winner = gewinner();
    if (winner == 0) {
        cout << "Das Spiel endet unentschieden!" << endl;
    } else {
        cout << "Spieler " << winner << " gewinnt!" << endl;
    }
}

void spielenMitFreund() {
    int spielerTyp[2] = { 1, 1 }; // Initialize both players as humans
    initialisiereSpielfeld();
    aktuellerSpieler = 1; // Set the current player to the first player type
    bool game_over = false;

    while (!game_over) {
        zeigeSpielfeld();
        bool current_player_can_move = (moeglicheZuege(spielfeld, aktuellerSpieler) > 0);

        if (!current_player_can_move) {
            cout << "Spieler " << aktuellerSpieler << " hat keine möglichen Züge. Überspringen..." << endl;
            cng_player();
            aktuellerSpieler = spielerTyp[aktuellerSpieler - 1];
            if (moeglicheZuege(spielfeld, aktuellerSpieler) == 0) {
                game_over = true; // Neither player can move
            }
            continue;
        }

        if (!menschlicherZug(spielfeld, aktuellerSpieler)) {
            cng_player();
            aktuellerSpieler = spielerTyp[aktuellerSpieler - 1];
        }
    }

    int winner = gewinner();
    if (winner == 0) {
        cout << "Das Spiel endet unentschieden!" << endl;
    } else {
        cout << "Spieler " << winner << " gewinnt!" << endl;
    }
}
int main() {
    int option;
    do{
        cout << "Waehlen Sie die Spieloption:" << endl;
        cout << "1. Spielen gegen den Computer" << endl;
        cout << "2. Spielen mit einem Freund" << endl;
        cout << "3. Beenden" << endl; // missing option 3 in the original code
        cout << "\n\nIhre Wahl: ";
        cin >> option;

        if (option == 1) {
            spielenMitComputer();
        } else if (option == 2) {
            spielenMitFreund();
        } else if (option == 3) {
            cout << "\n\n\t\t\t     ----- Danke schön! -----\n\t\t\t------ Danke für das Spielen ------\n\t\t\t    ---- Auf Wiedersehen! ----\n\n\n" << endl;
            break;
        } else {
            cout << "Ungueltige Auswahl!" << endl;
        }
    }while(true); // change the condition to true to create an infinite loop

    return 0;
}