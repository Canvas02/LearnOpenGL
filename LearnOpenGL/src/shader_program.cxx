#include "shader_program.h"

#include <vector>
#include <stdexcept>
#include <glad/glad.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include "file.h"
#include "gl_error.h"


// ShaderProgram members return codes
//  1: Successful
//
// -1: Uniform not found
// -2: Max data size exceded
//
// -11: Error compiling shader
// -12: Error linking program

ShaderProgram::~ShaderProgram() noexcept { glDeleteProgram(id); }

ShaderProgram::ShaderProgram(const char* vert_src, const char* frag_src)
{
    id = glCreateProgram();

    uint32_t vert_shader;
    uint32_t frag_shader;

    try {
        vert_shader = make_shader(vert_src, GL_VERTEX_SHADER);
    }
    catch (gl_error&){
        throw gl_error(fmt::format("Error compiling vertex shader for program {}", id));
    }

    try {
        frag_shader = make_shader(frag_src, GL_FRAGMENT_SHADER);
    }
    catch (gl_error&){
        throw gl_error(fmt::format("Error compiling fragment shader for program {}", id));
    }

    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);
    glLinkProgram(id);

    glDetachShader(id, vert_shader);
    glDetachShader(id, frag_shader);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    int link_status;
    glGetProgramiv(id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        int info_log_len;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_len);

        std::vector<char> info_log(info_log_len);
        glGetProgramInfoLog(id, info_log_len, &info_log_len, &info_log[0]);

        std::string info_log_str(info_log.begin(), info_log.end());

        write_file("info.log", info_log_str.c_str());

        throw gl_error(fmt::format("Error linking program {}", id));
    }
}

void ShaderProgram::use() noexcept { glUseProgram(id); }

// throw gl_error(fmt::format("Uniform {} not found", name));

void ShaderProgram::setUniform(const char* name, std::vector<float_t> value)
{
    if(value.empty())
    {
        throw std::invalid_argument("Not enough elements in vertex");
    }
    else if (value.size() > 4)
    {
        throw std::invalid_argument("too many elements, max is 4");
    }

    if(!m_uniforms.contains(name))
    {
        auto uniform = glGetUniformLocation(id, name);
        if (uniform == -1)
        {
            throw gl_error(fmt::format("Uniform {} not found", name));
        }
        m_uniforms.insert({name, uniform});
    }

    switch (value.size())
    {
    case 1:
        glUniform1f(m_uniforms.at(name), value[0]);
        break;
    case 2:
        glUniform2f(m_uniforms.at(name), value[0], value[1]);
        break;
    case 3:
        glUniform3f(m_uniforms.at(name), value[0], value[1], value[2]);
        break;
    case 4:
        glUniform4f(m_uniforms.at(name), value[0], value[1], value[2], value[3]);
        break;
    }
}

void ShaderProgram::setUniform(const char* name, std::vector<int32_t> value)
{
    if(value.empty())
    {
        throw std::invalid_argument("Not enough elements in vector");
    }
    else if(value.size() > 4)
    {
        throw std::invalid_argument("too many elements, max is 4");
    }

    if(!m_uniforms.contains(name))
    {
        auto uniform = glGetUniformLocation(id, name);
        if (uniform == -1)
        {
            throw gl_error(fmt::format("Uniform {} not found", name));
        }
        m_uniforms.insert({name, uniform});
    }

    switch (value.size())
    {
    case 1:
        glUniform1i(m_uniforms.at(name), value[0]);
        break;
    case 2:
        glUniform2i(m_uniforms.at(name), value[0], value[1]);
        break;
    case 3:
        glUniform3i(m_uniforms.at(name), value[0], value[1], value[2]);
        break;
    case 4:
        glUniform4i(m_uniforms.at(name), value[0], value[1], value[2], value[3]);
        break;
    }
}

auto make_shader(const char* src, uint32_t type) -> uint32_t
{
    int compile_status;

    auto shader = glCreateShader(type);

    glShaderSource(shader, 1, &src, nullptr);
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
        throw gl_error("Failed to compile shader");
    }

    return shader;
}
