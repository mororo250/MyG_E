#pragma once

class Uncopyable
{
protected:
	Uncopyable() {}
	~Uncopyable() {}

private:
	// Prevent any class inheriting it from being copied.
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};