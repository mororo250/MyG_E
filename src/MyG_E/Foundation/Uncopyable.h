#pragma once

class Uncopyable
{
protected:
	Uncopyable() {}
	~Uncopyable() {}

private:
	// Prevent any class inheriting it from being copied.
	Uncopyable(Uncopyable const&);
	Uncopyable& operator=(Uncopyable const&);
};
