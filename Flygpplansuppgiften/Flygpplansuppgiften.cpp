#include <iostream>
#include <fstream>
#define DYNAMIC 1
#if DYNAMIC
#include "FlightHandlerD.h"
#else
#include "FlightHandler.h"
#endif
#include "Constants.h"
struct FlightLogData{
    int longestWaitTimeAir = 0, longestWaitTimeGround = 0;
    double averageWaitTimeAir = 0, averageWaitTimeGround = 0, averageTimeWasted = 0, failRate = 0;
};
using namespace std;
int main()
{
    const string dataFolder = "test"; // The folder that all the data is put into. 

    FlightLogData FLD[SPAWNRATE]; 

    //Every spawnrate is tested from 0 to SPAWNRATE in promille.
    //It's tested TRIES times and every try is put through CYCLES cycles
    for (int j = 0; j < SPAWNRATE; j++) {
        double averageWaitTimeA = 0, averageWaitTimeG = 0, unusedTime = 0;
        for (int k = 0; k < TRIES; k++) {
#if DYNAMIC
            FlightHandlerD a(FLD[j].longestWaitTimeAir, FLD[j].longestWaitTimeGround, averageWaitTimeA, averageWaitTimeG, unusedTime);
#else
            FlightHandler a(FLD[j].longestWaitTimeAir, FLD[j].longestWaitTimeGround, averageWaitTimeA, averageWaitTimeG, unusedTime);
#endif
            for (int i = 0; i < CYCLES; i++) 
                if (!a.tick(j)) { // Tick returns false if a plane crashed
                    FLD[j].failRate += 1;
                    break;
                }
            // All the statistics are updated after a crash or succesfull run
            FLD[j].averageWaitTimeAir += averageWaitTimeA;
            FLD[j].averageWaitTimeGround += averageWaitTimeG;
            FLD[j].averageTimeWasted += unusedTime;
            averageWaitTimeA = 0;
            averageWaitTimeG = 0;
            unusedTime = 0;
        }
        // The avrage over all the TRIES are calculated and stored
        FLD[j].averageWaitTimeAir /= TRIES;
        FLD[j].averageWaitTimeGround /= TRIES;
        FLD[j].averageTimeWasted /= TRIES;
        FLD[j].failRate /= TRIES / 100;
        // The failrate is printed to show that the program is actually runnning
        std::cout << FLD[j].failRate << std::endl;
    }







    // The CSV file
    ofstream data(((string)"data/").append(dataFolder).append("/data.FL")); // Open the file
    if (!data.is_open()) { // Test if the file was succesfully opened else exit
        std::cout << "Bad file name";
        return -1;
    }
    // The data is out into the file in CSV format
    data << "Flight spawn chance in promille, Fail rates, Average air wait time, Average ground wait time, Longest air time, Longest ground time, Average time wasted" << endl;
    for (int i = 0; i < SPAWNRATE; i++)
        data << i << "," << FLD[i].failRate << "," << FLD[i].averageWaitTimeAir <<
        "," << FLD[i].averageWaitTimeGround  << "," << FLD[i].longestWaitTimeAir  <<
        "," << FLD[i].longestWaitTimeGround << "," << FLD[i].averageTimeWasted << endl;

    data.close();

    // The binary file
    ofstream binData(((string)"data/").append(dataFolder).append("/binData.FL"), ios::binary); // Open the file in binary mode
    if (!binData.is_open()) {// Test if the file was succesfully opened else exit
        std::cout << "Bad file name";
        return -1;
    }

    binData.write((char*)&SPAWNRATE, 4); // Firstly the number of data points is written
    binData.write((char*)FLD, 40 * SPAWNRATE);  // Then the whole dataset
    binData.close();
    return 0;
}

