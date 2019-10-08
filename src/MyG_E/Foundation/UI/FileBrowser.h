#pragma once

std::string open_file_browser(std::wstring const& file_types, std::wstring const& filter_name);
std::string save_file_browser(std::wstring const& file_types, std::wstring const& filter_name);
std::string open_folder_browser();