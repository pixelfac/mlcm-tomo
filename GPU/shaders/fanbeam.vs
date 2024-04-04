#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

uniform float sourceDist;
uniform float detectorDist;
uniform float detectorPanelWidth;

uniform int views;
uniform int viewNum;
uniform int numDetectors;
uniform int detectorNum;

vec2 rotate(vec2 coord, float radians);
vec2 GetCurrentSourcePosition(int viewNum);
vec4 GetCurrentDetectorPosition(int viewNum, int detectorNum);

void main()
{
    if (gl_VertexID == 0) {
        gl_Position = vec4(GetCurrentSourcePosition(viewNum), 0.0, 1.0);
    }
    else if (gl_VertexID == 1) {
        gl_Position = vec4(GetCurrentDetectorPosition(viewNum, detectorNum).xy, 0.0, 1.0);
    }
    else if (gl_VertexID == 2) {
        gl_Position = vec4(GetCurrentDetectorPosition(viewNum, detectorNum).zw, 0.0, 1.0);
    }
    else {
        gl_Position = vec4(aPos, 1.0); // default value
    }
}

vec2 rotate(vec2 coord, float radians)
{
    float sinTheta = sin(radians);
    float cosTheta = cos(radians);
    return vec2(coord.x * cosTheta - coord.y * sinTheta, coord.x * sinTheta + coord.y * cosTheta);
}

vec2 GetCurrentSourcePosition(int viewNum)
{
    vec2 initialPosition = vec2(0.0, sourceDist);
    // if viewNum == 0, then source has not moved from initial position, (0, sourceDist)
    float radiansRotated = viewNum * (6.28318530718 / views); //6.28318530718 = 2PI

    return rotate(initialPosition, radiansRotated);
}

vec4 GetCurrentDetectorPosition(int viewNum, int detectorNum)
{
    vec2 detectorLeftSide = vec2(0.0, -detectorDist);  // leftmost range of this detector pixel
    vec2 detectorRightSide = vec2(0.0, -detectorDist); // rightmost range of this detector pixel

    float detectorWidth = detectorPanelWidth / numDetectors; // width of each individual detector pixel
    detectorLeftSide.x = (detectorWidth * detectorNum) - (detectorPanelWidth / 2);
    detectorRightSide.x = detectorLeftSide.x + detectorWidth;

    // if viewNum == 0, then source has not moved from initial position, (0, detectorDist)
    float radiansRotated = viewNum * (6.28318530718 / views);

    // TODO how to make rotation matrix to rotate a vector by a set number of degrees/radians?
    detectorLeftSide = rotate(detectorLeftSide, radiansRotated);
    detectorRightSide = rotate(detectorRightSide, radiansRotated);

    return vec4(detectorLeftSide, detectorRightSide);
}