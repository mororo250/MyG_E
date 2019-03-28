#pragma once
#include "Math.h"

class Camera 
{
public:
	Camera(Vector<float, 3> Position, Vector<float, 3> Target);
	~Camera();

	inline Vector<float, 3> GetPosition() const { return mPosition; }
	inline Vector<float, 3> GetFrontCamera() const { return mFrontCamera; }
	inline Matrix<float, 4, 4>& GetView() { return mView; }

	void Update(Vector<float, 3> position, Vector<float, 3> frontcamera);
private:
	Vector<float, 3> mPosition;
	Vector<float, 3> mFrontCamera;
	Matrix<float, 4, 4> mView;
};
