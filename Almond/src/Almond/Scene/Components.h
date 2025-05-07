#pragma once

#include "glm/glm.hpp"
#include "Almond/Renderer/Camera.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Almond {

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string tag)
			:Tag(tag) {}
	};

	struct TransformComponent {
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			:Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }

	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			:Color(color) {}

	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true;			// 当前摄像头是否为主摄像头
		bool FixedAspectRatio = false;	// 是否为固定宽高比

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent {
		// 当前绑定的脚本实例指针
		ScriptableEntity* Instance = nullptr;

		// 创建和销毁脚本的函数指针
		std::function<ScriptableEntity*()> InstantiateScript;
		std::function<void(NativeScriptComponent*)> DestroyScript;

		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}