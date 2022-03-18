#include <iostream>
#define DYNAMIC 1
#if DYNAMIC
#include "FlightHandlerD.h"
#include "FlightHandler.h"
#endif
#include <fstream>

struct FlightLogData{
    int longestWaitTimeAir = 0, longestWaitTimeGround = 0;
    double averageWaitTimeAir = 0, averageWaitTimeGround = 0, averageTimeWasted = 0, failRate = 0;
};
using namespace std;
int main()
{
    const long SPAWNRATE = 200; // Spawn chanve of planes rangees from 0 to this chance in promille
    const int TRIES = 100;  //Number of times each differnet spawnrate is tested
    const int CYCLES = 1000; // Number of cycles per try 
    const string dataFolder = "test"; // The folder that all the data is put into. 
    //This is const while testing

    srand(time(NULL));
    FlightLogData FLD[SPAWNRATE];

    for (int j = 0; j < SPAWNRATE; j++) {
        double averageWaitTimeA = 0, averageWaitTimeG = 0, unusedTime = 0;
        for (int k = 0; k < TRIES; k++) {
#if DYNAMIC
            FlightHandler a(FLD[j].longestWaitTimeAir, FLD[j].longestWaitTimeGround, averageWaitTimeA, averageWaitTimeG, unusedTime);
#else
            FlightHandlerD a(FLD[j].longestWaitTimeAir, FLD[j].longestWaitTimeGround, averageWaitTimeA, averageWaitTimeG, unusedTime);
#endif
            for (int i = 0; i < CYCLES; i++) 
                if (!a.tick(j)) {
                    FLD[j].failRate += 1;
                    break;
                }
            FLD[j].averageWaitTimeAir += averageWaitTimeA;
            FLD[j].averageWaitTimeGround += averageWaitTimeG;
            FLD[j].averageTimeWasted += unusedTime;
            averageWaitTimeA = 0;
            averageWaitTimeG = 0;
            unusedTime = 0;
        }
        FLD[j].averageWaitTimeAir /= TRIES;
        FLD[j].averageWaitTimeGround /= TRIES;
        FLD[j].averageTimeWasted /= TRIES;
        FLD[j].failRate /= TRIES / 100;
        std::cout << FLD[j].failRate << std::endl;
    }

    ofstream data(((string)"data/").append(dataFolder).append("/data.FL"));
    if (!data.is_open()) {
        std::cout << "Bad file name";
        exit;
    }
    data << "Flight spawn chance in promille, Fail rates, Average air wait time, Average ground wait time, Longest air time, Longest ground time, Average time wasted" << endl;
    for (int i = 0; i < SPAWNRATE; i++)
        data << i << "," << FLD[i].failRate << "," << FLD[i].averageWaitTimeAir <<
        "," << FLD[i].averageWaitTimeGround  << "," << FLD[i].longestWaitTimeAir  <<
        "," << FLD[i].longestWaitTimeGround << "," << FLD[i].averageTimeWasted << endl;

    data.close();

    ofstream binData(((string)"data/").append(dataFolder).append("/binData.FL"), ios::binary);
    if (!binData.is_open()) {
        std::cout << "Bad file name";
        exit;
    }

    binData.write((char*)&SPAWNRATE, 4);
    binData.write((char*)FLD, 40 * SPAWNRATE);  
    binData.close();
}

