#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <format>
#include <iostream>
#include <string>

import Exception;

const char* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

unsigned int createAndCompileShader(const char* shaderSource, unsigned int shaderType) {
    unsigned int shader = glCreateShader(shaderType);

    // Compile the shader
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check whether compilation was successful
    int success;
    constexpr size_t LOG_LENGTH = 512;
    char infoLog[LOG_LENGTH];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, LOG_LENGTH, NULL, infoLog);
        throw ShaderCompilationException{infoLog, shaderType};
    }

    return shader;
}

bool createShaderProgram(unsigned int &shaderProgram) {
    // Set up shaders

    // Compile the vertex shader
    unsigned int vertexShader = createAndCompileShader(vertexShaderSource, GL_VERTEX_SHADER);

    // Compile the fragment shader
    unsigned int fragmentShader = createAndCompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    // Link the compiled shaders into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check whether linking was successful
    int success;
    constexpr size_t LOG_LENGTH = 512;
    char infoLog[LOG_LENGTH];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, LOG_LENGTH, NULL, infoLog);
        throw ShaderLinkingException{infoLog};
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
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

    unsigned int shaderProgram;
    try {
        createShaderProgram(shaderProgram);
    } catch (ShaderCompilationException& e) {
        std::string shaderTypeString;
        switch(e.getShaderType()) {
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
    float vertices[] = {
         -0.5f, -0.5f, 0.0f, // First triangle
          0.0f, -0.5f, 0.0f,
        -0.25f,  0.5f, 0.0f,
    };

    float vertices2[] = {
          0.5f, -0.5f, 0.0f, // Second triangle
          0.0f, -0.5f, 0.0f,
         0.25f,  0.5f, 0.0f
    };

    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);
    glBindVertexArray(VAO[0]);

    unsigned int VBO[2];
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // set up VAO[1]
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
