#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

layout(location = 0) out vec4 FragColor;

uniform vec2 u_resolution;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

vec2 upperLine;
vec2 lowerLine;

float pixelTop = ceil(gl_FragCoord.y);
float pixelBottom = floor(gl_FragCoord.y);
float pixelLeft = floor(gl_FragCoord.x);
float pixelRight = ceil(gl_FragCoord.x);

flat in vec2 sourcePos;
flat in vec2 detectorLeftPos;
flat in vec2 detectorRightPos;

float upper_line_at(float x);
float lower_line_at(float x);
vec2 get_point_slope_form(vec2 sourcePos, vec2 detectorPos);

void main()
{
    if (detectorLeftPos.y > detectorRightPos.y) // if left side is higher than right side
    {
        upperLine = get_point_slope_form(sourcePos, detectorLeftPos);
        lowerLine = get_point_slope_form(sourcePos, detectorRightPos);
    }
    else
    {
        upperLine = get_point_slope_form(sourcePos, detectorRightPos);
        lowerLine = get_point_slope_form(sourcePos, detectorLeftPos);
    }



    //if slopes have same sign
    if ((upperLine.x>=0) == (lowerLine.x>=0))
    {
        //get area under upper line, bounded by top wall of pixel
        //calc total area under line, from pixelLeft to pixelRight

        //if intersect pixelTop
            //if positive slope
                //find area of triangle between pixelTopRight, line@pixelRight, and intersectTop
                //Atop = (line@pixelRight - pixelTop) * (pixelRight - intersectTop.x) * 0.5;
            //if negative slope
                //find area of triangle between pixelTopLeft, line@pixelLeft, and intersectTop
                //Atop = (line@pixelLeft - pixelTop) * (intersectTop.x - pixelLeft) * 0.5;
            //total area - Atop

        //if intersect PixelBottom
            //if positive slope
                //find area of triangle between pixelBotLeft, line@pixelLeft, and intersectBot
                //Abot = (pixelBot - line@pixelLeft) * (intersectBot.x - pixelLeft) * 0.5;
            //if negative slope
                //find area of triangle between pixelBotRight, line@pixelRight, and intersectBot
                //Abot = (pixelBot - line@pixelRight) * (pixelRight - intersectBot.x) * 0.5;
            //total area + Abot

        //get area under lower line, bounded by bottom wall of pixel
    }
    else
    {
        //special case where neither line is on top of the other
    }

    //if upperLine intersects top wall of pixel
    // if (upperLeftCorner.y != upperRightCorner.y)
    // {
    //     //compute intersection point
    //     // x = (y-b)/m
    //     float intersectionPt = (pixelTopLeft.y + upperLine.y) / upperLine.x;

    //     //calculate 
    // }

	FragColor = vec4(sourcePos.y,detectorLeftPos.y,detectorRightPos.y,0.5); // vec4(r,g,b,a)
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

