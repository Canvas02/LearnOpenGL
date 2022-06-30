#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

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

constexpr std::array<float, 9> vertices_base{
	 0.25f, -0.25f, 0.0f,
	-0.25f, -0.25f, 0.0f,
	 0.0f,   0.25f, 0.0f
};

/*
constexpr std::array<unsigned int, 6> indices{
	0, 1, 2,
	0, 3, 2
};
*/

int main()
{
	std::array<std::array<float, 9>, 2> vertices{};
	
	for (int i = 0; i < vertices_base.size(); i++)
	{
		if (i == 0 || i % 3 == 0)
		{
			vertices[0][i] = vertices_base[i] + 0.35f;
			vertices[1][i] = vertices_base[i] - 0.35f;
		} 
		else
		{
		vertices[0][i] = vertices_base[i];
		vertices[1][i] = vertices_base[i];
		}
	}

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
	
	std::array<unsigned int, 2> vbo{}, vao{};
	glGenVertexArrays(vao.size(), &vao[0]);
	glGenBuffers(vbo.size(), &vbo[0]);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferStorage(GL_ARRAY_BUFFER, vertices[0].size() * sizeof(float), &vertices[0][0], 0);
	//							  pointer of first element of first array ^^

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferStorage(GL_ARRAY_BUFFER, vertices[1].size() * sizeof(float), &vertices[1][0], 0);
	//							 pointer of first element of second array ^^

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	const auto vert_src = read_file("res/shaders/basic.vert.glsl");
	const auto b_frag_src = read_file("res/shaders/blue.frag.glsl");
	const auto y_frag_src = read_file("res/shaders/yellow.frag.glsl");

	std::array<unsigned int, 2> programs{};
	programs[0] = make_program(vert_src.c_str(), b_frag_src.c_str());
	programs[1] = make_program(vert_src.c_str(), y_frag_src.c_str());

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao[0]);
		glUseProgram(programs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(vao[1]);
		glUseProgram(programs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(vbo.size(), &vbo[0]);
	glDeleteVertexArrays(vao.size(), &vao[0]);
	for (auto program : programs)
	{
		glDeleteProgram(program);
	}

	glfwTerminate();
    return 0;
}
