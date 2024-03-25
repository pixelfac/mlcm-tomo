#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec4 vertexColor; // specify a color output to the fragment shader
out float vertexID;

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
    vertexID = gl_VertexID;
}

vec2 rotate(vec2 coord, float radians) {
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
    vec2 detectorLeftSide = vec2(0.0, detectorDist);  // leftmost range of this detector pixel
    vec2 detectorRightSide = vec2(0.0, detectorDist); // rightmost range of this detector pixel

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

// TODO
// update gl_Position based on the current state of the CTScanner
// using code like GetCurrentSourcePosition() and GetCurrentDetectorPosition() from CTScanner.h
// determine whether vertex is source, dest_upper, dest_lower using gl_VertexID

// uniform sourceDist
// uniform views
// uniform detectorDist
// uniform detectorPanelWidth
// uniform numDetectors
