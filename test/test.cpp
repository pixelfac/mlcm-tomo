#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../CTScanner.h"

TEST_CASE("Horizontal Line Through Pixels", "[ComputeLine]") {
    CTScanner test(4, 1, 1, 1, 2, 2);

    auto sourcePos = pair<double, double>(1, 0.1);
    auto detectorPos = pair<double, double>(-1, 0.1);

    map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);
    cout << "row[0]" << row[0] << endl;

    REQUIRE(row.size() == 2);
    REQUIRE(row[0] == 0.2);
    REQUIRE(row[1] == 0.2);
}

TEST_CASE("Vertical Line Through Pixels", "[ComputeLine]") {
    CTScanner test(4, 1, 1, 1, 2, 2);

    auto sourcePos = pair<double, double>(0.1, 1);
    auto detectorPos = pair<double, double>(0.1, -1);

    map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

    REQUIRE(row.size() == 2);
    REQUIRE(row[1] == 0.2);
    REQUIRE(row[3] == 0.2);
}