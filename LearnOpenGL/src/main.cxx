#include <array>
#include <cstdint>
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "platfrom.h"
#include "callbacks.h"
#include "file.h"
#include "shader_program.h"

extern "C"
{
	DLL_EXPORT unsigned long NvOptimusEnablement = 0x00000001;
    DLL_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

constexpr int32_t WIDTH = 800, HEIGHT = 600;

constexpr std::array<float_t, 8 * 4> vertices{
//	 [0] aPos				[1] aColor			[2] aTexCoord
	 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f
};

constexpr std::array<uint32_t, 6> indices{
	0, 1, 2,
	0, 3, 2
};

int main()
{
	if (!glfwInit())
	{
		spdlog::error("Failed to init glfw");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if !defined(NDEBUG)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // !defined(NDEBUG)

	auto window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (!window)
	{
		spdlog::error("Failed to create glfw window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // V-sync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to load opengl functions");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	spdlog::info("{}", glGetString(GL_RENDERER));
	spdlog::info("Version: {}", glGetString(GL_VERSION));

	// Init debug
	int32_t flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debug_msg_callback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		puts("Debug output enabled\n\n");
	}
	else
	{
		puts("Debug output disabled\n\n");
	}

	// Texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image
	int32_t width, height, nrChannels;
	auto data = stbi_load("res/textures/wood_container.jpg", &width, &height, &nrChannels, 0);
	if (!data)
	{
		spdlog::error("Failed to load image");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	uint32_t vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferStorage(GL_ARRAY_BUFFER, vertices.size() * sizeof(float_t), &vertices[0], 0);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], 0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float_t) * 8, (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float_t) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float_t) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

	const auto vert_src = read_file("res/shaders/basic.vert.glsl");
	const auto frag_src = read_file("res/shaders/basic.frag.glsl");
	// const auto y_frag_src = read_file("res/shaders/yellow.frag.glsl");
	// const auto b_frag_src = read_file("res/shaders/blue.frag.glsl");

	try { 
		ShaderProgram program(vert_src.c_str(), frag_src.c_str());
		program.use();

		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindTexture(GL_TEXTURE_2D, texture);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	catch (gl_error& ecx)
	{
		spdlog::error("Opengl: {}", ecx.what());
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwTerminate();
	return 0;
}
