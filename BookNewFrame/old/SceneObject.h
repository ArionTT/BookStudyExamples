#pragma once
#include"BaseKits.h"


//´ýÑ¡ SceneUnit  »ò Unit
class SceneObject
{
public:
	SceneObject()
	{
		Transform*trans=new Transform();
		components.push_back(trans);
	}
public:
	template<class T>
	Object* GetComponent()
	{
		
		return components.back();
	}
	template<class T>
	bool AppendComponent(T *type)
	{
		components.push_back(type);
		return true;
	}

	bool SetBaseAttribute(Transform base)
	{
		return true;
	}

protected:
	vector<Object*> components;
};



