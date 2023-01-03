workspace "MetaRay"
	architecture "x86_64"
	startproject "MetaRay"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glm"] = "MetaRay/vendor/glm"

project "MetaRay"
	location "MetaRay"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.glm}",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
	        debugargs{">", "image.ppm"}
		defines {"MR_DEBUG", "_USE_MATH_DEFINES"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		debugargs{">", "image.ppm"}
		defines {"MR_RELEASE", "_USE_MATH_DEFINES"}
		runtime "Release"
		optimize "on"
