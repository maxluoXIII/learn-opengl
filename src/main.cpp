#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <format>
#include <iostream>
#include <string>

import Shader;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	// initialize GLFW and set OpenGL version
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// load function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set size of rendering window
	glViewport(0, 0, 800, 600);

	// set callback to set viewport size when window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ShaderProgram *shaderProgram = nullptr;

    try {
        shaderProgram = new ShaderProgram{1.0f, 0.5f, 0.2f, 1.0f};
    } catch (ShaderCompilationException& e) {
        std::string shaderTypeString;
        switch(static_cast<int>(e.getShaderType())) {
            case GL_VERTEX_SHADER:
                shaderTypeString = "VERTEX";
                break;
            case GL_FRAGMENT_SHADER:
                shaderTypeString = "FRAGMENT";
                break;
            default:
                shaderTypeString = "UNKNOWN";
        }

        std::cerr << format("ERROR::SHADER::{}::COMPILATION_FAILED", shaderTypeString) << std::endl;
        std::cerr << e.what() << std::endl;

        glfwTerminate();
        return -1;
    } catch (ShaderLinkingException& e) {
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
        std::cerr << e.what() << std::endl;

        glfwTerminate();
        return -1;
    }
    // Draw two triangles
    // Set up the vertex buffer
    std::array vertices {
        std::array {
             -0.5f, -0.5f, 0.0f, // First triangle
              0.0f, -0.5f, 0.0f,
            -0.25f,  0.5f, 0.0f,
        },
        std::array {
              0.5f, -0.5f, 0.0f, // Second triangle
              0.0f, -0.5f, 0.0f,
             0.25f,  0.5f, 0.0f
        }
    };

    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);
    glBindVertexArray(VAO[0]);

    unsigned int VBO[2];
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[0].data(), GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // set up VAO[1]
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[1]), vertices[1].data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // set wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw
        glUseProgram(shaderProgram->getId());
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, vertices[0].size());

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, vertices[1].size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
