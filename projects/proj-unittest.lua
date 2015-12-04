-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "UnitTest"
	kind "ConsoleApp"
	language "C++"
	debugdir "../bin"
	targetdir "../bin"
	location "../build/unittest"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/unittest/**.cpp", "../src/unittest/**.h", "../src/unittest/**.inl" }
	includedirs { "../src" }
	links { "Core", "Engine" }
	prebuildcommands { cxxcommand }
	
	-- set up unit test framework
	filter "configurations:Debug"
		defines { "_DEBUG", "_CXXTEST_HAVE_EH" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG", "_CXXTEST_HAVE_EH" }
		flags { "Optimize" }
		
	filter "system:windows"
		includedirs { 	path.join(libdir, "cxxtest"),
						path.join(libdir, "zlib/include") }
						
		libdirs { 	path.join(libdir, "zlib/lib") }
		
	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		
