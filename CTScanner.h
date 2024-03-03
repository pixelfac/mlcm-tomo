#include <stdio.h>
#include <utility>

#include "VectorUtils.h"

using namespace std;

class CTScanner {
public:
    int views; //number of views, spaced equally around the subject (e.g. 36 views is one every 10 degrees)
    double sourceDist; //distance from source ray emitter to center of subject. Subject diagonal is sqrt(2)/2
    double detectorDist; //distance from center detector to center of subject. Subject diagonal is sqrt(2)/2
    double detectorPanelWidth; //total width of entire detector panel
    int numDetectors; //number of equal-sized detectors on detector panel.

    // subject is a square sized 1x1 units, centered at the origin, so bounds are (-0.5, 0.5) in both axes.
    // for simplicity and comparison, we are only working with square imaging tasks
    // These units are the same as determining the source and detector distance.
    // The diagonal from the subject to it's furthest corner is sqrt(2)/2
    int subjectResolution;  // how many pixels each dimension of the subject should be partitioned into

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
    // calls computeLineIntersections for both lines in the fan beam
    // combines area to get complete list of pixels with their areas
    void computeFanIntersections(int viewNum, int detectorNum) {
        //compute fanbeam triangle using source position, detector panel position, and which detector on the panel is receiving
        pair<double, double> sourcePosition = GetCurrentSourcePosition(viewNum);

        auto detectorPosition = GetCurrentDetectorPosition(viewNum, detectorNum);

        //TODO call computeLineIntersections for both lines

        //checking for overlap and pixel area
        // see "Fast and accurate computation of system matrix for area integral model-based algebraic reconstruction technique" in Slack
        double pixelSize = 1.0 / subjectResolution;
    }

    //what pixels each line intersects, always computes pixel area below line, never above
    void computeLineIntersections(pair<double, double> sourcePos, pair<double, double> detectorPos) {
/*
    if Line vertical

    if line horizontal
    
    if line slope up
        if m<=1
        else m>1

        if line-left enters left wall
        else line-left enters bottom wall

        march along until exit

        if line-right exits right wall
        else line-right exits top wall

    else line slope down
        if m<=1
        else m>1

        if line-right enters right wall
        else line-right enters bottom wall

        march along until exit

        if line-left exits left wall
        else line-left exits top wall

    // from the aforementioned paper:
    // delta = the width of each square pixel (1/subjectResolution), but for our uses, we leave it as *1* to reduce floating pt error
    // S1 = (2d - m*delta)*delta/2
    // S3 = delta^2 - (m*delta - d)^2/2m
    // S4 = d^2/2m
    
*/


        // First pixel intersected by a ray:
        // (0,0) is CENTERED TOP LEFT
        // n = pixels in each row/column
        // m = slope (it's a ratio, no units needed)
        // b = y-intercept of line, in units of pixels
        // c = x intercept of line, in units of pixels
        // D = radius of subject, which is a 1x1 square, so D = 0.5, but this is on the units of pixels, not the whole area, so 0.5 * subjectResolution

        // if -1 > m > 1 ; see pg4

        
        // if m > 0

        // h_left = height of intersect of left wall, from bottom (in pixels) = -m*D + b + D
        // w_bot = width of intersect of bottom wall from left (in pixels) = -(1/m)*D + c + D

        //   if 0 < h_left < n
        //     i = row index = n - 1 - floor(h_left/delta)
        //     j = 0
        //     d = h_left - i
        //   else
        //     i = 0
        //     j = column index = n - 1 - floor(w_bot/delta)
        //     d = w_bot - j

        // else m < 0

        // h_right = height of intersect of right wall, from bottom (in pixels) = m*D + b + D
        // w_bot = width of intersect of bottom wall from left (in pixels) = (1/m)*D + c + D

        //   if 0 < h_right < n
        //     i = row index = n - 1 - floor(h_right/delta)
        //     j = 0
        //     d = h_right - i
        //   else
        //     i = 0
        //     j = column index = n - 1 - floor(w_bot/delta)
        //     d = w_bot - j
        
        // k = i*n + j

        //"pixel traversing and line-pixel intersection areas calculation algorithm"
        // OUTPUT: # of pixels intersected by line, their indices, and the area *under* the ray for that pixel
        /*
        vector<int> index // stores indices, in order, of the pixels that our ray intersects
        vector<double> area // stores the areas, in order, of the pixels that our ray intersects
            //each value in area corresponds to the pixel at the same location in index

        num = 0 //counter for number of pixels the ray intersects

        while i >= 0 and j < n {
            d += m*delta
            if d > delta {
                d -= delta
                index[num] = k
                k -= n
                area[num] = S3
                num += 1
                i -= 1
                if i >= 0 { //while not run out of horizontal room yet
                    index[num] = k
                    k += 1
                    area = S4
                    num += 1
                    j += 1
                }
                else {
                    return num
                }
            }
            else if d < delta {
                index[num] = k
                k += 1
                area[num] = S1
                num += 1
                j += 1
            }
            else {
                d = 0
                index[num] = k
                k = k - n + 1
                area[num] = S1
                j += 1
                i -= 1
                num += 1
            }
        }
        return num
        */

       //NOTES:
       // This alg works when the ray exits a vertical boundary of the image. (see fig 2A) 
       // It may need to be modified according to the paragraphs after the algorithm to handle the case when the line exits a horizontal boundary
    }
};