#pragma once

#include <filesystem>
#include <fstream>
#include <nfd.h>
#include <string>
#include <utility>
#include <vector>

namespace sol::utils
{

std::string read_from_file(const std::string &filepath,
                           const std::_Iosb<int>::_Openmode mode);
std::string current_working_directory(const std::filesystem::path &append = "");

class FileDialog
{
public:
	// NOTE(rafael): these return an empty std::string on dialog cancel.
	static std::string open_file(const std::vector<nfdu8filteritem_t>
	                                 &filter_items = {{"All files", "(.*)"}},
	                             const std::string &default_path = "");
	static std::string save_file(const std::string &default_name,
	                             const std::vector<nfdu8filteritem_t>
	                                 &filter_items = {{"All files", "(.*)"}},
	                             const std::string &default_path = "");
};

} // namespace sol::utils