project "TexScript"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tspch.hpp"
	pchsource "src/tspch.cpp"

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"vendor/lua/include"
	}

	links
	{
		"vendor/lua/lua54.lib"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

		filter "configurations:Debug"
		defines "TS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TS_DIST"
		runtime "Release"
		optimize "on"