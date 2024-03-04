import matlab.engine


def init_engine() -> matlab.engine.MatlabEngine:
    eng = matlab.engine.start_matlab()
    eng.addpath("../toolkit_astra", nargout=0)
    eng.addpath("../toolkit_astra/algorithms", nargout=0)
    eng.addpath("../toolkit_astra/mex", nargout=0)
    eng.addpath("../toolkit_astra/tools", nargout=0)
    eng.addpath("astra_demo", nargout=0)

    return eng


def run_cpu(eng) -> float:
    time_s = eng.astra_demo()
    print(time_s)

    return time_s

def run_gpu(eng) -> float:
    time_s = eng.astra_cuda_demo()
    print(time_s)

    return time_s