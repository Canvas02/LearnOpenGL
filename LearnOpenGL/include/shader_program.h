#pragma once

#include <cstdint>
#include <cmath>
#include <array>
#include <unordered_map>
#include <gl_error.h>

auto make_shader(const char* src, uint32_t type) -> uint32_t;

class ShaderProgram
{
private:
    uint32_t m_id;
    std::unordered_map<const char*, uint32_t> m_uniforms = {};
public:
    ShaderProgram(const char* vert_src, const char* frag_src);
    ~ShaderProgram() noexcept;

    void use() noexcept;

    void setUniform(const char* name, std::array<float_t, 4> value);
    void setUniform(const char* name, std::array<int32_t, 4> value);
};
