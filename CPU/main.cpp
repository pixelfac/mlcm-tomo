#include <stdio.h>
#include <utility>
#include <cmath>
#include <iostream>
#include <chrono>

#include "CTScanner.h"

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();
    CTScanner test(4, 1, 1, 1, 2, 128);

    auto sourcePos = pair<double, double>(0.5, -0.65);
        auto detectorPos = pair<double, double>(0, -0.3);

    int iterations = 180 * 180 * 2;
    for (int i = 0; i < iterations; i++)
        auto row = test.computeLineIntersections(sourcePos, detectorPos).first;


    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    cout << "Execution time: " << duration.count() << " seconds." << endl;
    cout << "Avg Per Func Call: " << duration.count() / iterations << " seconds" << endl;



    // for(auto iter = row.begin(); iter != row.end(); iter++) {
    //     cout << "Index: " << iter->first << "\tArea: " << iter->second << endl;
    // }

    return 0;
};

/*



*/