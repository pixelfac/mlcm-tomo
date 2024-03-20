#include <stdio.h>
#include <utility>
#include <math.h>
#include "VectorUtils.h"
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class CTScanner
{
public:
    int views;                 // number of views, spaced equally around the subject (e.g. 36 views is one every 10 degrees)
    double sourceDist;         // distance from source ray emitter to center of subject. Subject diagonal is sqrt(2)/2
    double detectorDist;       // distance from center detector to center of subject. Subject diagonal is sqrt(2)/2
    double detectorPanelWidth; // total width of entire detector panel
    int numDetectors;          // number of equal-sized detectors on detector panel.

    // subject is a square sized 1x1 units, centered at the origin, so bounds are (-0.5, 0.5) in both axes.
    // for simplicity and comparison, we are only working with square imaging tasks
    // These units are the same as determining the source and detector distance.
    // The diagonal from the subject to it's furthest corner is sqrt(2)/2
    int subjectResolution; // how many pixels each dimension of the subject should be partitioned into

    CTScanner(int _views, double _sourceDist, double _detectorDist, double _detectorPanelWidth, int _numDetectors, int _subjectResolution) {
        views = _views;
        sourceDist = _sourceDist;
        detectorDist = _detectorDist;
        detectorPanelWidth = _detectorPanelWidth;
        numDetectors = _numDetectors;
        subjectResolution = _subjectResolution;
    }

    // returns a coordinate pair for the position of the source, factoring in the rotation from the current view
    pair<double, double> GetCurrentSourcePosition(int viewNum)
    {
        pair<double, double> initialPosition(0.0, sourceDist);
        // if viewNum == 0, then source has not moved from initial position, (0, sourceDist)
        double degreesRotated = viewNum * (360.0 / views);

        // TODO how to make rotation matrix to rotate a vector by a set number of degrees/radians?
        return rotate(initialPosition, degreesRotated);
    }

    // returns a pair of coordinates for the position of the detector, factoring in the rotation from the current view and position along detector panel
    //  INPUT: detectorNum is 0-indexed, so #0 means left side of detector
    //  RETURN: first pair is leftmost coord, second pair is right side of detector
    pair<pair<double, double>, pair<double, double>> GetCurrentDetectorPosition(int viewNum, int detectorNum)
    {
        pair<double, double> detectorLeftSide(0.0, detectorDist);  // leftmost range of this detector pixel
        pair<double, double> detectorRightSide(0.0, detectorDist); // rightmost range of this detector pixel

        double detectorWidth = detectorPanelWidth / numDetectors; // width of each individual detector pixel
        detectorLeftSide.first = (detectorWidth * detectorNum) - (detectorPanelWidth / 2);
        detectorRightSide.first = detectorLeftSide.first + detectorWidth;

        // if viewNum == 0, then source has not moved from initial position, (0, detectorDist)
        double degreesRotated = viewNum * (360.0 / views);

        // TODO how to make rotation matrix to rotate a vector by a set number of degrees/radians?
        detectorLeftSide = rotate(detectorLeftSide, degreesRotated);
        detectorRightSide = rotate(detectorRightSide, degreesRotated);

        return pair<pair<double, double>, pair<double, double>>(detectorLeftSide, detectorRightSide);
    }

    // simulate a fan of photons from source, through subject, and hitting a specific detector
    //  viewNum is which
    //  detectorNum is which detector on the panel this fan beam is projected to
    //  calls computeLineIntersections for both lines in the fan beam
    //  combines area to get complete list of pixels with their areas
    void computeFanIntersections(int viewNum, int detectorNum)
    {
        // compute fanbeam triangle using source position, detector panel position, and which detector on the panel is receiving
        pair<double, double> sourcePosition = GetCurrentSourcePosition(viewNum);

        auto detectorPosition = GetCurrentDetectorPosition(viewNum, detectorNum);

        // TODO call computeLineIntersections for both lines
        // auto lineLeft = computeLineIntersections(sourcePosition, detectorPosition.first)
        // auto lineRight = computeLineIntersections(sourcePosition, detectorPosition.second)

        // checking for overlap and pixel area
        //  see "Fast and accurate computation of system matrix for area integral model-based algebraic reconstruction technique" in Slack

        double pixelSize = 1.0 / subjectResolution; // length of each pixel as a fraction of the total subject length

        /*
        map<int, double> Area // map that correlates grid index, k, with the area of the pixel at k that is in the fan.

        if detectorPosition.first.second > detectorPosition.second.second // if left side is higher than right side
            lineUpper = lineLeft
            lineLower = lineRight
        else
            lineUpper = lineRight
            lineLower = lineLeft



        //find end points on upper line
        if (lineUpper.j[0] < lineUpper.j[last])
            lineUpperStart = 0
            lineUpperEnd = last
        else
            lineUpperStart = last
            lineUpperEnd = 0

        //find end points on lower line
        if (lineLower.j[0] < lineLower.j[last])
            lineLowerStart = 0
            lineLowerEnd = last
        else
            lineLowerStart = last
            lineLowerEnd = 0



//compute group 1, the left third of the grid, where only one line is present
        //group 1 can be empty
        if lineUpperStart < lineLowerStart
            //from [lineUpperStart, lineLowerStart) all pixels below lineUpper are 100% in fanbeam
            group1Start = lineUpperStart
            group2Start = lineLowerStart

            for (index = lineUpperStart; lineUpper.j[index] < lineUpper.j[group2Start];)
                maxI = lineUpper.i[index]
                currJ = lineUpper.j[index]

                //if the line has slope<-1, then there will be multiple pixels in same j, so need to find the lowest one to serve as the boundary
                while lineUpper.k[index] == currJ
                    if lineUpper.i[index] > maxI
                        maxI = lineUpper.i[index]
                    Area[lineUpper.k[index]] = lineUpper.A[index]
                    index++
                    
                //all pixels below are 100% included, so area=1

                while maxI*n + currJ < n*n
                    Area[maxI*n + currJ] = 1
                    maxI++
        else
            //from [lineLowerStart, lineUpperStart) all pixels above lineLower are 100% in fanbeam
            group1Start = lineLowerStart
            group2Start = lineUpperStart

            for (index = lineLowerStart; lineLower.j[index] < lineLower.j[group2Start];)
                minI = lineLower.i[index]
                currJ = lineLower.j[index]

                //if the line has slope>1, then there will be multiple pixels in same j, so need to find the highest one to serve as the boundary
                while lineLower.k[index] == currJ
                    if lineLower.i[index] < minI
                        minI = lineLower.i[index]
                    Area[lineLower.k[index]] = lineLower.A[index]
                    index++
                    
                //all pixels above are 100% included, so area=1

                while minI*n + currJ >= 0
                    Area[minI*n + currJ] = 1
                    minI--



//compute group 3, the right third of the grid, where only one line is present
        if lineUpperEnd > lineLowerEnd
            //from (lineLowerEnd, lineUpperEnd], all pixels below are 100% in fanbeam
            group2End = lineLowerEnd
            group3End = lineUpperEnd

            for (index = group3End; lineUpper.j[index] > lineUpper.j[group2End];)
                maxI = lineUpper.i[index]
                currJ = lineUpper.j[index]

                //if the line has slope<-1, then there will be multiple pixels in same j, so need to find the lowest one to serve as the boundary
                while lineUpper.k[index] == currJ
                    if lineUpper.i[index] > maxI
                        maxI = lineUpper.i[index]
                    Area[lineUpper.k[index]] = lineUpper.A[index]
                    index--
                    
                //all pixels below are 100% included, so area=1

                while maxI*n + currJ < n*n
                    Area[maxI*n + currJ] = 1
                    maxI++
        else
            //from (lineUpperEnd, lineLowerEnd], all pixels above are 100% in fanbeam
            group2End = lineUpperEnd
            group3End = lineLowerEnd

            for (index = lineLowerEnd; lineLower.j[index] > lineLower.j[lineUpperEnd];)
                minI = lineLower.i[index]
                currJ = lineLower.j[index]

                //if the line has slope>1, then there will be multiple pixels in same j, so need to find the highest one to serve as the boundary
                while lineLower.k[index] == currJ
                    if lineLower.i[index] < minI
                        minI = lineLower.i[index]
                    Area[lineLower.k[index]] = lineLower.A[index]
                    index--
                    
                //all pixels above are 100% included, so area=1

                while minI*n + currJ >= 0
                    Area[minI*n + currJ] = 1
                    minI--



        group2 = [group2Start, group2End) //intersects both upper and lower lines
        */
    }

    // what pixels each line intersects, always computes pixel area below line, never above
    pair<vector<double>, pair<vector<int>, vector<int>>> computeLineIntersections(pair<double, double> sourcePos, pair<double, double> detectorPos) 
    {
        // calculate the slope of the line from the source to the detector using y2 - y1/x2-x1
        double slope = (detectorPos.second - sourcePos.second) / (detectorPos.first - sourcePos.first);
        double b = (sourcePos.second - slope * sourcePos.first) * subjectResolution; // y intercept in units of pixels
        double c = (sourcePos.first - (1.0/slope) * sourcePos.second) * subjectResolution; // x intercept in units of pixels
        double D = 0.5 * subjectResolution;                                          // half-width/half-height of subject area, in units of pixels
        double delta = 1;

        //store output
        //key = index in subject grid
        //value = area under line at that pos in subject grid
        vector<double> A;
        vector<int> kList;
        vector<int> jList;
        /*
            // reference from the aforementioned paper:
            // delta = the width of each square pixel (1/subjectResolution), but for our uses, we leave it as *1* to reduce floating pt error
            // S1 = (2d - m*delta)*delta/2
            // S3 = delta^2 - (m*delta - d)^2/2*m
            // S4 = d^2/2/m
        */
        cout << "Slope: " << slope << endl;

        if (isinf(slope)) // the line is strictly vertical
        {
            cout << "Vertical Line Case" << endl;

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
            double w_top = (sourcePos.first*subjectResolution) + D;                                 // height of the intersect of left wall from bottom
            cout << "w_top: " << w_top << endl;
            int j = floor(w_top / delta); // column index
            cout << "j: " << j << endl;
            double d = w_top - floor(w_top / delta);                                 // distance
            int num = 0;                                          // counter
            int k = j;                                            // index

            for (int i = 0; i < subjectResolution; i++)
            {
                //computed area is to the left of the line, since there's no "under" for a vertical line
                A.push_back(d*delta);
                kList.push_back(k);
                jList.push_back(j);
                k += subjectResolution;
                num += 1;
            }
        }
        else if (slope == 0) // the line is strictly horizontal
        {
            cout << "Horizontal Line Case" << endl;
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
            double h_left = (sourcePos.second*subjectResolution) + D;                                 // height of intersect of left wall from bottom
            int i = subjectResolution - 1 - floor(h_left / delta); // row index
            double d = h_left - floor(h_left / delta);                                 // distance
            int num = 0;                                           // Counter for number of pixels the ray intersects
            int k = i * subjectResolution;                         // index

            for (int j = 0; j < subjectResolution; j++)
            {
                A.push_back(d*delta);
                kList.push_back(k);
                jList.push_back(j);
                k += 1;
                num += 1;
            }
        }

        // i=0,j=0 is TOP LEFT
        // n = pixels in each row/column
        // m = slope (it's a ratio, no units needed)
        // b = y-intercept of line, in units of pixels
        // c = x intercept of line, in units of pixels
        // D = radius of subject, which is a 1x1 square, so D = 0.5, but this is on the units of pixels, not the whole area, so 0.5 * subjectResolution

        // TODO if -1 > m > 1
        // NOT SURE, NEEDS TESTING
        // for m < -1, m > 1: m=1/m, swap b<=>c, this will yield
        // when <increasing/decreasing> <i/j> to <0/n>, invert it to <increasing/decreasing> <j/i> to <n/0>
        // to convert k out of inverted coordinatesk = k <+/-> <n/1> --> k = k <-/+> <i/n>
        // k = (n-1-j)*n + (n-1-i)

        // h_left = height of intersect of left wall, from bottom (in pixels) = -m*D + b + D
        // w_bot = width of intersect of bottom wall from left (in pixels) = -(1/m)*D + c + D
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

        */

        else if (abs(slope) <= 1)
        { // -1 <= slope <= 1
            if (slope > 0)
            {
                int num = 0;

                // Calculate the height of intersect of left wall
                double h_left = (-1 * slope * D) + b + D;
 
                int i, j;
                double d;

                // Check if h_left is within the range of pixels
                if (0 <= h_left && h_left < subjectResolution)
                {
                    // Start from the left wall
                    i = subjectResolution - 1 - floor(h_left / delta);
                    j = 0; // leftmost column
                    d = h_left - floor(h_left / delta);
                }
                else
                {
                    // Start from the bottom wall
                    // Calculate the width of intersect of bottom wall from left
                    double w_bot = (-1 * (1.0f / slope) * D) + c + D;

                    i = subjectResolution - 1; // bottom row
                    j = floor(w_bot / delta);
                    d = slope*(1 - (w_bot - j));

                    // insert first triangle and move to next pixel so starting loop intersecting a vertical boundary
                    // like the left wall case
                    A.push_back(d*d/slope/2);
                    kList.push_back(i*subjectResolution + j);
                    jList.push_back(j);
                    num += 1;
                    j += 1;
                }

                int k = i * subjectResolution + j;

                // Pixel traversing and line-pixel intersection areas calculation algorithm
                while (i >= 0 && j < subjectResolution)
                {
                    d += slope * delta;
                    if (d > delta)
                    {
                        d -= delta;
                        A.push_back(delta * delta - pow((slope * delta - d), 2) / (2 * slope));
                        kList.push_back(k);
                        jList.push_back(j);
                        num++;
                        i--;
                        k -= subjectResolution;
                        if (i >= 0)
                        {
                            A.push_back(pow(d, 2) / (2 * slope));
                            kList.push_back(k);
                            jList.push_back(j);
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
                        A.push_back((2 * d - slope * delta) * delta / 2);
                        kList.push_back(k);
                        jList.push_back(j);
                        num++;
                        j++;
                        k++;
                    }
                    else // d = delta
                    {
                        A.push_back((2 * d - slope * delta) * delta / 2);
                        kList.push_back(k);
                        jList.push_back(j);
                        d = 0;
                        j++;
                        i--;
                        k = k - subjectResolution + 1;
                        num++;
                    }
                }
            }
            else
            { // slope < 0

                int num = 0;

                // Determine whether line-right enters the right wall or bottom wall
                // This depends on the y-intercept of the line.
                double h_right = slope * D + b + D; // height of intersect of right wall, from bottom

                int i, j;
                double d;

                if (0 <= h_right && h_right < subjectResolution)
                {
                    i = subjectResolution - 1 - floor(h_right / delta);
                    j = subjectResolution - 1; // rightmost column
                    d = h_right - floor(h_right / delta);
                }
                else
                {
                    double w_bot = (-1 * (1.0f / slope) * D) + c + D;
                    i = subjectResolution - 1; // bottom row
                    j = floor(w_bot / delta);
                    d = -1 * slope*(w_bot - j);

                    A.push_back(d*d/slope/-2);
                    kList.push_back(i*subjectResolution + j);
                    jList.push_back(j);
                    num += 1;
                    j -= 1;
                }

                // Traverse along the line until exit
                
                int k = i * subjectResolution + j;
                while (i >= 0 && j >= 0)
                {
                    d -= slope * delta; //since the line slopes down, reverse up the line
                    if (d > delta)
                    {
                        d -= delta;
                        A.push_back(delta * delta - pow((-1 * slope * delta - d), 2) / (-2 * slope));
                        kList.push_back(k);
                        jList.push_back(j);
                        num++;
                        i--;
                        k -= subjectResolution;
                        if (i >= 0)
                        {
                            A.push_back(pow(d, 2) / (-2 * slope));
                            kList.push_back(k);
                            jList.push_back(j);
                            num++;
                            j--;
                            k--;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if (d < delta)
                    {
                        A.push_back((2 * d + slope * delta) * delta / 2);
                        kList.push_back(k);
                        jList.push_back(j);
                        num++;
                        j--;
                        k--;
                    }
                    else
                    {
                        A.push_back((2 * d + slope * delta) * delta / 2);
                        kList.push_back(k);
                        jList.push_back(j);
                        d = 0;
                        j--;
                        i--;
                        k = k - subjectResolution - 1;
                        num++;
                    }
                }
            }
        }
        else // abs(slope) > 1
        {
            if (slope > 0) // slope > 1
            {
                cout << "slope>1 case not implemented yet" << endl;
            }
            else // slope < -1
            {
                cout << "slope<-1 case not implemented yet" << endl;
            }
        }

        pair<vector<int>, vector<int>> lists(kList, jList);
        pair<vector<double>, pair<vector<int>, vector<int>>> rtrn(A, lists);
        return rtrn;
    }
};