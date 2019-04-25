#pragma once

#include "Foundation/Math/Vector.h"


class Light
{
public:
	Light(Vector<float, 3> position, Vector<float, 3> color = { 1.0f, 1.0f, 1.0f });
	~Light();

	Vector<float, 3> GetLightPosition() const { return mPosition; }
	void SetLightPosition(Vector<float, 3> position) { mPosition = position; }

	Vector<float, 3> GetLightColor() const { return mColor; }
	void SetLightColor(Vector<float, 3> color) { mColor = color; }

	void ImGuiRenderer();

private:
	Vector<float, 3> mPosition;
	Vector<float, 3> mColor;
};

