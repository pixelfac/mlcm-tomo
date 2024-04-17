#include <iostream>

using namespace std;

struct vec2 {
    float x;
    float y;

    vec2(float _x, float _y) {
        x = _x;
        y = _y;
    }

    vec2() {
        x = 0;
        y = 0;
    }
};

vec2 upperLine;
vec2 lowerLine;

vec2 u_resolution(2,2);

float upper_line_at(float x);
float lower_line_at(float x);
vec2 get_slope_intercept_form(vec2 sourcePos, vec2 detectorPos);

int main()
{
    vec2 gl_FragCoord(0.5, 0.5);
    vec2 positions[] = {vec2(-1,-1), vec2(1,1), vec2(1,-1)};

    float pixelTop = ceil(gl_FragCoord.y);
    float pixelBottom = floor(gl_FragCoord.y);
    float pixelLeft = floor(gl_FragCoord.x);
    float pixelRight = ceil(gl_FragCoord.x);




    if (positions[1].y > positions[2].y) // if left side is higher than right side
    {
        upperLine = get_slope_intercept_form(positions[0], positions[1]);
        lowerLine = get_slope_intercept_form(positions[0], positions[2]);
    }
    else
    {
        upperLine = get_slope_intercept_form(positions[0], positions[2]);
        lowerLine = get_slope_intercept_form(positions[0], positions[1]);
    }

    cout << "upperLine.y: " << upperLine.y << endl;

    float areaTotal = 0;
    float areaTotalUpper;
    float areaTotalLower;

    //if slopes have same sign
    if ((upperLine.x>=0) == (lowerLine.x>=0))
    {
        cout << "same slope" << endl;
        //get area under upper line, bounded by top wall of pixel
        //calc total area under line, from pixelLeft to pixelRight
        float lineAtLeft = upper_line_at(pixelLeft);
        float lineAtRight = upper_line_at(pixelRight);
        areaTotalUpper = float(lineAtLeft + lineAtRight) * 0.5;
        cout << "lineAtLeft: " << lineAtLeft << endl;
        cout << "lineAtRight: " << lineAtRight << endl;
        cout << "areaTotalUpper: " << areaTotalUpper << endl;

        //if intersect pixelTop
        if (lineAtLeft - pixelTop != lineAtRight - pixelTop)
        {
            //if positive slope
            if (upperLine.x >= 0)
            {
                //find area of triangle between pixelTopRight, line@pixelRight, and intersectTop
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelTop - upperLine.y) / upperLine.x;
                //subtract triangle area
                areaTotalUpper -= (lineAtRight - pixelTop) * (pixelRight - intersectionPt) * 0.5;
            }
            //if negative slope
            else
            {
                //find area of triangle between pixelTopLeft, line@pixelLeft, and intersectTop
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelTop - upperLine.y) / upperLine.x;
                //subtract triangle area
                areaTotalUpper -= (lineAtLeft - pixelTop) * (intersectionPt - pixelLeft) * 0.5;
            }
        }
        //if line is completely over pixel
        else if (lineAtLeft - pixelTop > 0 || lineAtRight - pixelTop > 0)
        {
            areaTotalUpper = pixelTop; // area_rectangle = w*h, but width is 1, so area = h
        }

        //if intersect PixelBottom
        if (lineAtLeft - pixelBottom != lineAtRight - pixelBottom)
        {
            //if positive slope
            if (upperLine.x >= 0)
            {
                //find area of triangle between pixelBotLeft, line@pixelLeft, and intersectBot
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelBottom - upperLine.y) / upperLine.x;
                //Abot = (pixelBot - line@pixelLeft) * (intersectBot.x - pixelLeft) * 0.5;
                areaTotalUpper -= (pixelBottom - lineAtLeft) * (intersectionPt - pixelLeft) * 0.5;
            }
            //if negative slope
            else
            {
                //find area of triangle between pixelBotRight, line@pixelRight, and intersectBot
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelBottom - upperLine.y) / upperLine.x;
                //Abot = (pixelBot - line@pixelRight) * (pixelRight - intersectBot.x) * 0.5;
                areaTotalUpper -= (pixelBottom - lineAtRight) * (pixelRight - intersectionPt) * 0.5;
            }
        }

        //get area under lower line, bounded by bottom wall of pixel
        lineAtLeft = lower_line_at(pixelLeft);
        lineAtRight = lower_line_at(pixelRight);
        areaTotalLower = float(lineAtLeft + lineAtRight) * 0.5;

        //if intersect pixelTop
        if (lineAtLeft - pixelTop != lineAtRight - pixelTop)
        {
            //if positive slope
            if (lowerLine.x >= 0)
            {
                //find area of triangle between pixelTopRight, line@pixelRight, and intersectTop
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelTop - lowerLine.y) / lowerLine.x;
                //subtract triangle area
                areaTotalLower -= (lineAtRight - pixelTop) * (pixelRight - intersectionPt) * 0.5;
            }
            //if negative slope
            else
            {
                //find area of triangle between pixelTopLeft, line@pixelLeft, and intersectTop
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelTop - lowerLine.y) / lowerLine.x;
                //subtract triangle area
                areaTotalLower -= (lineAtLeft - pixelTop) * (intersectionPt - pixelLeft) * 0.5;
            }
        }

        //if intersect PixelBottom
        if (lineAtLeft - pixelBottom != lineAtRight - pixelBottom)
        {
            //if positive slope
            if (lowerLine.x >= 0)
            {
                //find area of triangle between pixelBotLeft, line@pixelLeft, and intersectBot
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelBottom - lowerLine.y) / lowerLine.x;
                //Abot = (pixelBot - line@pixelLeft) * (intersectBot.x - pixelLeft) * 0.5;
                areaTotalLower -= (pixelBottom - lineAtLeft) * (intersectionPt - pixelLeft) * 0.5;
            }
            //if negative slope
            else
            {
                //find area of triangle between pixelBotRight, line@pixelRight, and intersectBot
                //compute intersection point
                // x = (y-b)/m
                float intersectionPt = (pixelBottom - lowerLine.y) / lowerLine.x;
                //Abot = (pixelBot - line@pixelRight) * (pixelRight - intersectBot.x) * 0.5;
                areaTotalLower -= (pixelBottom - lineAtRight) * (pixelRight - intersectionPt) * 0.5;
            }
        }
        //if line is completely under pixel
        else if (lineAtLeft - pixelBottom < 0 || lineAtRight - pixelBottom < 0)
        {
            areaTotalLower = pixelBottom; // area_rectangle = w*h, but width is 1, so area = h
        }

        areaTotal = areaTotalUpper - areaTotalLower;
    }
    else
    {
        //special case where neither line is on top of the other
        //this will only ever show up in the corners of the image soemtimes maybe, for most resolution scales, so it is not really worth implementing
    }


    cout << "areaTotalUpper: " << areaTotalUpper << endl;
    cout << "areaTotalLower: " << areaTotalLower << endl;
    cout << "areaTotal: " << areaTotal << endl;
	// FragColor = vec4(areaTotal, areaTotal, areaTotal, 1.0); // vec4(r,g,b,a)
	// FragColor = vec4(1.0);

    return 0;
}

// x is in pixels
float upper_line_at(float x)
{
    return upperLine.x * x + upperLine.y; // y = mx + b
}

// x is in pixels
float lower_line_at(float x)
{
    return lowerLine.x * x + lowerLine.y; // y = mx + b
}

vec2 get_slope_intercept_form(vec2 sourcePos, vec2 detectorPos)
{
    float slope = (detectorPos.y - sourcePos.y) / (detectorPos.x - sourcePos.x);    // slope
    // float b = (sourcePos.y - slope * sourcePos.x + 1.0) * float(u_resolution.x);    // y intercept in units of pixels
    float b;
    if (slope == 0) {
        b = sourcePos.y;
    }
    else {
        float b = (-1 * sourcePos.y / slope + sourcePos.x);
    }

    b = (b + 1) * float(u_resolution.x * 0.5);

    // cout << "slope: " << slope << endl;
    // cout << "b: " << b << endl;
    return vec2(slope, b);
}