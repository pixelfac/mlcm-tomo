n = 4096; % is the resolution of the image
r = n * 4; % the distance from the detector to the origin
os = n * 4; % the distance from the origin to the source
thetas = linspace(0, 2*pi, 100); 
delta = 1;

maxProjLen = (ceil( getMaxProjection(r, os, thetas, n/2) ));
samplePt   = [-fliplr( 0 : delta : maxProjLen/2 + delta), delta : delta : maxProjLen/2 + delta];
ns = length(samplePt);