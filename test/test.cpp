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

TEST_CASE("0<m<=1 slope 2x2 grid", "[ComputeLine]") {
    CTScanner test(4, 1, 1, 1, 2, 2);

    SECTION("y=x, perfectly bisects grid") {
        auto sourcePos = pair<double, double>(-1, -1);
        auto detectorPos = pair<double, double>(1, 1);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(1) == Approx(0.5));
        REQUIRE(row.at(2) == Approx(0.5));
    }

    SECTION("y=0.5m, left wall to right wall") {
        auto sourcePos = pair<double, double>(-1, -0.5);
        auto detectorPos = pair<double, double>(1, 0.5);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(1) == Approx(0.25));
        REQUIRE(row.at(2) == Approx(0.75));
    }

    SECTION("y=0.5m+0.25, left wall to right wall") {
        auto sourcePos = pair<double, double>(-0.5, 0);
        auto detectorPos = pair<double, double>(0.5, 0.5);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(0) == Approx(0.25));
        REQUIRE(row.at(1) == Approx(0.75));
    }

    SECTION("y=0.5m+0.3, left wall to top wall") {
        auto sourcePos = pair<double, double>(-0.6, 0);
        auto detectorPos = pair<double, double>(0.5, 0.55);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 2);
        REQUIRE(row.at(0) == Approx(0.35));
        REQUIRE(row.at(1) == Approx(0.84));
    }

    SECTION("y=0.5x-0.5, bottom wall to right wall") {
        auto sourcePos = pair<double, double>(0, -0.5);
        auto detectorPos = pair<double, double>(0.5, -0.25);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 1);
        REQUIRE(row.at(3) == Approx(0.25));
    }

    SECTION("y=0.5x-0.6, bottom wall to right wall") {
        auto sourcePos = pair<double, double>(0, -0.6);
        auto detectorPos = pair<double, double>(0.5, -0.35);

        map<int, double> row = test.computeLineIntersections(sourcePos, detectorPos);

        REQUIRE(row.size() == 1);
        REQUIRE(row.at(3) == Approx(0.09));
    }
}