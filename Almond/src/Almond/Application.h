#pragma once

#include "Core.h"

namespace Almond {

	class ALMOND_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();

}

