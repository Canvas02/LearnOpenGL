#include "shader_program.h"

#include <vector>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "file.h"

static int32_t make_shader(uint32_t shader, const char* src, uint32_t type);

// ShaderProgram members return codes
//  1: Successful
//
// -1: Uniform not found
// -2: Max data size exceded
//
// -11: Error compiling shader
// -12: Error linking program

ShaderProgram::~ShaderProgram() { glDeleteProgram(m_id); }

ShaderProgram::ShaderProgram(const char* vert_src, const char* frag_src)
{
    m_id = glCreateProgram();

    auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
    auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if (!make_shader(vert_shader, vert_src, GL_VERTEX_SHADER))
    {
        spdlog::error("Error compiling vertex shader: {}", vert_shader);
        exit(EXIT_FAILURE);
    }

    if (!make_shader(frag_shader, frag_src, GL_FRAGMENT_SHADER))
    {
        spdlog::error("Error compiling fragment shader: {}", frag_shader);
        exit(EXIT_FAILURE);
    }

    glAttachShader(m_id, vert_shader);
    glAttachShader(m_id, frag_shader);
    glLinkProgram(m_id);

    glDetachShader(m_id, vert_shader);
    glDetachShader(m_id, frag_shader);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    int link_status;
    glGetProgramiv(m_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        int info_log_len;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &info_log_len);

        std::vector<char> info_log(info_log_len);
        glGetProgramInfoLog(m_id, info_log_len, &info_log_len, &info_log[0]);

        std::string info_log_str(info_log.begin(), info_log.end());

        write_file("info.log", info_log_str.c_str());

        spdlog::error("Error linking program {}", m_id);
        exit(EXIT_FAILURE);
    }
}

void ShaderProgram::use() { glUseProgram(m_id); }

int32_t ShaderProgram::setUniform(const char* name, bool value)
{
    if(!m_uniforms.contains(name))
    {
        auto uniform = glGetUniformLocation(m_id, name);
        if (uniform == -1)
        {
            spdlog::error("Uniform {} not found", name);
            return -1;
        }
        m_uniforms.insert({name, uniform});
    }

    glUniform1i(m_uniforms.at(name), (int)value);
    return 1;
}

int32_t ShaderProgram::setUniform(const char* name, float_t value)
{
    if(!m_uniforms.contains(name))
    {
        auto uniform = glGetUniformLocation(m_id, name);
        if (uniform == -1)
        {
            spdlog::error("Uniform {} not found", name);
            return -1;
        }
        m_uniforms.insert({name, uniform});
    }

    glUniform1f(m_uniforms.at(name), value);
    return 1;
}

int32_t ShaderProgram::setUniform(const char* name, std::vector<float_t> value)
{
    if (value.size() > 4)
    {
        spdlog::error("Max data size exceded, max size is 4");
        return -2;
    }

    if(!m_uniforms.contains(name))
    {
        auto uniform = glGetUniformLocation(m_id, name);
        if (uniform == -1)
        {
            spdlog::error("Uniform {} not found", name);
            return -1;
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
    return 1;
}

int32_t ShaderProgram::setUniform(const char* name, int32_t value)
{
    if(!m_uniforms.contains(name))
    {
        auto uniform = glGetUniformLocation(m_id, name);
        if (uniform == -1)
        {
            spdlog::error("Uniform {} not found", name);
            return -1;
        }
        m_uniforms.insert({name, uniform});
    }

    glUniform1i(m_uniforms.at(name), value);
    return 1;
}

int32_t ShaderProgram::setUniform(const char* name, std::vector<int32_t> value)
{
    if (value.size() > 4)
    {
        spdlog::error("Max data size exceded, max size is 4");
        return -2;
    }

    if(!m_uniforms.contains(name))
    {
        auto uniform = glGetUniformLocation(m_id, name);
        if (uniform == -1)
        {
            spdlog::error("Uniform {} not found", name);
            return -1;
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
    return 1;
}

static int32_t make_shader(uint32_t shader, const char* src, uint32_t type)
{
    int compile_status;

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

        return 0;
    }

    return 1;
}
