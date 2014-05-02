-- JEngine SSE PreMake 5 configuration file
-- Copyright 2013, Jeroen Broekhuizen

-- create the project
project "mod_ui"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/mods/mod_ui"
	
	-- set project files
	files { "src/mods/mod_ui/**.cpp", "src/mods/mod_ui/**.h", "src/mods/mod_ui/**.inl" }
	includedirs { "src" }
	links { "Core" }
	defines { "MOD_EXPORTS", "TIXML_USE_STL" }
		
	configuration "Debug"
		defines { "_DEBUG" }
		flags { "Symbols" }
		targetsuffix "d"
		
	configuration "Release"
		defines { "NDEBUG" }
		optimize "On"

	configuration "windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "tinyxml/include") }
		libdirs { path.join(libdir, "tinyxml/lib") }
		links { "d3dcompiler.lib" }
	
	configuration { "Debug", "windows" }
		links { "tinyxmld_STL.lib" }
			
	configuration { "Release", "windows" }
		links { "tinyxml_STL.lib" }

	configuration "linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
