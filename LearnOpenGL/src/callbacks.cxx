#include "callbacks.h"

#include <string>
#include <glad/glad.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	static bool draw_filled = true;

    if (action == GLFW_PRESS)
    spdlog::info("GLFW key press = {}", key);
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_W)
	{
		if (draw_filled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			draw_filled = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			draw_filled = true;
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void debug_msg_callback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	std::string _source;
	std::string _type;
	std::string _severity;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		_source = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		_source = "WINDOW SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		_source = "SHADER COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		_source = "THIRD PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		_source = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		_source = "UNKNOWN";
		break;
	default:
		_source = "UNKNOWN";
		break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		_type = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		_type = "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		_type = "UDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		_type = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		_type = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		_type = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		_type = "MARKER";
		break;

	default:
		_type = "UNKNOWN";
		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		_severity = "HIGH";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		_severity = "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		_severity = "LOW";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		_severity = "NOTIFICATION";
		break;
	default:
		_severity = "UNKNOWN";
		break;
	}

	const auto msg = fmt::format("{}: {} {} {} \n\t{}\n", id, _source, _severity, _type, message);
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		spdlog::error(msg);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		spdlog::warn(msg);
		break;
	case GL_DEBUG_SEVERITY_LOW:
		spdlog::warn(msg);
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		spdlog::info(msg);
		break;
	default:
		spdlog::info(msg);
		break;
	}
}
