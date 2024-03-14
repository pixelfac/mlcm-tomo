#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../CTScanner.h"

TEST_CASE("Horizontal Line 2x2 grid", "[ComputeLine]") {
    CTScanner test(4, 1, 1, 1, 2, 2);

    SECTION("Horizontal Line through pixels") {
        auto sourcePos = pair<double, double>(1, 0.1);
        auto detectorPos = pair<double, double>(-1, 0.1);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(0) == Approx(0.2));
        REQUIRE(row.at(1) == Approx(0.2));
    }

    SECTION("Horizontal Line between pixels") {
        auto sourcePos = pair<double, double>(1, 0);
        auto detectorPos = pair<double, double>(-1, 0);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(0) == Approx(0));
        REQUIRE(row.at(1) == Approx(0));
    }

}

TEST_CASE("Vertical Line 2x2 grid", "[ComputeLine]") {
    CTScanner test(4, 1, 1, 1, 2, 2);

    SECTION("Vertical Line through pixels") {
        auto sourcePos = pair<double, double>(0.1, 1);
        auto detectorPos = pair<double, double>(0.1, -1);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(1) == Approx(0.2));
        REQUIRE(row.at(3) == Approx(0.2));
    }

    SECTION("Vertical Line between pixels") {
        auto sourcePos = pair<double, double>(0, 1);
        auto detectorPos = pair<double, double>(0, -1);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(1) == Approx(0));
        REQUIRE(row.at(3) == Approx(0));
    }
}