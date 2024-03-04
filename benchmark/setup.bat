@ECHO OFF
cls

set "venv=benchmark-venv"

ECHO Creating virtual environment if does not exist...
timeout 2 > NUL
cd ..\
IF exist %venv% ( echo %venv% exists ) ELSE ( python -m venv %venv% && echo %venv% created)
ECHO Done
timeout 1 > NUL

ECHO Activating benchmark virtaul environment...
timeout 2 > NUL
call benchmark-venv\Scripts\activate.bat
ECHO Done
timeout 1 > NUL

ECHO INSTALLING matlab engine...
timeout 2 > NUL
pip install matlabengine
ECHO Done
timeout 1 > NUL

ECHO Changing into TIGRE Folder...
timeout 2 > NUL
cd toolkit_tigre\Python
ECHO Done
timeout 1 > NUL

ECHO Compiling/installing tigre...
timeout 2 > NUL
pip install .
ECHO Done
timeout 1 > NUL

ECHO Returning to current directory...
timeout 2 > NUL
cd ..\..\benchmark
ECHO Done
timeout 1 > NUL