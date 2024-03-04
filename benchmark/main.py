import subprocess

import astra_demo.astra_wrapper as astra
import tigre_demo.tigre_wrapper as tigre

import matlab.engine


def setup():
    subprocess.call('setup.bat', shell=False)



if __name__ == "__main__":

    dist_to_detector = 0
    dist_to_source = 0
    num_angles_slash_views = 0
    angle_range = 0
    image_resolution = 0
    num_detectors = 0

    setup()

    eng = astra.init_engine()

    astra.run_cpu(eng)

    astra.run_gpu(eng)

    tigre.run()