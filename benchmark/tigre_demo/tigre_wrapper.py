## DEMO 16:  TIGRE 2D tomography
#
#
#  In demo we show how to do 2D tomography in TIGRE. It is exactly the same
#  as 3D.
#
#
#
# --------------------------------------------------------------------------
# --------------------------------------------------------------------------
# This file is part of the TIGRE Toolbox
#
# Copyright (c) 2015, University of Bath and
#                     CERN-European Organization for Nuclear Research
#                     All rights reserved.
#
# License:            Open Source under BSD.
#                     See the full license at
#                     https://github.com/CERN/TIGRE/blob/master/LICENSE
#
# Contact:            tigre.toolbox@gmail.com
# Codes:              https://github.com/CERN/TIGRE/
# Coded by:           Ander Biguri
# --------------------------------------------------------------------------
#%%Initialize
import tigre
import numpy as np
from tigre.utilities import sample_loader
from tigre.utilities import CTnoise
import tigre.algorithms as algs

def run() -> float:
    SIZE = 512

    #%% 2D Parallel beam
    # VARIABLE                                   DESCRIPTION                    UNITS

    # HERE COMES THE TRICK.
    #
    # 1- Make the detector Nx1 (instead of 1)
    # 2- Make the image NxMx1  (isntead of 1)

    geo = tigre.geometry()
    # VARIABLE                                   DESCRIPTION                    UNITS
    # -------------------------------------------------------------------------------------
    # Distances
    geo.DSD = SIZE * 4  # Distance Source Detector      (mm)
    geo.DSO = SIZE * 4  # Distance Source Origin        (mm)
    # Image parameters
    geo.nVoxel = np.array([1, SIZE, SIZE])  # number of voxels              (vx)
    geo.sVoxel = np.array([1, SIZE, SIZE])  # total size of the image       (mm)
    geo.dVoxel = geo.sVoxel / geo.nVoxel  # size of each voxel            (mm)
    # Detector parameters
    geo.nDetector = np.array([1, SIZE * 2])  # number of pixels              (px)
    geo.dDetector = np.array([geo.dVoxel[0], 0.8])  # size of each pixel            (mm)
    geo.sDetector = geo.nDetector * geo.dDetector  # total size of the detector    (mm)
    # Offsets
    geo.offOrigin = np.array([0, 0, 0])  # Offset of image from origin   (mm)
    geo.offDetector = np.array([0, 0])  # Offset of Detector            (mm)
    # MAKE SURE THAT THE DETECTOR PIXELS SIZE IN V IS THE SAME AS THE IMAGE!

    geo.mode = "cone" # THIS NEEDS TO BE CHANGED BACK TO PARALLEL

    #%% Define angles of projection and load phatom image
    angles = np.linspace(0, 2 * np.pi, 100)

    head = sample_loader.load_head_phantom(geo.nVoxel)
    # head = np.array([1, 256, 256])

    from tigre.utilities import sl3d
    shepp = sl3d.shepp_logan_3d(np.array([16, SIZE, SIZE]))
    head[0] = shepp[5]

    # tigre.plotImg(head, dim="z")

    from datetime import datetime
    dt = datetime.now()
    print(dt)
    projections = tigre.Ax(head, geo, angles)
    dt2 = datetime.now()
    diff = dt2 - dt
    print(diff)
    print(diff.microseconds / 1000.0)

    # tigre.plotSinogram(projections, 0)

    #%% recosntruct
    # imgOSSART = algs.ossart(projections, geo, angles, 40)
    # imgCGLS = algs.cgls(projections, geo, angles, 40)

    #%% Plot
    # tigre.plotimg(np.concatenate([head, imgOSSART, imgCGLS], axis=1), dim="z", slice=0, clims=[0, 0.5])

    return diff.microseconds / 1000.0
