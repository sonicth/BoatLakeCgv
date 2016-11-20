	-- A solution contains projects, and defines the available configurations
-- local BOOST_DIR = "M:/development/frameworks/src"


local PATH = ("build/".._ACTION)
local GLUT_INC_DIR
local GLUT_LIB_DIR

if os.get() == "windows" then
	GLUT_INC_DIR = "K:/frameworks/src/freeglut-3.0.0/include"
	GLUT_LIB_DIR = "K:/frameworks/build/freeglut-3.0.0-build/lib"
else
end

solution "BoatLake"
	location (PATH)
	targetdir (PATH .. "/../products")
	configurations { "Debug", "Release" }

	platforms { "x64" }

	language "C"
	-- defines {	"_USE_MATH_DEFINES", 
		-- }

	includedirs { GLUT_INC_DIR }
	
	links { "freeglutd" }
		
	debugdir "$(TargetDir)"
	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		libdirs { GLUT_LIB_DIR .. "/Debug"
			}
	
	configuration "Release"
		defines { "NDEBUG" }
		libdirs { GLUT_LIB_DIR .. "/Release"
			}
----------------------------------------------------------------
--projects
----------------------------------------------------------------
	project "BoatLake"
		kind "ConsoleApp"
		files { "src/*",
			}
		excludes { "src/Makefile" }
		

