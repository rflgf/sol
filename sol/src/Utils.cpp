#include "Utils.h"

#include "Core.h"

#include <fstream>

namespace sol
{
namespace utils
{
std::string read_from_file(const char *filepath)
{

	std::string file_text;
	std::fstream file(filepath, std::ios::in | std::ios::binary);
	if (file)
	{
		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		if (size != -1)
		{
			file_text.resize(size);
			file.seekg(0, std::ios::beg);
			file.read(&file_text[0], size);
		}
		else
			SOL_CORE_ERROR("could not read from file '{}'", filepath);
	}
	else
		SOL_CORE_ERROR("could not open file '{}'", filepath);
	return file_text;
}
} // namespace utils
} // namespace sol