#include <stdio.h>
#include <utility>
#include <cmath>
#include <iostream>
#include <chrono>

#include "CTScanner.h"

using namespace std;

int main() {
    CTScanner test(4, 1, 1, 1, 2, 128);

    auto sourcePos = pair<double, double>(0.5, -0.65);
        auto detectorPos = pair<double, double>(0, -0.3);




    auto row = test.computeLineIntersections(sourcePos, detectorPos).first;

    for(auto iter = row.begin(); iter != row.end(); iter++) {
        cout << "Index: " << iter->first << "\tArea: " << iter->second << endl;
    }

    return 0;
};

/*



*/