APP_NAME = a.out
C_FILES = ./*.cpp

APP_DEFINES :=
APP_INCLUDES := -I./GL -I./GLFW -I./GLM -framework OpenGL
APP_LINKERS := -L./GLFW/lib -lglfw -L./GL/lib -lglew 

build: 
	clang++ $(C_FILES) -o ./$(APP_NAME) $(APP_INCLUDES) $(APP_LINKERS)