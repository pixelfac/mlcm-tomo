%DEMO_ASTRA_2D  Demonstrates interfacing to the ASTRA Toolbox
%
% This script demonstrates how to interface from AIR Tools to other
% software, in the present case to the ASTRA Tomography Toolbox, see 
% http://www.astra-toolbox.com/. Instead of using the matrices or matrix-
% free versions of the test problems in AIR Tools, ASTRA can be used for
% efficient GPU-accelerated application of the matrix and its transpose
% from all AIR Tools reconstruction functions.
%
% This script compares a fanlineartomo case reconstructed by the SART
% algorithm using the matrix and matrix-free modes of AIR Tools,
% interfacing from AIR Tools to ASTRA for application of the matrix and
% its transpose, as well as the built-in "SIRT" algorithm of ASTRA which is
% equivalent to AIR Tools' SART.
%
% See also: afun_astra_2d, demo_sirt.

% Code written by: Per Christian Hansen, Jakob Sauer Jorgensen, and 
% Maria Saxild-Hansen, 2010-2017.

% This file is part of the AIR Tools II package and is distributed under
% the 3-Clause BSD License. A separate license file should be provided as
% part of the package. 
% 
% Copyright 2017 Per Christian Hansen, Technical University of Denmark and
% Jakob Sauer Jorgensen, University of Manchester.
function out = airtools_demo(size)%( size, distance, detectors)
    fprintf(1,'Starting demo_astra_2d:\n\n');

    % Parameters for fanlineartomo test problem
    dtheta = 1;
    theta = 0:dtheta:(360-dtheta);
    p = 1.5*size;
    R = 2;
    dw = 2;
    sd = 3;

    % Test image
    X = phantomgallery('shepplogan',size);
    x = X(:);

    % AIRtools matrix and matrix-free
    fprintf(1,'Creating a fanlineartomo tomography test problem\n');
    fprintf(1,'with N = %2.0f, theta = %1.0f:%1.0f:%3.0f, and p = %2.0f.',...
        [size,theta(1),theta(2)-theta(1),theta(end),p]);
    tic
    A = fanlineartomo(size,theta,p,R,dw,sd);
    out=toc;
    
    Afun = fanlineartomo(size,theta,p,R,dw,sd,[],0);

    % Data
    % out = 0;
end