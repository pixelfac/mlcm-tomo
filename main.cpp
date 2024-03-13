#pragma once

#include <stdio.h>
#include <utility>
#include <cmath>
#include <iostream>

#include "CTScanner.h"

using namespace std;

int main() {
    CTScanner test(4, 1, 1, 1, 2, 2);

    auto sourcePos = test.GetCurrentSourcePosition(0);
    auto detectorPos = test.GetCurrentDetectorPosition(0, 1);

    map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos.first);

    for(auto iter = row.begin(); iter != row.end(); iter++) {
        cout << "Index: " << iter->first << "\tArea: " << iter->second << endl;
    }

    return 0;
};

/*



*/