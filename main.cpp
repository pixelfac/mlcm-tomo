#include <stdio.h>
#include <utility>
#include <cmath>
#include <iostream>

#include "CTScanner.h"

using namespace std;

int main() {
    CTScanner test(4, 1, 1, 1, 2, 2);

    // auto sourcePos = pair<double, double>(0.5, 0);
    // auto detectorPos = pair<double, double>(-0.5, 0);

    auto sourcePos = pair<double, double>(-0.1, 1);
    auto detectorPos = pair<double, double>(-0.1, -1);

    map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

    for(auto iter = row.begin(); iter != row.end(); iter++) {
        cout << "Index: " << iter->first << "\tArea: " << iter->second << endl;
    }

    return 0;
};

/*



*/