#pragma once

#include <stdio.h>
#include <utility>
#include <cmath>

#include "CTScanner.h"

using namespace std;

int main() {
    CTScanner test(4, 1, 1, 1, 2, 2);

    auto sourcePos = test.GetCurrentSourcePosition(0);
    auto detectorPos = test.GetCurrentDetectorPosition(0, 1);

    auto results = test.computeLineIntersections(sourcePos, detectorPos.first);

    return 0;
};

/*



*/