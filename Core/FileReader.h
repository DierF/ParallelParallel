#pragma once

#include <string>
#include <fstream>
#include <cassert>

namespace PParallel
{
	class FileReader
	{
	public:
		static std::string readFile(std::string const& path)
		{
			std::ifstream filename(path);
			assert(filename.is_open());

			return std::string((std::istreambuf_iterator<char>(filename)),
								std::istreambuf_iterator<char>());
		}
	};
}
