#include "tspch.hpp"

#include "TexScript/Base/Application.hpp"

int main(int argc, char** argv)
{
	TexScript::Log::Init();
	TS_INFO("Initialized Log!");

	TexScript::Application app;
	TS_INFO("Initialized App!");

	app.Run();
	TS_INFO("Shutting down App!");

	return 0;
}