-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "mod_ogl"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	location "../build/mods/mod_ogl"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/mods/mod_ogl/**.cpp", "../src/mods/mod_ogl/**.h", "../src/mods/mod_ogl/**.inl" }
	includedirs { "../src", "../src/mods" }
	links { "SDL" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
			
	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	-- set the system specific settings
	filter "system:Windows"
		defines { "WIN32", "OGL_EXPORTS" }
        links { "Core" }
	
		includedirs {
			path.join(libdir, "glee/include"),
			path.join(libdir, "sdl/include"),
			path.join(libdir, "freetype2/include"),
		}

		libdirs {
			path.join(libdir, "glee/lib"),
			path.join(libdir, "sdl/lib"),
			path.join(libdir, "freetype2/lib"),
		}
		
	filter "system:Linux"
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		defines { "LINUX" }
		includedirs { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
        libdirs { "../bin" }
		links { "GLEW", "freetype" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore" }


	-- set IDE specific settings
	filter "action:vs*"
		links { "opengl32", "glu32" }
		
	filter { "action:vs*", "Debug" }
		links { "Glew_d", "freetype242MT_D" }
				
	filter { "action:vs*", "Release" }
		links { "Glew", "freetype242MT" }


