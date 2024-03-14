#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../CTScanner.h"

TEST_CASE("Horizontal Line", "[ComputeLine]") {
    CTScanner test(4, 1, 1, 1, 2, 2);

    SECTION("Horizontal Line through pixels") {
        auto sourcePos = pair<double, double>(1, 0.1);
        auto detectorPos = pair<double, double>(-1, 0.1);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row[0] == Approx(0.2));
        REQUIRE(row[1] == Approx(0.2));
    }

    SECTION("Horizontal Line between pixels") {
        auto sourcePos = pair<double, double>(1, 0);
        auto detectorPos = pair<double, double>(-1, 0);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row[0] == Approx(0));
        REQUIRE(row[1] == Approx(0));
    }

}

TEST_CASE("Vertical Line Through Pixels", "[ComputeLine]") {
    CTScanner test(4, 1, 1, 1, 2, 2);

    auto sourcePos = pair<double, double>(0.1, 1);
    auto detectorPos = pair<double, double>(0.1, -1);

    map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

    REQUIRE(row.size() == 2);
    REQUIRE(row[1] == Approx(0.2));
    REQUIRE(row[3] == Approx(0.2));
}