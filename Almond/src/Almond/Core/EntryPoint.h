#pragma once

#ifdef AM_PLATFORM_WINDOWS

extern Almond::Application* Almond::CreateApplication();

int main(int argc, char** argv) {
	Almond::Log::Init();
	
	// AM_PROFILE_BEGIN_SESSION("Startup", "AlmondProfile-Startup.json");
	auto app = Almond::CreateApplication();
	// AM_PROFILE_END_SESSION();

	// AM_PROFILE_BEGIN_SESSION("Runtime", "AlmondProfile-Runtime.json");
	app->Run();
	// AM_PROFILE_END_SESSION();


	// AM_PROFILE_BEGIN_SESSION("Shutdown", "AlmondProfile-Shutdown.json");
	delete app;
	// AM_PROFILE_END_SESSION();
}

#endif