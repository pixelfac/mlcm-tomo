// Based on templates from learnopengl.com
#include "GL/glew.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "importShaders.h"
#include <iostream>
#include <cmath>
#include <chrono>


#define PX_RESOLUTION 128  // # of pixels screen is tall. keep it in powers of 2!

#define VIEWS 16 // # of angles around subject that scans are taken
#define DETECTOR_PIXELS 64 // # of discrete pixels on detector panel

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main()
{
    const char *vertexShaderSource = importShader("fanbeam.vs");
    const char *geometryShaderSource = importShader("fanbeam.gs");
    const char *fragmentShaderSource = importShader("fanbeam.fs");

    const char *frag2ShaderSource = importShader("testdoublefrag.fs");
    const char *vert2ShaderSource = importShader("testdoublevert.vs");

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(PX_RESOLUTION, PX_RESOLUTION, "Display RGB Array", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    // make render target
    // -------------------------
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // The texture we're going to render to
    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, PX_RESOLUTION, PX_RESOLUTION, 0, GL_RGBA, GL_FLOAT, 0);

    // Poor filtering. Needed !
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        return 0;
    }



    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // geometry shader
    unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
    glCompileShader(geometryShader);
    // check for shader compile errors
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

    //load second pass
    // vertex shader
    unsigned int vert2Shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert2Shader, 1, &vert2ShaderSource, NULL);
    glCompileShader(vert2Shader);
    // check for shader compile errors
    glGetShaderiv(vert2Shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vert2Shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int frag2Shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag2Shader, 1, &frag2ShaderSource, NULL);
    glCompileShader(frag2Shader);
    // check for shader compile errors
    glGetShaderiv(frag2Shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(frag2Shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vert2Shader);
    glAttachShader(shaderProgram2, frag2Shader);
    glLinkProgram(shaderProgram2);
    // check for linking errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM2::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vert2Shader);
    glDeleteShader(frag2Shader);



    //init uniform
    int resolution = glGetUniformLocation(shaderProgram, "u_resolution");
    int sourceDist = glGetUniformLocation(shaderProgram, "sourceDist");
    int detectorDist = glGetUniformLocation(shaderProgram, "detectorDist");
    int detectorPanelWidth = glGetUniformLocation(shaderProgram, "detectorPanelWidth");
    int views = glGetUniformLocation(shaderProgram, "views");
    int viewNum = glGetUniformLocation(shaderProgram, "viewNum");
    int currView = 0;
    int numDetectors = glGetUniformLocation(shaderProgram, "numDetectors");
    int detectorNum = glGetUniformLocation(shaderProgram, "detectorNum");
    int currDetectorPixel = 0;

    int renderedTargetID = glGetUniformLocation(shaderProgram2, "rendered_texture");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions       
         1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //collect timings of draw loop
    double totalTime = 0;


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //start timer
        std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

        // render container
        glUseProgram(shaderProgram);
        
        //update uniform variables
        glUniform2f(resolution, PX_RESOLUTION, PX_RESOLUTION);
        glUniform1f(sourceDist, 1.5);
        glUniform1f(detectorDist, 1.5);
        glUniform1f(detectorPanelWidth, 2.0);
        glUniform1i(views, VIEWS);
        glUniform1i(viewNum, currView);
        glUniform1i(numDetectors, DETECTOR_PIXELS);
        glUniform1i(detectorNum, currDetectorPixel);

        //render triangle
        //glBindVertexArray(VAO);
        // Render to our framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0); //0 to write to screen, FramebufferName to write to texture
        int realSreenWidth, realSreenHeight;
        glfwGetFramebufferSize(window, &realSreenWidth, &realSreenHeight); // high DPI displays may have more pixels, so get px count from screen, not program
        glViewport(0, 0, realSreenWidth, realSreenHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //second shader pass
        glUseProgram(shaderProgram2);
        glUniform1i(renderedTargetID, renderedTexture);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); //0 to write to screen, FramebufferName to write to texture
        glDrawArrays(GL_TRIANGLES, 0, 3);


        //stop timer
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        // std::cout << duration.count() << " ms" << std::endl;
        totalTime += duration.count();

        // debugging
        //read pixels
        // float* pixels = new float[PX_RESOLUTION*PX_RESOLUTION];
        // glReadPixels(0, 0, PX_RESOLUTION, PX_RESOLUTION, GL_RED, GL_FLOAT, pixels); //starts bottomleft, reads first left-right, then bottom-top
        // for (int i = 0; i < PX_HEIGHT*PX_WIDTH; i++) {
        //     std::cout << i << '\t';
        //     std::cout << pixels[i] << std::endl;
        // }
        // std::cout << std::endl;
        // delete[] pixels;

        //update CT Scanner progress
        currDetectorPixel++;
        if (currDetectorPixel >= DETECTOR_PIXELS) {
            currDetectorPixel = 0;
            currView++;

            if (currView >= VIEWS) {
                break;
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        // break;
    }

    //print timing data
    std::cout << "Avg Per Draw Call: " << totalTime / (VIEWS * DETECTOR_PIXELS) << " ms" << std::endl;

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}