#include <iostream>
#include <fstream>
#include "FlightHandler.h"
#include "Constants.h"

using namespace std;
int main(int argc, char** argv)
{
    /*if (argc != 6)
        return 0;

    cout << "Lives: " << argv[1];
    cout << "UseTime:  " << argv[2];
    cout << "Landingstrips: " << argv[3];
    cout << "Min spawnrate : " << argv[4];
    cout << "Algo: " << argv[5];
    int lives = atoi(argv[1]);

    FlightHandler a(atoi(argv[3]), atoi(argv[2]), atoi(argv[5]));
    for (int j = atoi(argv[4]); j < 1000; j++) { // Start with 10% spawnrate
        for (int k = 0; k < TRIES; k++) {
            for (int i = 0; i < CYCLES; i++)
                if (!a.tick(j))   // Spawnrate can change with upgrades// Tick returns false if a plane crashed
                    if (lives-- <= 0) {
                        // Calculate reward
                        return 1;
                    }
        }
    }*/
    int lives = 2;
    FlightHandler a(2, 3, 0);
    for (int j = 700; j < 1000; j++) { // Start with 10% spawnrate
        for (int k = 0; k < 100; k++) {
                if (!a.tick(j))   // Spawnrate can change with upgrades// Tick returns false if a plane crashed
                    if (lives-- <= 0) {
                        return 1;
                    }
        }
    }
}
