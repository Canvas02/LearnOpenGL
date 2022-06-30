#include "file.h"

#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

std::string read_file(const char* file_path)
{
	std::ifstream file(file_path);
	std::stringstream buffer;
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			buffer << line << '\n';
		}
		file.close();
	}
	else {
		spdlog::error("Failed to open file {}", file_path);
		exit(EXIT_FAILURE);
	}

	return buffer.str();
}

void write_file(const char* file_name, const char* content)
{
	std::ofstream file(file_name, std::ofstream::out | std::ofstream::trunc);

	file << content;

	file.close();
}
