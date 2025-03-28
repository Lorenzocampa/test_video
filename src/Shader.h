#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

struct shader_program_src
{
	std::string vertex_src;
	std::string fragment_src;
};
class Shader
{
  private:
	std::string m_file_path;
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniform_location_cache;
	//*caching for uniforms
  public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//*set uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

  private:
	shader_program_src parse_shader(const std::string& filepath);
	unsigned int compile_shader(unsigned int type, const std::string& source);
	unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	int GetUniformLocation(const std::string& name);
};