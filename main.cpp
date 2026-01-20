// GLAD - loads OpenGL function pointers at runtime
#include <glad/glad.h>
// GLFW - API for creating windows, contexts and surfaces, receiving input and events
#include <GLFW/glfw3.h>

// Standard input/output operations
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// window width
constexpr unsigned int SCR_WIDTH = 800;
// window height
constexpr unsigned int SCR_HEIGHT = 600;


// Vertex shader required for rendering graphics
// Done at every vertex
// Vec4 used for the vec.w - perspective division
auto vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader required for rendering graphics
// Done at every pixel
auto fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // Initializes the GLFW library
    glfwInit();

    // Required for MacOS M chip support
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Creates a window and its associated context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    // window null check
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        // Terminates the GLFW library
        glfwTerminate();
        // Return -1, to signify unsuccessful operation / error signaling
        return -1;
    }

    // Makes the context of the specified window current for the calling thread
    glfwMakeContextCurrent(window);
    // Sets the framebuffer resize callback for the specified window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Check if GLAD GL is loaded and the GLFW specific function is found in the graphics driver
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // creates an empty shader object and returns a non-zero value by which it can be referenced
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // replaces the source code in a shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // compiles the source code strings that have been stored in the shader object specified by shader
    glCompileShader(vertexShader);

    int success;
    // character array to hold a shader error message
    char infoLog[512];

    // return the requested parameter from a shader object
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // checking if GL compile for the shader is successful
    if (!success)
    {
        // returns the information log for a shader object
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // creates shader object, in this case a fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // same checks for the fragment shader as the vertex shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // creates an empty program object and returns a non-zero value by which it can be referenced
    unsigned int shaderProgram = glCreateProgram();
    // shaders that will be linked together must first be attached to a program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // linking the shaders to the program
    glLinkProgram(shaderProgram);

    // check if program was successfully linked
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // once the shaders are linked, the memory can be freed, as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Array of vec, in this case a triangle
    float vertices_1[] = {
        0.0f, 0.0f, 0.0f, // left
        0.5f, 0.0f, 0.0f, // right
        0.25f, 0.5f, 0.0f, // top
    };

    float vertices_2[] = {
        0.0f, 0.0f, 0.0f, // left
        -0.5f, 0.0f, 0.0f, // right
        -0.25f, 0.5f, 0.0f // top
    };

    // Vertex Buffer Object & Vertex Array Object
    unsigned int VBO_1, VAO_1, VBO_2, VAO_2;
    // if we wanted, we could declare multiple VBO/VAO like: unsigned int VBOs[2], VAOs[2];
    // generate vertex array
    glGenVertexArrays(1, &VAO_1);
    glGenVertexArrays(1, &VAO_2);
    // if we declared VAOs are arr, we could generate both at same time: glGenVertexArrays(2, VAOs);
    // generate vertex buffer
    glGenBuffers(1, &VBO_1);
    glGenBuffers(1, &VBO_2);
    // same here: glGenBuffers(2, VBOs);

    // binds the first VAO
    glBindVertexArray(VAO_1);

    // binds the first VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
    // static draw - this shape never changes it's vertex values
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);

    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // enable or disable a generic vertex attribute array
    glEnableVertexAttribArray(0);

    // Every VAO needs its own configuration calls to know how to interpret the buffer data
    // Repeating below for the second set of VAO/VBO

    // binds the second VAO
    glBindVertexArray(VAO_2);

    // binds the second VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
    // static draw - this shape never changes it's vertex values
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // enable or disable a generic vertex attribute array
    glEnableVertexAttribArray(0);

    // polygon mode:
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // keep window open while the close is false
    while (!glfwWindowShouldClose(window))
    {
        // custom function to listen for user input to close window
        processInput(window);

        // clears the window’s background to a specific color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activates the specified shader program
        glUseProgram(shaderProgram);
        // binds first VAO
        glBindVertexArray(VAO_1);
        // draw call to draw (count: 3) vertices using the bound vao
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // binds second VAO
        glBindVertexArray(VAO_2);
        // draw call to draw (count: 3) vertices using the bound vao
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // "double buffering"
        glfwSwapBuffers(window);
        // process window events (eg. close window with esc)
        glfwPollEvents();
    }

    // delete vao, vbo, and program
    glDeleteVertexArrays(1, &VAO_1);
    glDeleteBuffers(1, &VBO_1);
    glDeleteVertexArrays(1, &VAO_2);
    glDeleteBuffers(1, &VBO_2);
    glDeleteProgram(shaderProgram);

    // terminate GLFW
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
