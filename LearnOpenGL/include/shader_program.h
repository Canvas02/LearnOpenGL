#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <gl_error.h>

auto make_shader(const char* src, uint32_t type) -> uint32_t;

class ShaderProgram
{
private:
    std::unordered_map<const char*, uint32_t> m_uniforms = {};
public:
    uint32_t id;

    ShaderProgram(const char* vert_src, const char* frag_src);
    ~ShaderProgram() noexcept;

    void use() noexcept;

    void setUniform(const char* name, std::vector<float_t> value);
    void setUniform(const char* name, std::vector<int32_t> value);
};
