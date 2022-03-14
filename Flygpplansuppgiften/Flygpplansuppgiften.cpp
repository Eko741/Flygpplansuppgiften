#include <iostream>
#include "FlightHandler.h"
#include <fstream>

struct FlightLogData{
    int longestWaitTimeAir = 0, longestWaitTimeGround = 0;
    double averageWaitTimeAir = 0, averageWaitTimeGround = 0, averageStripUse = 0, failRate = 0;
};
using namespace std;
int main()
{
    const long SPAWNRATE = 200; // Spawn chanve of planes rangees from 0 to this chance in promille
    const int TRIES = 10000;  //Number of times each differnet spawnrate is tested
    const int CYCLES = 1000; // Number of cycles per try 
    const string dataFolder = "test"; // The folder that all the data is put into. 
    //This is const while testing

    srand(time(NULL));
    FlightLogData FLD[SPAWNRATE];

    for (int j = 0; j < SPAWNRATE; j++) {
        double averageWaitTimeA = 0, averageWaitTimeG = 0, unusedTime = 0;
        for (int k = 0; k < TRIES; k++) {
            FlightHandler a(FLD[j].longestWaitTimeAir, FLD[j].longestWaitTimeGround, averageWaitTimeA, averageWaitTimeG, unusedTime);
            for (int i = 0; i < CYCLES; i++) 
                if (!a.tick(j)) {
                    FLD[j].failRate += 1;
                    break;
                }
            FLD[j].averageWaitTimeAir += averageWaitTimeA;
            FLD[j].averageWaitTimeGround += averageWaitTimeG;
            FLD[j].averageStripUse += unusedTime;
            averageWaitTimeA = 0;
            averageWaitTimeG = 0;
            unusedTime = 0;
        }
        FLD[j].averageWaitTimeAir /= TRIES;
        FLD[j].averageWaitTimeGround /= TRIES;
        FLD[j].averageStripUse /= TRIES;
        FLD[j].failRate /= TRIES / 100;
        std::cout << FLD[j].failRate << std::endl;
    }

    ofstream data(((string)"data/").append(dataFolder).append("/data.FL"));
    if (!data.is_open()) {
        std::cout << "Bad file name";
        exit;
    }
    data << "Flight, Spawn chance in promille, Fail rates, Average air wait time, Average ground wait time, Longest air time, Longest ground time, Average strip use" << endl;
    for (int i = 0; i < SPAWNRATE; i++)
        data << i << "," << FLD[i].failRate << "," << FLD[i].averageWaitTimeAir <<
        "," << FLD[i].averageWaitTimeGround  << "," << FLD[i].longestWaitTimeAir  <<
        "," << FLD[i].longestWaitTimeGround << "," << FLD[i].averageStripUse << endl;

    ofstream binData(((string)"data/").append(dataFolder).append("/binData.FL"), ios::binary);
    if (!binData.is_open()) {
        std::cout << "Bad file name";
        exit;
    }

    binData.write((char*)&SPAWNRATE, 4);
    for (int i = 0; i < SPAWNRATE; i++) {
        binData.write((char*)&FLD[i].failRate, 8);
    }
   
}

