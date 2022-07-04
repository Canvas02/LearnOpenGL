#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <unordered_map>
//#include <optional>

class ShaderProgram
{
private:
    uint32_t m_id;
    std::unordered_map<const char*, uint32_t> m_uniforms = {};
public:
    ShaderProgram(const char* vert_src, const char* frag_src);
    ~ShaderProgram();

    void use();

    int32_t setUniform(const char* name, bool value);
    int32_t setUniform(const char* name, int32_t value);
    int32_t setUniform(const char* name, float_t value);
    int32_t setUniform(const char* name, std::vector<float_t> value);
    int32_t setUniform(const char* name, std::vector<int32_t> value);
};
