target("CodeNect")
	set_basename("CodeNect_$(mode)_$(arch)")
	set_languages("cxx17")
	set_kind("binary")
	add_cxxflags("-g", "-Wall", "-Werror", "-Wformat", "-pedantic")

	--source files
	add_files("src/*.cpp")
	add_files("src/core/*.cpp")
	add_files("src/modules/*.cpp")
	add_files("src/node/*.cpp")
	add_files("src/popups/*.cpp")
	add_files("src/ui/*.cpp")
	add_includedirs("src/")

	--better-enums
	add_includedirs("includes/better-enums")

	--dearimgui
	add_files("includes/imgui/*.cpp", "includes/imgui/backends/imgui_impl_opengl2.cpp", "includes/imgui/backends/imgui_impl_glfw.cpp")
	add_includedirs("includes/imgui", "includes/imgui/backends")

	--fmt
	add_files("includes/fmt/src/*.cc")
	add_includedirs("includes/fmt/include")

	--iconfontcppheaders
	add_includedirs("includes/iconfont")

	--imnodes
	add_files("includes/imnodes/*.cpp|sample.cpp")
	add_includedirs("includes/imnodes")

	--simpleini
	add_files("includes/simpleini/ConvertUTF.c")
	add_includedirs("includes/simpleini/")

	--nativefiledialogs
	add_includedirs("includes/nfd/include")
	if is_plat("linux") then
		add_files("includes/nfd/*.c|nfd_zenity.c")
	end

	--plog
	add_includedirs("includes/plog/include/")

	--ppk_assert
	add_files("includes/ppk/src/*.cpp")
	add_includedirs("includes/ppk/src/")

	--stb
	add_includedirs("includes/stb")

	--tweeny
	add_includedirs("includes/tweeny/include/")

	--verbalexpressions
	add_includedirs("includes/verbal_expr/")

	if is_mode("debug") then
		add_defines("DEBUG_MODE=1")
		set_symbols("debug")
		set_optimize("none")
	end

	if is_mode("release") then
		set_symbols("hidden")
		set_optimize("fastest")
		set_strip("all")
	end

	if is_host("linux") then
		add_defines("OS_LINUX=1")
	elseif is_host("windows") then
		add_defines("OS_WIN=1")
	end

	on_load(function(target)
		local packages = find_packages("GL", "gtk+-3.0", "glfw3")

		target:add(packages)
	end)

	if is_arch("x86_64") then
		if is_mode("debug") then
			add_linkdirs("libs/nfd/Debug/x64")
		elseif is_mode("release") then
			add_linkdirs("libs/nfd/Release/x64")
		end
	end

	after_link(function(target)
		local mode

		if is_mode("debug") then mode = "debug"
		else mode = "release"
		end

		os.cp("$(curdir)/assets", "$(buildir)/$(os)/$(arch)/" .. mode)
		os.cp("$(curdir)/config.ini", "$(buildir)/$(os)/$(arch)/" .. mode)
	end)
