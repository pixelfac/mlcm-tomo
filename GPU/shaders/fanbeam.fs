#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

layout(location = 0) out vec4 FragColor;

uniform vec2 u_resolution;

vec2 upperLine;
vec2 lowerLine;

float pixelTop = ceil(gl_FragCoord.y);
float pixelBottom = floor(gl_FragCoord.y);
float pixelLeft = floor(gl_FragCoord.x);
float pixelRight = ceil(gl_FragCoord.x);

flat in vec2 positions[3];

float upper_line_at(float x);
float lower_line_at(float x);
vec2 get_point_slope_form(vec2 sourcePos, vec2 detectorPos);

void main()
{
    if (positions[1].y > positions[2].y) // if left side is higher than right side
    {
        upperLine = get_point_slope_form(positions[0], positions[1]);
        lowerLine = get_point_slope_form(positions[0], positions[2]);
    }
    else
    {
        upperLine = get_point_slope_form(positions[0], positions[2]);
        lowerLine = get_point_slope_form(positions[0], positions[1]);
    }

    float areaTotal = 0;

    //if slopes have same sign
    if ((upperLine.x>=0) == (lowerLine.x>=0))
    {
        //get area under upper line, bounded by top wall of pixel
        //calc total area under line, from pixelLeft to pixelRight
        float lineAtLeft = upper_line_at(pixelLeft);
        float lineAtRight = upper_line_at(pixelRight);
        float areaTotalUpper = float(lineAtLeft + lineAtRight) * 0.5;

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
        float areaTotalLower = float(lineAtLeft + lineAtRight) * 0.5;

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

	FragColor = vec4(positions[0].y,positions[1].y,positions[2].y,0.5); // vec4(r,g,b,a)
}

// x is in pixels
float upper_line_at(float x)
{
    return upperLine.x * x + upperLine.y + float(u_resolution * 0.5); // y = mx + b
}

// x is in pixels
float lower_line_at(float x)
{
    return lowerLine.x * x + lowerLine.y + float(u_resolution * 0.5); // y = mx + b
}

vec2 get_point_slope_form(vec2 sourcePos, vec2 detectorPos)
{
    float slope = (detectorPos.y - sourcePos.y) / (detectorPos.x - sourcePos.x);    // slope
    float b = (sourcePos.y - slope * sourcePos.x) * float(u_resolution);            // y intercept in units of pixels

    return vec2(slope, b);
}

// TODO
// get corner coords of given pixel
// func to get y given x,slope,intercept
// compute area under upper line
// compute area under lower line

