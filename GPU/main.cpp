// Based on templates from learnopengl.com
#include "GL/glew.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "importShaders.h"
#include <iostream>
#include <cmath>
#include <chrono>

#define PX_RESOLUTION 512 // # of pixels for each screen dimension. keep it in powers of 2!

#define VIEWS 16           // # of angles around subject that scans are taken
#define DETECTOR_PIXELS 64 // # of discrete pixels on detector panel

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
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
    GLFWwindow *window = glfwCreateWindow(PX_RESOLUTION, PX_RESOLUTION, "Display RGB Array", NULL, NULL);
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
    // code in this section from this page: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    // -------------------------
    // The framebuffer, which allows us to write shader output to somewhere that's not the screen.
    GLuint FramebufferName;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // The texture we're going to render to
    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last NULL means empty image )
    // we're going to overwrite the image with our shader output
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PX_RESOLUTION, PX_RESOLUTION, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // Use the nearest pixel to UV coords when drawing texture, if it is stretched or squished
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set "renderedTexture" as colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the draw buffer to draw to ColorAttachment0
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // Check that framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer Initialization not correct" << std::endl;
        return -1;
    }

    // build and compile the shaders
    // ------------------------------------
    // read in shader files
    const char *fanbeamVSSource = importShader("fanbeam.vs");
    const char *fanbeamGSSource = importShader("fanbeam.gs");
    const char *fanbeamFSSource = importShader("fanbeam.fs");

    const char *dotproductVSSource = importShader("dotproduct.vs");
    const char *dotproductFSSource = importShader("dotproduct.fs");

    // vertex shader
    unsigned int fanbeamVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fanbeamVS, 1, &fanbeamVSSource, NULL);
    glCompileShader(fanbeamVS);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(fanbeamVS, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fanbeamVS, 512, NULL, infoLog);
        std::cout << "ERROR::FANBEAM_SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // geometry shader
    unsigned int fanbeamGS = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(fanbeamGS, 1, &fanbeamGSSource, NULL);
    glCompileShader(fanbeamGS);
    // check for shader compile errors
    glGetShaderiv(fanbeamGS, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fanbeamGS, 512, NULL, infoLog);
        std::cout << "ERROR::FANBEAM_SHADER::GEOMETRY::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fanbeamFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fanbeamFS, 1, &fanbeamFSSource, NULL);
    glCompileShader(fanbeamFS);
    // check for shader compile errors
    glGetShaderiv(fanbeamFS, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fanbeamFS, 512, NULL, infoLog);
        std::cout << "ERROR::FANBEAM_SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // link shaders
    unsigned int fanbeamShaderProgram = glCreateProgram();
    glAttachShader(fanbeamShaderProgram, fanbeamVS);
    glAttachShader(fanbeamShaderProgram, fanbeamGS);
    glAttachShader(fanbeamShaderProgram, fanbeamFS);
    glLinkProgram(fanbeamShaderProgram);
    // check for linking errors
    glGetProgramiv(fanbeamShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(fanbeamShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::FANBEAM_SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(fanbeamVS);
    glDeleteShader(fanbeamGS);
    glDeleteShader(fanbeamFS);

    // load second pass to compute dot product
    //  vertex shader
    unsigned int dotproductVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(dotproductVS, 1, &dotproductVSSource, NULL);
    glCompileShader(dotproductVS);
    // check for shader compile errors
    glGetShaderiv(dotproductVS, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(dotproductVS, 512, NULL, infoLog);
        std::cout << "ERROR::DOT_PRODUCT_SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    unsigned int dotproductFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(dotproductFS, 1, &dotproductFSSource, NULL);
    glCompileShader(dotproductFS);
    // check for shader compile errors
    glGetShaderiv(dotproductFS, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(dotproductFS, 512, NULL, infoLog);
        std::cout << "ERROR::DOT_PRODUCT_SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // link shaders
    unsigned int dotproductShaderProgram = glCreateProgram();
    glAttachShader(dotproductShaderProgram, dotproductVS);
    glAttachShader(dotproductShaderProgram, dotproductFS);
    glLinkProgram(dotproductShaderProgram);
    // check for linking errors
    glGetProgramiv(dotproductShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(dotproductShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::DOT_PRODUCT_SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(dotproductVS);
    glDeleteShader(dotproductFS);

    // initialize uniform variables
    // ---------------------------
    // fanbeam shader
    int resolution = glGetUniformLocation(fanbeamShaderProgram, "u_resolution");
    int sourceDist = glGetUniformLocation(fanbeamShaderProgram, "sourceDist");
    int detectorDist = glGetUniformLocation(fanbeamShaderProgram, "detectorDist");
    int detectorPanelWidth = glGetUniformLocation(fanbeamShaderProgram, "detectorPanelWidth");
    int views = glGetUniformLocation(fanbeamShaderProgram, "views");
    int viewNum = glGetUniformLocation(fanbeamShaderProgram, "viewNum");
    int currView = 0;
    int numDetectors = glGetUniformLocation(fanbeamShaderProgram, "numDetectors");

    // dot product shader
    int dotresolution = glGetUniformLocation(dotproductShaderProgram, "u_resolution");
    int dotsourceDist = glGetUniformLocation(dotproductShaderProgram, "sourceDist");
    int dotdetectorDist = glGetUniformLocation(dotproductShaderProgram, "detectorDist");
    int dotdetectorPanelWidth = glGetUniformLocation(dotproductShaderProgram, "detectorPanelWidth");
    int dotviews = glGetUniformLocation(dotproductShaderProgram, "views");
    int dotviewNum = glGetUniformLocation(dotproductShaderProgram, "viewNum");
    int dotnumDetectors = glGetUniformLocation(dotproductShaderProgram, "numDetectors");
    int renderedTargetID = glGetUniformLocation(dotproductShaderProgram, "rendered_texture");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[9*DETECTOR_PIXELS];

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute to buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // collect timings of draw loop
    double totalTime = 0;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // start benchmarking timer
        std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

        // set active shader
        glUseProgram(fanbeamShaderProgram);

        // update uniform variables
        glUniform2f(resolution, PX_RESOLUTION, PX_RESOLUTION);
        glUniform1f(sourceDist, 1.5);
        glUniform1f(detectorDist, 1.5);
        glUniform1f(detectorPanelWidth, 2.0);
        glUniform1i(views, VIEWS);
        glUniform1i(viewNum, currView);
        glUniform1i(numDetectors, DETECTOR_PIXELS);

        // Render to our framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName); // 0 to write to screen, FramebufferName to write to whatever is in the framebuffer
        glViewport(0, 0, PX_RESOLUTION, PX_RESOLUTION);
        glClear(GL_COLOR_BUFFER_BIT); // clear what was originally drawn to the texture

        glDrawArrays(GL_TRIANGLES, 0, 3); // draw a triangle w/ 3 vertices to screen

        // second shader pass, performing the dot product
        glUseProgram(dotproductShaderProgram);

        // uniforms
        glUniform1f(dotresolution, PX_RESOLUTION);
        glUniform1f(dotsourceDist, 1.5);
        glUniform1f(dotdetectorDist, 1.5);
        glUniform1f(dotdetectorPanelWidth, 2.0);
        glUniform1i(dotviews, VIEWS);
        glUniform1i(dotviewNum, currView);
        glUniform1i(dotnumDetectors, DETECTOR_PIXELS);

        // set texture used by shader to the one we just drew to in the last draw call
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glUniform1i(renderedTargetID, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // 0 to write to screen, FramebufferName to write to whatever is in the framebuffer

        int realSreenWidth, realSreenHeight;
        glfwGetFramebufferSize(window, &realSreenWidth, &realSreenHeight); // high DPI displays may have more pixels, so get px count from screen, not program
        glViewport(0, 0, realSreenWidth, realSreenHeight);                 // Render on the whole framebuffer, complete from the lower left corner to the upper right

        glDrawArrays(GL_TRIANGLES, 0, 3); // draw a triangle w/ 3 vertices to screen

        // glfw: swap buffers
        glfwSwapBuffers(window);

        // stop benchmarking timer
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        totalTime += duration.count();

        // update Scanning progress
        currView++;
        if (currView >= VIEWS)
        {
            break;
        }

        // poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents(); // needed to see screen for some reason
    }

    // print timing data
    std::cout << "Total Time: " << totalTime << " ms" << std::endl;
    std::cout << "Avg Per Primitive: " << totalTime / (VIEWS * DETECTOR_PIXELS) << " ms" << std::endl;

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteTextures(1, &renderedTexture);
    glDeleteFramebuffers(1, &FramebufferName);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}