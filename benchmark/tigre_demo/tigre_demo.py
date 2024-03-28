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
from tigre.utilities import sl3d
from datetime import datetime
import sys

def run(size: int, distance: int, detectors: int) -> float:
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
    geo.DSD = distance # Distance Source Detector      (mm)
    geo.DSO = distance # Distance Source Origin        (mm)
    # Image parameters
    geo.nVoxel = np.array([1, size, size])  # number of voxels              (vx)
    geo.sVoxel = np.array([1, size, size])  # total size of the image       (mm)
    geo.dVoxel = geo.sVoxel / geo.nVoxel  # size of each voxel            (mm)
    # Detector parameters
    geo.nDetector = np.array([1, detectors])  # number of pixels              (px)
    geo.dDetector = np.array([geo.dVoxel[0], 0.8])  # size of each pixel            (mm)
    geo.sDetector = geo.nDetector * geo.dDetector  # total size of the detector    (mm)
    # Offsets
    geo.offOrigin = np.array([0, 0, 0])  # Offset of image from origin   (mm)
    geo.offDetector = np.array([0, 0])  # Offset of Detector            (mm)
    # MAKE SURE THAT THE DETECTOR PIXELS SIZE IN V IS THE SAME AS THE IMAGE!

    geo.mode = "cone"
    #%% Define angles of projection and load phatom image
    angles = np.linspace(0, 2 * np.pi, 100)

    head = np.zeros((1, size, size), dtype=np.float32)
    shepp = sl3d.shepp_logan_3d(np.array([16, size, size]))
    head[0] = shepp[5].astype(np.float32)

    # tigre.plotImg(head, dim="z")

    dt = datetime.now()
    projections = tigre.Ax(head, geo, angles)
    dt2 = datetime.now()
    diff = dt2 - dt
    # print(diff.microseconds / 1000.0)
    # print(diff.microseconds / 1000.0)

    # tigre.plotSinogram(projections, 0)

    #%% recosntruct
    # imgOSSART = algs.ossart(projections, geo, angles, 40)
    # imgCGLS = algs.cgls(projections, geo, angles, 40)

    #%% Plot
    # tigre.plotimg(np.concatenate([head, imgOSSART, imgCGLS], axis=1), dim="z", slice=0, clims=[0, 0.5])

    return diff.microseconds / 1000.0


def main(): 
    n = len(sys.argv)
    if (n != 4):
        print(f"Invalid number of arguments {n}", file=sys.stderr)
        return

    size = int(sys.argv[1])
    distance = int(sys.argv[2])
    detectors = int(sys.argv[3])
   
    run(size, distance, detectors)

if __name__ == "__main__":
    main()
        