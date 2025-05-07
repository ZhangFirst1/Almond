#pragma once

#include "ampch.h"
#include "Entity.h"

namespace Almond {

	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene) {}


}