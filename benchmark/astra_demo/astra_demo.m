function out = astra_demo(size, distance, detectors)
    %store various parameters
    detector_spacing = 1.0; % distance between the centers of two adjacent detector pixels
    detector_count = detectors; % number of detector pixels in a single projection
    angles = linspace(0, 2*pi, 100); % projection angles in radians
    source_origin = distance; % distance between the source and the center of rotation
    origin_detector = distance; % distance between the centter of rotation and the detector array

    %resolutions for our final image
    resoultion_x = size;
    resolution_y = size;


    %% create geometries and projector
    proj_geom = astra_create_proj_geom('fanflat', detector_spacing, detector_count, angles, source_origin, origin_detector);
    vol_geom = astra_create_vol_geom(resoultion_x,resolution_y);
    proj_id = astra_create_projector('strip_fanflat', proj_geom, vol_geom);
    %this can also be line fanflat. Strip takes the weight to be the area of
    %intersection of the pixel and ray. Line takes the weight to be the length
    %of the insersection of the pixel and ray.

    %% store volume
    % change the second number if you want to change the number of rows and
    % columns in the shepp logan image
    V = phantom('Shepp-Logan', size);
    volume_id = astra_mex_data2d('create', '-vol', vol_geom, V);

    %% create forward projection
    sinogram_id = astra_mex_data2d('create', '-sino', proj_geom, 0);
    cfg = astra_struct('FP');
    cfg.ProjectorId = proj_id;
    cfg.ProjectionDataId = sinogram_id;
    cfg.VolumeDataId = volume_id;
    fp_id = astra_mex_algorithm('create', cfg);
    tic
    astra_mex_algorithm('run', fp_id);
    elapsed_time=toc;
    % sinogram = astra_mex_data2d('get', sinogram_id);
    %% imshow(sinogram, [])

    %% garbage disposal
    astra_mex_data2d('delete', sinogram_id, volume_id);
    astra_mex_projector('delete', proj_id);
    astra_mex_algorithm('delete', fp_id);

    % disp(['Elapsed time: ', num2str(elapsed_time), ' seconds']);

    out = elapsed_time;
end