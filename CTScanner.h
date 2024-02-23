#include <stdio.h>
#include <utility>

#include "Subject.h"
#include "VectorUtils.h"

using namespace std;

class CTScanner {
public:
    int views; //number of views, spaced equally around the subject (e.g. 36 views is one every 10 degrees)
    double sourceDist; //distance from source ray emitter to center of subject. MUST be greater than subject diagonal
    double detectorDist; //distance from center detector to center of subject. MUST be greater than subject diagonal
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

    //returns a pair of coordinates for the position of the detector, factoring in the rotation from the current view and position along detector panel
    // INPUT: detectorNum is 0-indexed, so #0 means left side of detector
    // RETURN: first pair is leftmost coord, second pair is right side of detector
    pair<pair<double, double>, pair<double, double>> GetCurrentDetectorPosition(int viewNum, int detectorNum) {
        pair<double, double> detectorPanelCenter(0.0, detectorDist);
        pair<double, double> detectorLeftSide(0.0, detectorDist);  //leftmost range of this detector pixel
        pair<double, double> detectorRightSide(0.0, detectorDist); //rightmost range of this detector pixel

        double detectorWidth = detectorPanelWidth / numDetectors; //width of each individual detector pixel
        detectorLeftSide.first = (detectorWidth * detectorNum) - (detectorPanelWidth / 2);
        detectorRightSide.first = detectorLeftSide.first + detectorWidth;
        
        // if viewNum == 0, then source has not moved from initial position, (0, detectorDist)
        double degreesRotated = viewNum * (360.0 / views);

        //TODO how to make rotation matrix to rotate a vector by a set number of degrees/radians?
        detectorLeftSide = rotate(detectorLeftSide, degreesRotated);
        detectorRightSide = rotate(detectorRightSide, degreesRotated);

        return pair<pair<double, double>, pair<double, double>>(detectorLeftSide, detectorRightSide);
    }

    //simulate a fan of photons from source, through subject, and hitting a specific detector
    // viewNum is which 
    // detectorNum is which detector on the panel this fan beam is projected to
    void computeFanIntersections(int viewNum, int detectorNum) {
        //compute fanbeam triangle using source position, detector panel position, and which detector on the panel is receiving
        pair<double, double> sourcePosition = GetCurrentSourcePosition(viewNum);

        auto detectorPosition = GetCurrentDetectorPosition(viewNum, detectorNum);

        //checking for overlap and pixel area
        // see "Fast and accurate computation of system matrix for area integral model-based algebraic reconstruction technique" in Slack
    }
};