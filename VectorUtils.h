#include <utility>

using namespace std;

// MATH HELPER FUNCS

//rotates a 2D vector, coord, by degrees (in degrees, not radians)
// RETURN: rotated vector
pair<double, double> rotate(pair<double, double> coord, double degrees) {
    double radians = degrees*3.14159/180;
    double sinTheta = sin(radians);
    double cosTheta = cos(radians);
    return pair<double, double>(coord.first * cosTheta - coord.second * sinTheta, coord.first * sinTheta + coord.second * cosTheta);
}

//computes dot product of input 2D vectors
double dot(pair<double, double> v1, pair<double, double> v2) {
    return v1.first * v2.first + v1.second * v2.second;
}