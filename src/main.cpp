#include <../inc/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Structure to hold vertex and fragment shader source strings.
struct shader_program_src
{
	std::string vertex_src;
	std::string fragment_src;
};

// Improved shader parser that looks for "#shader vertex" and "#shader fragment".
static shader_program_src parse_shader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	if (!stream.is_open())
	{
		std::cerr << "Could not open shader file: " << filepath << std::endl;
		return {};
	}

	enum class shader_type
	{
		NONE	 = -1,
		VERTEX	 = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2]; // index 0 for vertex, 1 for fragment.
	shader_type type = shader_type::NONE;

	while (getline(stream, line))
	{
		// Check for shader type declaration.
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = shader_type::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = shader_type::FRAGMENT;
			}
		}
		else
		{
			// Only add lines if a valid shader type has been set.
			if (type != shader_type::NONE)
			{
				ss[(int)type] << line << "\n";
			}
		}
	}
	return {ss[0].str(), ss[1].str()};
}

// Helper function to compile a shader and print compile errors if any.
static unsigned int compile_shader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		printf("Failed to compile %s shader:\n%s\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
		glDeleteShader(id);
		return 0;
	}
	return id;
}

// Helper function to create a shader program.
static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs		 = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs		 = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// Bind the "position" attribute to location 0.
	glBindAttribLocation(program, 0, "position");

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "GLFW Initialization failed!" << std::endl;
		return -1;
	}

	// Request an OpenGL 2.1 context (compatibility profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	// Define vertices for a single triangle (3 vertices, each with 2 components)
	float positions[] = {
		-0.5f, -0.5f, // Vertex 1
		0.5f,  -0.5f, // Vertex 2
		0.5f,  0.5f,  // Vertex 3

		-0.5f, 0.5f // vertex 5
	};

	unsigned int indices[] = {0, 1, 2, 2, 3, 0};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	// Set up the vertex attribute pointer for attribute 0 ("position")
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Parse the shader file (should contain both vertex and fragment shader source separated by "#shader" markers)
	shader_program_src source = parse_shader("res/shaders/basic.shader");
	// For debugging, print the shader sources
	printf("VERTEX SHADER SOURCE:\n%s\n", source.vertex_src.c_str());
	printf("FRAGMENT SHADER SOURCE:\n%s\n", source.fragment_src.c_str());

	// Create and use the shader program from the parsed shader sources.
	unsigned int shader_program = create_shader(source.vertex_src, source.fragment_src);
	glUseProgram(shader_program);

	// Unbind the buffer (optional)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.125f, 0.125f, 0.125f, 1.0f); // Dark gray color
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader program and bind the vertex buffer state
		glUseProgram(shader_program);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(0);

		// Draw the triangle (3 vertices)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up
	glDeleteProgram(shader_program);
	glDeleteBuffers(1, &buffer);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
