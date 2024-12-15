#include "Component.h"
#include "Actor.h"
#include "InputSystem.h"

Component::Component(Actor* owner, int updateOrder)
	: mOwner(owner)
	, mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}