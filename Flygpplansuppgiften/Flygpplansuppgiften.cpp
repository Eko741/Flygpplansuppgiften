#include <iostream>
#include "FlightHandler.h"
int main()
{
    srand(time(NULL));
    FlightHandler a;
    int failRate[300];
    for (int i = 0; i < 300; i++)
        failRate[i] = 0;


    for (int j = 0; j < 300; j++) {
        for (int k = 0; k < 100; k++) {
            for (int i = 0; i < 1000; i++) {
                if (!a.tick(j)) {
                    failRate[j]++;
                    break;
                }
                if (i % 3 == 0)
                    a.useStrip();
               //a >> &(std::cout << "\n\n" << "Itteration: " << i << ", " << k << ", " << j << std::endl);
            }
        }
        //std::cout << "Fail rate at " << j << " promille: " << failRate[j] << std::endl;
        std::cout << failRate[j] << std::endl;
    }
    for (int i = 0; i < 300; i++)
        std::cout << i << std::endl;
}

