#include "Utils.h"

#include "Core.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

namespace sol::utils
{

std::string read_from_file(const std::string &filepath,
                           const std::_Iosb<int>::_Openmode mode)
{

	std::string file_text;
	std::ifstream file(filepath, mode);
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

std::string current_working_directory(const std::filesystem::path &append)
{
	std::filesystem::path cwd = std::filesystem::current_path();
	return (cwd / append).make_preferred().string();
}

// FileDialog-------------------------------------------------------------------
std::string
FileDialog::open_file(const std::vector<nfdu8filteritem_t> &filter_items,
                      const std::string &default_path)
{
	std::string out;
	NFD_Init();
	nfdchar_t *output_path;

	nfdresult_t result =
	    NFD_OpenDialog(&output_path, filter_items.data(), filter_items.size(),
	                   default_path.c_str());

	if (result == NFD_OKAY)
	{
		out = output_path;
		NFD_FreePath(output_path);
	}
	else if (result != NFD_CANCEL)
		SOL_CORE_ERROR("could not open file: {}", NFD_GetError());

	NFD_Quit();
	return out;
}

std::string
FileDialog::save_file(const std::string &default_name,
                      const std::vector<nfdu8filteritem_t> &filter_items,
                      const std::string &default_path)
{
	std::string out;
	NFD_Init();
	nfdchar_t *output_path;
	nfdresult_t result =
	    NFD_SaveDialog(&output_path,

	                   filter_items.data(), filter_items.size(),
	                   default_path.c_str(), default_name.c_str());

	if (result == NFD_OKAY)
	{
		out = output_path;
		NFD_FreePath(output_path);
	}
	else if (result != NFD_CANCEL)
		SOL_CORE_ERROR("could not save file: {}", NFD_GetError());

	NFD_Quit();
	return out;
}
// end of FileDialog------------------------------------------------------------

} // namespace sol::utils