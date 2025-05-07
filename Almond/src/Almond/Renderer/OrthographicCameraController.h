#pragma once
#include "Almond/Renderer/OrthographicCamera.h"
#include "Almond/Core/Timestep.h"

#include "Almond/Events/ApplicationEvent.h"
#include "Almond/Events/MouseEvent.h"

namespace Almond {

	// 相机边界
	struct OrthographicCameraBounds {
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};


	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRation, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(const float zoom) { m_ZoomLevel = zoom; CalculateView(); }
		float GetZoomLevel() const { return m_ZoomLevel; }

		const OrthographicCameraBounds& GetBounds() { return m_Bounds; }
	private:
		void CalculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;				// 长宽比
		float m_ZoomLevel = 1.0f;			// 缩放等级
		OrthographicCameraBounds m_Bounds;	// 相机边界
		OrthographicCamera m_Camera;		// 摄像机

		bool m_Rotation;					// 是否启用旋转
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f }; //相机位置
		float m_CameraRotation = 0.0f;		// 旋转位置

		float m_CameraTranslateSpeed = 1.0f, m_CameraRotationSpeed = 30.0f;	// 移动和旋转速度
	};

}