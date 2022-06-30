#include <array>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

#include "platfrom.h"
#include "callbacks.h"
#include "file.h"
#include "shader_util.h"

extern "C" 
{
	DLL_EXPORT unsigned long	NvOptimusEnablement = 0x00000001;
	DLL_EXPORT int				AmdPowerXpressRequestHighPerformance = 1;
}

constexpr int WIDTH = 800, HEIGHT = 600;

constexpr std::array<float, 8> vertices{
 	 0.5f, -0.5f,
	-0.5f, -0.5f,
	-0.5f,  0.5f,
	 0.5f,  0.5f
};

constexpr std::array<unsigned int, 6> indices{
	0, 1, 2,
	0, 3, 2
};

int main()
{
    if (!glfwInit())
    {
        spdlog::error("Failed to init glfw");
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#   if !defined(NDEBUG)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#   endif // !defined(NDEBUG)

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        spdlog::error("Failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);    // V-sync
    
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to load opengl functions");
		glfwTerminate();
		return 1;
	}

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    spdlog::info("{}", glGetString(GL_RENDERER));
	spdlog::info("Version: {}", glGetString(GL_VERSION));

    // Init debug
	GLint flags = 0;
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
	
	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferStorage(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], 0);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], 0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	const auto vert_src = read_file("res/shaders/basic.vert");
	const auto frag_src = read_file("res/shaders/basic.frag");

	auto program = make_program(vert_src.c_str(), frag_src.c_str());
	glUseProgram(program);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);

	glfwTerminate();
    return 0;
}

