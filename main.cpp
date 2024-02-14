#include <stdio.h>
#include <utility>

using namespace std;


class Subject {
public:
    double subjectWidth;
    double subjectHeight;
    int subjectWidthResolution; //how many pixels the subject width should be divided into
    int subjectHeightResolution;  //how many pixels the subject height should be divided into
};

class CTScanner {
public:
    int views; //number of views, spaced equally around the subject (e.g. 36 views is one every 10 degrees)
    double sourceDist; //distance from source ray emitter to center of subject. MUST be greater than furthest subject distance from origin
    double detectorDist; //distance from center detector to center of subject. MUST be greater than furthest subject distance from origin
    double detectorPanelWidth; //total width of entire detector panel
    int numDetectors; //number of equal-sized detectors on detector panel.

    Subject subject;

    //returns a coordinate pair for the position of the source, factoring in the rotation from the current view
    pair<double, double> GetCurrentSourcePosition(int viewNum) {
        pair<double, double> initialPosition(0.0, sourceDist);
        // if viewNum == 0, then source has not moved from initial position, (0, sourceDist)
        double degreesRotated = viewNum * (360.0 / views);

        //TODO how to make rotation matrix to rotate a vector by a set number of degrees/radians?
        return rotate(initialPosition, degreesRotated);
    }

    //returns a coordinate pair for the position of the detector, factoring in the rotation from the current view and position along detector panel
    pair<double, double> GetCurrentSourcePosition(int viewNum, int detectorNum) {
        pair<double, double> detectorPanelCenter(0.0, detectorDist);
        
        // if viewNum == 0, then source has not moved from initial position, (0, detectorDist)
        double degreesRotated = viewNum * (360.0 / views);

        //TODO how to make rotation matrix to rotate a vector by a set number of degrees/radians?
        return rotate(initialPosition, degreesRotated);
    }

    //simulate a fan of photons from source, through subject, and hitting a specific detector
    // viewNum is which 
    // detectorNum is which detector on the panel this fan beam is projected to
    void computeFanIntersections(int viewNum, int detectorNum) {
        //compute fanbeam triangle using source position, detector panel position, and which detector on the panel is receiving
        pair<double, double> sourcePosition = GetCurrentSourcePosition(viewNum);

    }
};


int main() {
    return 0;
};