#include "ampch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Almond {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatirx()
	{
		// glm::translate用于将相机移动到m_Position，glm::rotate用于旋转相机
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		// 相机的transform矩阵和物体的transform是互逆的，求出相机的transform再求逆即可得到物体的
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}


