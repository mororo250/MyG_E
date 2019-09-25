#pragma once

#include <iostream>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

void open_file_browser(std::string& path);
void save_file_browser(std::string& path);