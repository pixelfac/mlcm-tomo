r = 256 * 4; % the distance from the detector to the origin
os = 256 * 4; % the distance from the origin to the source
thetas = linspace(0, 2*pi, 100); 
n = 256; % is the resolution of the image
delta = 0.8;

maxProjLen = (ceil( getMaxProjection(r, os, thetas, n/2) ));
samplePt   = [-fliplr( 0 : delta : maxProjLen/2 + delta), delta : delta : maxProjLen/2 + delta];
ns = length(samplePt);