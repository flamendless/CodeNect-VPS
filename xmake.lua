target("CodeNect")
	set_basename("CodeNect_$(mode)_$(arch)")
	set_kind("binary")
	add_cxxflags("-g", "-Wall", "-Wformat", "-pedantic", "$(shell sdl2-config --cflags)")
	add_ldflags("$(shell sdl2-config --libs)")

	--source files
	add_files("src/*.cpp")
	add_includedirs("src")

	--dearimgui
	add_files("includes/imgui/*.cpp", "includes/imgui/backends/imgui_impl_opengl2.cpp", "includes/imgui/backends/imgui_impl_sdl.cpp")
	add_includedirs("includes/imgui", "includes/imgui/backends")

	--imnodes
	add_files("includes/imnodes/*.cpp|sample.cpp")
	add_includedirs("includes/imnodes")

	--inih
	add_includedirs("includes/inih/")

	--fmt
	add_files("includes/fmt/src/*.cc")
	add_includedirs("includes/fmt/include")

	--nativefiledialogs
	add_includedirs("includes/nfd/include")
	if is_plat("linux") then
		add_files("includes/nfd/*.c|nfd_zenity.c")
	end

	--plog
	add_includedirs("includes/plog/include/")

	--stb
	add_includedirs("includes/stb")

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

	on_load(function(target)
		local packages = find_packages("GL", "dl", "gtk+-3.0")

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
