#pragma once

int make_shader(unsigned int shader, const char* src, unsigned int type);
unsigned int make_program(const char* vert_src, const char* frag_src);