#pragma once
#include "Foundation/Math.h"
#include "Foundation/Input.h"

class Camera 
{
public:
	Camera(float speed = 0.5f, float sensitivity = 0.005f);
	virtual ~Camera();

	inline Matrix<float, 4, 4>& GetView() { return mView; }

	inline Vector<float, 3> GetPosition() const { return mPosition; }
	inline void SetPosition(const Vector<float, 3> position) { mPosition = position; }
	
	inline Vector<float, 3> GetDirection() const { return mFrontCamera; }
	void SetDirection(Vector<float, 3> front_camera) { mFrontCamera = front_camera; }
	
	inline float GetSpeed() const { return mSpeed; }
	inline void SetSpeed(const float speed) { mSpeed = speed; }
	
	inline float GetSensitivity() const { return mSensitivity; }
	inline void SetSensitivity(const float sensitivity) { mSensitivity = sensitivity; }

	virtual void ImGuiRenderer();
	virtual void Update() = 0;

protected:
	virtual void Translate() = 0;
	virtual void Rotate() = 0;

	float mSpeed; //speed of camera moviment
	float mSensitivity; //velocity in witch the camera will change front_camera

	std::pair<float, float> mMousePos; //currenty mouse possition

	Vector<float, 3> mFrontCamera;
	Vector<float, 3> mPosition;
	Matrix<float, 4, 4> mView;
};
