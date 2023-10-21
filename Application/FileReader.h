#pragma once

#include <vector>
#include <string>
#include <fstream>

namespace PParallel
{
	std::vector<char> readFile(std::string const& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (not file.is_open())
		{
			throw std::runtime_error("failed to open file!");
		}

		size_t            fileSize{ (size_t)file.tellg() };
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}
}