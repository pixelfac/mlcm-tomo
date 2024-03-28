% -----------------------------------------------------------------------
% This file is part of the ASTRA Toolbox
% 
% Copyright: 2010-2022, imec Vision Lab, University of Antwerp
%            2014-2022, CWI, Amsterdam
% License: Open Source under GPLv3
% Contact: astra@astra-toolbox.com
% Website: http://www.astra-toolbox.com/
% -----------------------------------------------------------------------

% Create a basic 256x256 square volume geometry
vol_geom = astra_create_vol_geom(256, 256);

% Create a parallel beam geometry with 180 angles between 0 and pi, and
% 384 detector pixels of width 1.
% For more details on available geometries, see the online help of the
% function astra_create_proj_geom .
proj_geom = astra_create_proj_geom('parallel', 1.0, 384, linspace2(0,pi,180));

% Create a 256x256 phantom image using matlab's built-in phantom() function
P = phantom(256);

% Create a sinogram using the GPU.
% Note that the first time the GPU is accessed, there may be a delay
% of up to 10 seconds for initialization.
%tic;
func_handle = @()astra_create_sino_gpu(P, proj_geom, vol_geom);
exec_time = timeit(func_handle);
disp(['Elapsed time: ', num2str(exec_time), ' seconds']);
[sinogram_id, sinogram] = astra_create_sino_gpu(P, proj_geom, vol_geom);
%time = toc;

%disp(['Elapsed time: ', num2str(time), ' seconds']);

figure(1); imshow(P, []);
figure(2); imshow(sinogram, []);


% Free memory
astra_mex_data2d('delete', sinogram_id);