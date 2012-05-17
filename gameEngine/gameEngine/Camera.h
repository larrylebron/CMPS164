#include "externalLibs\vec3f.h";
#include "externalLibs\Timer.h";

#pragma once
class Camera
{
public:
	Camera(void);
	~Camera(void);

private:
	Vec3f lookAt, upVector, topDownUpVector;
	Timer timer;
};

