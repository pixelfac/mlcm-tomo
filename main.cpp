#include <stdio.h>
#include <utility>
#include <cmath>
#include <iostream>
#include <chrono>

#include "CTScanner.h"

using namespace std;

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    CTScanner test(4, 1, 1, 1, 2, 128);

    auto sourcePos = pair<double, double>(0.5, -0.65);
        auto detectorPos = pair<double, double>(0, -0.3);

    for (int i = 0; i < 64000; i++)
        auto row = test.computeLineIntersections(sourcePos, detectorPos).first;


    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;



    // for(auto iter = row.begin(); iter != row.end(); iter++) {
    //     cout << "Index: " << iter->first << "\tArea: " << iter->second << endl;
    // }

    return 0;
};

/*



*/