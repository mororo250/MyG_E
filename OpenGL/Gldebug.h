#pragma once

//this file implement Macors and fuction to helb the debug process
bool glCheckError_(const char *function, const char *file, const int line);

#define ASSERT(x) if((x)) __debugbreak();
#define GLcall(x) x;  ASSERT(glCheckError_(#x, __FILE__, __LINE__));

