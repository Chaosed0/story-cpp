
-- baseName = path.getbasename(os.getcwd());

project ("game")
    kind "ConsoleApp"
    location "../_build"
    targetdir "../_bin/%{cfg.buildcfg}"
	cppdialect "C++20"

    filter "action:vs*"
        debugdir "$(SolutionDir)"
		
	filter {"action:vs*", "configurations:Release"}
		kind "WindowedApp"
		entrypoint "mainCRTStartup"
		
    filter{}

    vpaths 
    {
        ["Header Files/*"] = { "include/**.h",  "include/**.hpp", "src/**.h", "src/**.hpp", "**.h", "**.hpp"},
        ["Source Files/*"] = {"src/**.c", "src/**.cpp","**.c", "**.cpp"},
    }
    files {"**.c", "**.cpp", "**.h", "**.hpp"}
  
    includedirs { "./" }
    includedirs { "src" }
    includedirs { "include" }
    
    link_raylib()
	
	link_to("lua54")
	link_to("spdlog")
	link_to_include_only("sol")
	link_to_include_only("raylib-cpp")
	
	link_to("inkcpp")
	links ("inkcpp_compiler")
	
	postbuildcommands {
		"{COPY} ../libs/lua54/lua54.dll %{cfg.targetdir}",
		"{COPYDIR} ../assets/ %{cfg.targetdir}/assets/",
		"../compile-ink.bat assets/ink/MageSchool.ink"
	}