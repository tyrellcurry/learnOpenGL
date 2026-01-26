// GLAD - loads OpenGL function pointers at runtime 
#include <glad/glad.h>
// GLFW - API for creating windows, contexts and surfaces, receiving input and events
#include <GLFW/glfw3.h>

#include "shader/shader.h"

// Standard input/output operations
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// window width
constexpr unsigned int SCR_WIDTH = 800;
// window height
constexpr unsigned int SCR_HEIGHT = 600;

int main()
{
    // Initializes the GLFW library
    glfwInit();

    // Required for macOS M chip support
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Creates a window and its associated context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);

    // window null check
    if (window == nullptr)
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

    // Check if GLAD GL is loaded and the GLFW-specific function is found in the graphics driver
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

    Shader ourShader("shader/vertex_shader.glsl", "shader/fragment_shader.glsl");

    // Array of vec, in this case a triangle
    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // polygon mode:
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // keep the window open while the close is false
    while (!glfwWindowShouldClose(window))
    {
        // custom function to listen for user input to close a window
        processInput(window);

        // clears the window’s background to a specific color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activates the specified shader program
        ourShader.use();
        // update the uniform color
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // "double buffering"
        glfwSwapBuffers(window);
        // process window events (e.g., close a window with esc)
        glfwPollEvents();
    }

    // delete vao, vbo, and program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
