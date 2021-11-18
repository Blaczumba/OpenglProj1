#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>


Shader::Shader(const std::string& filepath) :m_FilePath(filepath), m_RendererID(0){
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}
Shader::~Shader() {
	glDeleteProgram(m_RendererID);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType :int {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else ss[static_cast<int>(type)] << line << '\n';
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = &source[0];
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* infolog = new char[length];
		glGetShaderInfoLog(id, length, &length, infolog);
		std::cout << "Compiling shader (" << (type == GL_VERTEX_SHADER ? "VERTEX" : "SHADER") << ") error: " << infolog << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* infoLog = new char[length];
		glGetProgramInfoLog(program, length, &length, infoLog);
		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(program);
		std::cout << "Linking program error: " << infoLog << std::endl;
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Bind() const {
	glUseProgram(m_RendererID);
}

void Shader::UnBind() const {
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value) {
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value) {
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}