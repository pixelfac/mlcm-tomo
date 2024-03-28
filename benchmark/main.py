import subprocess

import matlab.engine

import numpy as np

import visual as vis

SAMPLE_LIST = np.array([
    16,
    32,
    64,
    128,
    256,
    512,
    1024,
    2048,
    4096
])

NUM_DETECTORS = {
    16: 53,
    32: 103,
    64: 201,
    128: 399,
    256: 797,
    512: 1591,
    1024: 3179,
    2048: 6355,
    4096: 12707
}

ITERATIONS_PER_SAMPLE = 10


def setup():
    subprocess.call('setup.bat', shell=False)


def init_engine() -> matlab.engine.MatlabEngine:
    eng = matlab.engine.connect_matlab()
    eng.addpath("../toolkit_astra", nargout=0)
    eng.addpath("../toolkit_astra/algorithms", nargout=0)
    eng.addpath("../toolkit_astra/mex", nargout=0)
    eng.addpath("../toolkit_astra/tools", nargout=0)
    eng.addpath("astra_demo", nargout=0)
    eng.addpath("airtools_demo", nargout=0)
    eng.addpath("../toolkit_airtools", nargout=0)
    eng.addpath("../toolkit_airtools/testprobs", nargout=0)

    return eng


def benchmark(size, distance, detectors, eng) -> np.ndarray[np.float32]:
    samples = np.zeros(4, dtype=np.float32)

    # astra_cpu_time_ms: float = eng.astra_demo(size, distance, detectors) * 1000
    astra_cpu_time_ms = 0
    print(f"Astra CPU: {astra_cpu_time_ms}ms")

    astra_gpu_time_ms = eng.astra_cuda_demo(size, distance, detectors) * 1000
    print(f"Astra GPU: {astra_gpu_time_ms}ms")

    p = subprocess.run(f"..\\benchmark-venv\\Scripts\\activate.bat && python tigre_demo/tigre_demo.py {size} {distance} {detectors}", shell=False, capture_output=True, text=True)
    tigre_gpu_time_ms = p.stdout.strip()[:-2]
    print(f"Tigre GPU: {tigre_gpu_time_ms}ms")

    # airtools_cpu_time_ms = eng.airtools_demo(size) * 1000
    # print(f"Airtools CPU: {airtools_cpu_time_ms}ms")

    samples[0] = astra_cpu_time_ms
    samples[1] = astra_gpu_time_ms
    samples[2] = tigre_gpu_time_ms
    samples[3] = 0

    return samples


if __name__ == "__main__":
    # setup()

    print(f"Starting Matlab Engine...")
    eng: matlab.engine.MatlabEngine = init_engine()

    # Run a benchmark once to init the GPU and get the computer cookin'
    print(f"Running warmup benchmark...")
    benchmark(1024, 4096, NUM_DETECTORS[1024], eng)

    # Begin benchmark sampling
    print(f"Running benchmarks...")
    samples = np.zeros((len(SAMPLE_LIST), ITERATIONS_PER_SAMPLE, 4), dtype=np.float32)
    
    for i, size in enumerate(SAMPLE_LIST):
        print(f"Benchmarking at phantom image size of: {size}x{size}")
        distance = size * 4
        detectors = NUM_DETECTORS[size]

        for j in range(ITERATIONS_PER_SAMPLE):
            result = benchmark(size, distance, detectors, eng)
            samples[i][j] = result


    print(samples)
        
    vis.box(samples, ITERATIONS_PER_SAMPLE)