#include <stdio.h>
#include <utility>
#include <math.h>
#include "VectorUtils.h"
#include <vector>

using namespace std;

class CTScanner {
public:
    int views; //number of views, spaced equally around the subject (e.g. 36 views is one every 10 degrees)
    double sourceDist; //distance from source ray emitter to center of subject. Subject diagonal is sqrt(2)/2
    double detectorDist; //distance from center detector to center of subject. Subject diagonal is sqrt(2)/2
    double detectorPanelWidth; //total width of entire detector panel
    int numDetectors; //number of equal-sized detectors on detector panel.
    double pixelSize;

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
        
        pixelSize = 1.0 / subjectResolution; //this is delta?
    }

    //what pixels each line intersects, always computes pixel area below line, never above
    void computeLineIntersections(pair<double, double> sourcePos, pair<double, double> detectorPos) {
    
        //calculate the slope of the line from the source to the detector using y2 - y1/x2-x1
        double slope = (detectorPos.second - sourcePos.second)/(detectorPos.first - sourcePos.first);
        double b = sourcePos.second - slope*sourcePos.first;
        double c = sourcePos.first - slope*sourcePos.second;
        int delta = 1;
        /*

            // from the aforementioned paper:
            // delta = the width of each square pixel (1/subjectResolution), but for our uses, we leave it as *1* to reduce floating pt error
            // S1 = (2d - m*delta)*delta/2
            // S3 = delta^2 - (m*delta - d)^2/2*m
            // S4 = d^2/2*m
            
        */
        
        if(isinf(slope))  //the line is strictly vertical
        {
            /*
                c = x intercept of line, in units of pixels
                w_top = height of intersect of left wall, from bottom (in pixels) = c + D
                i = 0
                j = column index = n - 1 - floor(w_top/delta)
                d = w_top - j
                num = 0;
                k = i*n + j

                vector<int> index // stores indices, in order, of the pixels that our ray intersects
                vector<double> area // stores the areas, in order, of the pixels that our ray intersects
                //each value in area corresponds to the pixel at the same location in index

                for (; i < n; i++) {
                    index[num] = k
                    k += 1
                    area[num] = d*delta
                    num += 1
                }
                return num
            */
            double w_top = c + 0.5; //height of the intersect of left wall from bottom
            int j = subjectResolution - 1 - floor(w_top/delta); //column index
            double d = w_top - j; //distance
            int num  = 0; //counter
            int k = j; // index

            vector<int> index; // stores indices, in order, of the pixels that our ray intersects
            vector<double> area; // stores the areas, in order, of the pixels that our ray intersects

            for(int i = 0; i < subjectResolution; i++)
            {
                index.push_back(k);
                k += 1;
                area.push_back(d * delta);
                num += 1;
            }
        }
        else if(slope == 0) //the line is strictly horizontal
        {
            /*
                 b = y-intercept of line, in units of pixels
                h_left = height of intersect of left wall, from bottom (in pixels) = b + D
                i = row index = n - 1 - floor(h_left/delta)
                j = 0
                d = h_left - i
                num = 0;
                k = i*n + j

                vector<int> index // stores indices, in order, of the pixels that our ray intersects
                vector<double> area // stores the areas, in order, of the pixels that our ray intersects
                //each value in area corresponds to the pixel at the same location in index

                for (; j < n; j++) {
                    index[num] = k
                    k += 1
                    area[num] = d*delta
                    num += 1
                }
                return num
            */
            double h_left = b + 0.5; // height of intersect of left wall from bottom
            int i = subjectResolution - 1 - floor(h_left / delta); // row index
            double d = h_left - i; // distance
            int num = 0; //Counter for number of pixels the ray intersects
            int k = i * subjectResolution; // index

            vector<int> index; // stores indices, in order, of the pixels that our ray intersects
            vector<double> area; // stores the areas, in order, of the pixels that our ray intersects

            for(int j = 0; j < subjectResolution; j++)
            {
                index.push_back(k);
                k += 1;
                area.push_back(d * delta);
                num += 1;
            }
        }
        
                // First pixel intersected by a ray:
                // (0,0) is CENTERED TOP LEFT
                // n = pixels in each row/column
                // m = slope (it's a ratio, no units needed)
                // b = y-intercept of line, in units of pixels
                // c = x intercept of line, in units of pixels
                // D = radius of subject, which is a 1x1 square, so D = 0.5, but this is on the units of pixels, not the whole area, so 0.5 * subjectResolution

                //TODO if -1 > m > 1
                // NOT SURE, NEEDS TESTING
                // for m < -1, m > 1: m=1/m, swap b<=>c, this will yield 
                // when <increasing/decreasing> <i/j> to <0/n>, invert it to <increasing/decreasing> <j/i> to <n/0>
                // to convert k out of inverted coordinatesk = k <+/-> <n/1> --> k = k <-/+> <i/n>
                // k = (n-1-j)*n + (n-1-i)

                // find starting points for  
                // if m > 0

                // h_left = height of intersect of left wall, from bottom (in pixels) = -m*D + b + D
                // w_bot = width of intersect of bottom wall from left (in pixels) = -(1/m)*D + c + D

                //   if 0 < h_left < n
                //     i = row index = n - 1 - floor(h_left/delta)
                //     j = 0
                //     d = h_left - i
                //   else
                //     i = n-1
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
                //     i = n-1
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

                // case for 0 < m < 1

                if i == n - 1    // if start is bottom wall
                    d -= delta
                    index[num] = k
                    k += 1
                    area[num] = S4
                    num += 1
                    j += 1

                while i >= 0 and j < n {
                    d += m*delta
                    if d > delta {
                        d -= delta
                        index[num] = k
                        area[num] = S3
                        num += 1
                        i -= 1
                        k -= n
                        if i >= 0 { //while not run out of horizontal room yet
                            index[num] = k
                            area[num] = S4
                            num += 1
                            j += 1
                            k += 1
                        }
                        else {
                            return num
                        }
                    }
                    else if d < delta {
                        index[num] = k
                        area[num] = S1
                        num += 1
                        j += 1
                        k += 1
                    }
                    else {
                        d = 0
                        index[num] = k
                        area[num] = S1
                        j += 1
                        i -= 1
                        k = k - n + 1
                        num += 1
                    }
                }
                return num


                // for -1 < m < 0: repeat the above 0 < m < 1 case but anything involving j and k is switched from j increasing to approach n, to j decreasing to approach 0

                // NOT SURE, NEEDS TESTING
                // for m > 1: repeat above 0 < m < 1 case, but inverse slope to m = 1/m, switch b<=>c, still use old m,b,c to determine i,j 

                // for m < -1: repeat above -1 < m < 0 case, but inverse slope to m = 1/m, switch b<=>c, still use old m,b,c to determine i,j


                */
        else if(slope > 0) //the line is sloping up
        {
            /*
                if m<=1
                else m>1

                if line-left enters left wall
                else line-left enters bottom wall

                march along until exit

                if line-right exits right wall
                else line-right exits top wall
            */

            // Check if the slope is less than or equal to 1
            if (slope <= 1)
            {
                // Calculate the height of intersect of left wall
                double h_left = (-slope * 0.5) + b + 0.5; // D = 0.5

                // Calculate the width of intersect of bottom wall from left
                double w_bot = (-(1 / slope) * 0.5) + c + 0.5; // D = 0.5

                int i, j;
                double d;

                // Check if h_left is within the range of pixels
                if (h_left >= 0 && h_left < subjectResolution)
                {
                    // Start from the left wall
                    i = subjectResolution - 1 - floor(h_left / delta);
                    j = 0;
                    d = h_left - i * delta;
                }
                else
                {
                    // Start from the bottom wall
                    i = subjectResolution - 1;
                    j = subjectResolution - 1 - floor(w_bot / delta);
                    d = w_bot - j * delta;
                }

                int num = 0;
                int k = i * subjectResolution + j;

                vector<int> index;
                vector<double> area;

                // Pixel traversing and line-pixel intersection areas calculation algorithm
                while (i >= 0 && j < subjectResolution)
                {
                    d += slope * delta;
                    if (d > delta)
                    {
                        d -= delta;
                        index.push_back(k);
                        area.push_back(delta * delta - pow((slope * delta - d), 2) / (2 * slope));
                        num++;
                        i--;
                        k -= subjectResolution;
                        if (i >= 0)
                        {
                            index.push_back(k);
                            area.push_back(pow(d, 2) / (2 * slope));
                            num++;
                            j++;
                            k++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if (d < delta)
                    {
                        index.push_back(k);
                        area.push_back((2 * d - slope * delta) * delta / 2);
                        num++;
                        j++;
                        k++;
                    }
                    else
                    {
                        d = 0;
                        index.push_back(k);
                        area.push_back((2 * d - slope * delta) * delta / 2);
                        j++;
                        i--;
                        k = k - subjectResolution + 1;
                        num++;
                    }
                }
            }
            else // Slope is greater than 1
            {
                // Calculate the height of intersect of right wall
                double h_right = (slope * 0.5) + b + 0.5; // D = 0.5

                // Calculate the width of intersect of bottom wall from left
                double w_bot = ((1 / slope) * 0.5) + c + 0.5; // D = 0.5

                int i, j;
                double d;

                // Check if h_right is within the range of pixels
                if (h_right >= 0 && h_right < subjectResolution)
                {
                    // Start from the right wall
                    i = subjectResolution - 1 - floor(h_right / delta);
                    j = 0;
                    d = h_right - i * delta;
                }
                else
                {
                    // Start from the bottom wall
                    i = subjectResolution - 1;
                    j = subjectResolution - 1 - floor(w_bot / delta);
                    d = w_bot - j * delta;
                }

                int num = 0;
                int k = i * subjectResolution + j;

                vector<int> index;
                vector<double> area;

                // Pixel traversing and line-pixel intersection areas calculation algorithm
                while (i >= 0 && j < subjectResolution)
                {
                    d -= slope * delta;
                    if (d > delta)
                    {
                        d -= delta;
                        index.push_back(k);
                        area.push_back(pow(d, 2) / (2 * slope));
                        num++;
                        i--;
                        k -= subjectResolution;
                        if (i >= 0)
                        {
                            index.push_back(k);
                            area.push_back((2 * d - slope * delta) * delta / 2);
                            num++;
                            j++;
                            k++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if (d < delta)
                    {
                        index.push_back(k);
                        area.push_back((slope * delta - d) * delta / 2);
                        num++;
                        j++;
                        k++;
                    }
                    else
                    {
                        d = 0;
                        index.push_back(k);
                        area.push_back((slope * delta - d) * delta / 2);
                        j++;
                        i--;
                        k = k - subjectResolution + 1;
                        num++;
                    }
                }
            }
        }
        else if(slope < 0) //the line is sloping down
        {
            /*
                if m<=1
                else m>1

                if line-right enters right wall
                else line-right enters bottom wall

                march along until exit

                if line-left exits left wall
                else line-left exits top wall
            */
            if (abs(slope) <= 1) // slope <= 1
            {
                // Determine whether line-left enters the left wall or bottom wall
                // This depends on the y-intercept of the line.
                // If b < 0.5, it enters the left wall. Otherwise, it enters the bottom wall.
                double b = sourcePos.second; // y-intercept of the line
                double D = 0.5 * subjectResolution; // Radius of the subject
                double h_left = -slope * D + b + D; // height of intersect of left wall, from bottom
                int i, j;
                double d;
                if (0 < h_left && h_left < subjectResolution)
                {
                    i = subjectResolution - 1 - floor(h_left / delta);
                    j = 0;
                    d = h_left - i;
                }
                else
                {
                    i = subjectResolution - 1;
                    double w_bot = -(1 / slope) * D + sourcePos.first + D; // width of intersect of bottom wall from left
                    j = subjectResolution - 1 - floor(w_bot / delta);
                    d = w_bot - j;
                }

                // Traverse along the line until exit
                int num = 0;
                int k = i * subjectResolution + j;
                vector<int> index;
                vector<double> area;
                while (i >= 0 && j < subjectResolution)
                {
                    d += slope * delta;
                    if (d > delta)
                    {
                        d -= delta;
                        index.push_back(k);
                        area.push_back((delta * delta - (slope * delta - d) * (slope * delta - d)) / (2 * slope));
                        num++;
                        i--;
                        k -= subjectResolution;
                        if (i >= 0)
                        {
                            index.push_back(k);
                            area.push_back(d * delta / 2);
                            num++;
                            j++;
                            k++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if (d < delta)
                    {
                        index.push_back(k);
                        area.push_back((delta - d) * delta / 2);
                        num++;
                        j++;
                        k++;
                    }
                    else
                    {
                        d = 0;
                        index.push_back(k);
                        area.push_back(delta * delta / 2);
                        j++;
                        i--;
                        k = k - subjectResolution + 1;
                        num++;
                    }
                }
            }
            else // slope > 1
            {
                // Similar to the case when the slope is less than 1, but we invert the role of x and y.
                // So, wherever we used to calculate the left wall, now we calculate the top wall, and vice versa.
                double D = 0.5 * subjectResolution; // Radius of the subject
                double w_top = slope * D + c + D; // width of intersect of top wall, from left
                int j, i;
                double d;
                if (0 < w_top && w_top < subjectResolution)
                {
                    j = subjectResolution - 1 - floor(w_top / delta);
                    i = 0;
                    d = w_top - j;
                }
                else
                {
                    j = subjectResolution - 1;
                    double h_right = slope * D + b + D; // height of intersect of right wall, from bottom
                    i = subjectResolution - 1 - floor(h_right / delta);
                    d = h_right - i;
                }

                // Traverse along the line until exit
                int num = 0;
                int k = i * subjectResolution + j;
                vector<int> index;
                vector<double> area;
                while (i < subjectResolution && j >= 0)
                {
                    d += (1 / slope) * delta;
                    if (d > delta)
                    {
                        d -= delta;
                        index.push_back(k);
                        area.push_back((delta * delta - ((1 / slope) * delta - d) * ((1 / slope) * delta - d)) / (2 / slope));
                        num++;
                        j--;
                        k--;
                        if (j >= 0)
                        {
                            index.push_back(k);
                            area.push_back(d * delta / 2);
                            num++;
                            i++;
                            k = k + subjectResolution;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if (d < delta)
                    {
                        index.push_back(k);
                        area.push_back((delta - d) * delta / 2);
                        num++;
                        i++;
                        k = k + subjectResolution;
                    }
                    else
                    {
                        d = 0;
                        index.push_back(k);
                        area.push_back(delta * delta / 2);
                        i++;
                        j--;
                        k = k + subjectResolution - 1;
                        num++;
                    }
                }
            }
        }
    }
};