#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../../Logic/Components/TransformComponent.h"
#include "../../Logic/Components/Component.h"
#include "../../Utilities/Vector3.h"
#include "../../Utilities/EventSystem/EventListener.h"

class CameraComponent : public Component, public EventListener
{
	//pointer to a position that we will be following
	Vector3<float>* m_pTargetPos;
	int m_screenCoushion;

public:
	CameraComponent(int cameraWidth, int cameraHeight, GameObject* pParent);
	~CameraComponent();

	//Will make sure the camera still follows its target
	virtual void OnEvent(const Event* pEvent) override;
	void UpdateCamera();

	Vector3<float>* GetPosition();
	//Get the width of the camera
	int GetWidth();
	//Get the height of the camera
	int GetHeight();

	//set the target pointer that we will be following
	void SetTarget(Vector3<float>* pTargetVector);

private:
	//camera screen width
	int m_cameraWidth;
	//camera screen heigh
	int m_cameraHeight;


};
#endif // !__CAMERA_H__
