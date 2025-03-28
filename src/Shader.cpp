#include <../inc/glad/glad.h>
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath) : m_file_path(filepath), m_rendererID(0)
{
	shader_program_src source = parse_shader(filepath);
	m_rendererID			  = create_shader(source.vertex_src, source.fragment_src);
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
}

shader_program_src Shader::parse_shader(const std::string& filepath)
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

unsigned int Shader::compile_shader(unsigned int type, const std::string& source)
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

unsigned int Shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
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

void Shader::Bind() const
{
	glUseProgram(m_rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string& name)
{

	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
	{
		return m_uniform_location_cache[name];
	}
	int location = glGetUniformLocation(m_rendererID, name.c_str());
	if (location == -1)
	{
		printf("Warning: uniform %s doesn't exist\n", name.c_str());
	}

	m_uniform_location_cache[name] = location;
	return location;
}