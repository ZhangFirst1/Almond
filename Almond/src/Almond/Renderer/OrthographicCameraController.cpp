#include "ampch.h"
#include "OrthographicCameraController.h"

#include "Almond/Core/Input.h"
#include "Almond/Core/KeyCodes.h"

namespace Almond {

	OrthographicCameraController::OrthographicCameraController(float aspectRation, bool rotation)
		:m_AspectRatio(aspectRation), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		AM_PROFILE_FUNCTION();

		if (Input::IsKeypressed(AM_KEY_A))
			m_CameraPosition.x -= m_CameraTranslateSpeed * ts;
		else if (Input::IsKeypressed(AM_KEY_D))
			m_CameraPosition.x += m_CameraTranslateSpeed * ts;

		if (Input::IsKeypressed(AM_KEY_S))
			m_CameraPosition.y -= m_CameraTranslateSpeed * ts;
		else if (Input::IsKeypressed(AM_KEY_W))
			m_CameraPosition.y += m_CameraTranslateSpeed * ts;

		if (m_Rotation) {
			if (Almond::Input::IsKeypressed(AM_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			else if (Almond::Input::IsKeypressed(AM_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslateSpeed = m_ZoomLevel;
	}

	void Almond::OrthographicCameraController::OnEvent(Event& e)
	{
		AM_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AM_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AM_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool Almond::OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		AM_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool Almond::OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		AM_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}