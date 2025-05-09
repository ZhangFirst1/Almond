#pragma once

#include "Scene.h"
#include "Almond/Core/Core.h"
#include <yaml-cpp/yaml.h>

namespace Almond {

	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);

		// 序列化
		void SerializeEntity(YAML::Emitter& out, Entity entity);
		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		// 反序列化
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;		// 接收一个场景，用于在场景中添加Entity以绘制到场景中
	};



}