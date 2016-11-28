#ifndef COMPONENT_H
#define COMPONENT_H
#include <stdint.h>

class Scene;

//Inside a namespace because there is a global static that is needed.
namespace Important
{
	static int32_t nextType = 0;
}

class ComponentInterface
{
public:
	virtual int32_t getType	() const = 0;
	int32_t getEntityID		() const { return entityID; }
	void setEntityID(int32_t id) { entityID = id; }

	inline bool isActive	() const		{ return active; }
	inline void setActive	(bool isActive) { active = isActive; }

private:

	bool active			= true;
	int32_t ID			= -1;
	int32_t entityID	= -1;

	friend Scene;
};


template <class T>
class Component : public ComponentInterface
{
public:

	int32_t getType() const { return T::type; }

private:

	static int32_t type;
	
	friend Scene;
};


template <typename T>
int32_t Component<T>::type(Important::nextType++);





#endif // !COMPONENT_H
