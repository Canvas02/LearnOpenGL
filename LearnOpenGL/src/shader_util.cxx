#include "shader_util.h"


#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <glad/glad.h>

#include "file.h"

int make_shader(unsigned int shader, const char* src, unsigned int type)
{
	int compile_status;
	const GLint src_len = (GLint)std::strlen(src);

	glShaderSource(shader, 1, &src, &src_len);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE)
	{
		int info_log_len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_len);

		std::vector<char> info_log(info_log_len);
		glGetShaderInfoLog(shader, info_log_len, &info_log_len, &info_log[0]);

		std::string info_log_str(info_log.begin(), info_log.end());

		write_file("info.log", info_log_str.c_str());

		return 0;
	}

	return 1;
}

unsigned int make_program(const char* vert_src, const char* frag_src)
{
	auto program = glCreateProgram();

	auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
	auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	if (!make_shader(vert_shader, vert_src, GL_VERTEX_SHADER))
	{
		fprintf(stderr, "Error compiling vertex shader: %d", vert_shader);
		exit(EXIT_FAILURE);
	}

	if (!make_shader(frag_shader, frag_src, GL_FRAGMENT_SHADER))
	{
		fprintf(stderr, "Error compiling fragment shader: %d", frag_shader);
		exit(EXIT_FAILURE);
	}

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);

	glDetachShader(program, vert_shader);
	glDetachShader(program, frag_shader);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	int link_status; 
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE)
	{
		int info_log_len; 
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_len);

		std::vector<char> info_log(info_log_len);
		glGetProgramInfoLog(program, info_log_len, &info_log_len, &info_log[0]);

		std::string info_log_str(info_log.begin(), info_log.end());

		write_file("info.log", info_log_str.c_str());

		exit(EXIT_FAILURE);
	}

	return program;
}