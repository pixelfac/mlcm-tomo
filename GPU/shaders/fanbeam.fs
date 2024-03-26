#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

layout(location = 0) out vec4 FragColor;

uniform vec2 u_resolution;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

vec2 upperLine;
vec2 lowerLine;

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

    //get coords of corners of curr pixel
    //find upperleft corner, between upperLine.left and pixel.upperLeft
    //find upperright corner, between upperLine.right and pixel.upperRight
    //make sure all in same units
    //if upperLine interscts top wall of pixel
        //compute intersection point

    //repeat for lower side of pixel and line

    //using shoelace formula, compute area of region

    //output area

	FragColor = vec4(sourcePos.y,detectorLeftPos.y,detectorRightPos.y,0.5); // vec4(r,g,b,a)
}

float upper_line_at(float x)
{
    return upperLine.x * x + upperLine.y; // y = mx + b
}

float lower_line_at(float x)
{
    return lowerLine.x * x + lowerLine.y; // y = mx + b
}

vec2 get_point_slope_form(vec2 sourcePos, vec2 detectorPos)
{
    float slope = (detectorPos.y - sourcePos.y) / (detectorPos.x - sourcePos.x);    // slope
    float b = (sourcePos.y - slope * sourcePos.x) * float(u_resolution);                   // y intercept in units of pixels

    return vec2(slope, b);
}

// TODO
// import slope and intercept for upper and lower lines
// get corner coords of given pixel
// func to get y given x,slope,intercept
// compute area under upper line
// compute area under lower line

