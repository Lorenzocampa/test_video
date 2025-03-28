#include <../inc/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "shader.h"
#include "Texture.h"

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

	GLFWwindow* window = glfwCreateWindow(640, 480, "LETSGOSKI", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	// Define vertices for a single triangle (3 vertices, each with 2 components)
	float positions[] = {
		-0.5F, -0.5F, 0.0F, 0.0F, // Vertex 1
		0.5F,  -0.5F, 1.0F, 0.0F, // Vertex 2
		0.5F,  0.5F,  1.0F, 1.0F, // Vertex 3

		-0.5F, 0.5F,  0.0F, 1.0F // vertex 5
	};

	unsigned int indices[] = {0, 1, 2, 2, 3, 0};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VertexArray va;
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	Shader shader("res/shaders/basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8F, 0.3F, 0.8F, 1.0F);

	Texture texture("res/textures/sfondo harry lo snitcher.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	va.Unbind();
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();

	Renderer renderer;

	float r			= 0.0F;
	float increment = 0.05F;
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();
		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3F, 0.8F, 1.0F);

		renderer.Draw(va, ib, shader);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (r > 1.0F)
		{
			increment = -0.05F;
		}
		else if (r < 0.0F)
		{
			increment = 0.05F;
		}

		r += increment;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
