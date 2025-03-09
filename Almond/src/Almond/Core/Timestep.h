#pragma once

namespace Almond {

	class Timestep {
	public:
		Timestep(float time)
			:m_Time(time){}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMillionSeconds() const { return m_Time * 1000; }

	private:
		float m_Time;
	};

}