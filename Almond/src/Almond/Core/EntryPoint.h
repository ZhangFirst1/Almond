#pragma once

#ifdef AM_PLATFORM_WINDOWS

extern Almond::Application* Almond::CreateApplication();

int main(int argc, char** argv) {
	Almond::Log::Init();
	AM_CORE_WARN("Initialized Log!");
	int a = 10; 
	AM_INFO("Initialized Log! Var={0}", a);

	AM_CORE_ERROR("lfqlfqlfq");

	auto app = Almond::CreateApplication();
	app->Run();
	delete app;
}

#endif