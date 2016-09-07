require "common"

-------------------------------------------------------------------------------
-- Workspace Definition -------------------------------------------------------
-------------------------------------------------------------------------------

premakeDef.workspace = {
	name = "Sulfur",
	architectures = { "x86_64" },
    flags = { "FatalCompileWarnings", "MultiProcessorCompile", "C++14" }, 
    defines = { "_CRT_SECURE_NO_WARNINGS", "SF_USE_SIMD" },
	startupProject = "SulfurGame",
	characterSet = "MBCS"
}

-------------------------------------------------------------------------------
-- Dependencies ---------------------------------------------------------------
-------------------------------------------------------------------------------
premakeDef.dependencies["VCUnitTests"] = {
	includeDirs = { "$(VCInstallDir)UnitTest/include" },
	libraryDirs = { "$(VCInstallDir)UnitTest/lib" },
}

premakeDef.dependencies["D3D11"] = {
	debugLibs = { "d3d11.lib", "dxguid.lib", "dxgi.lib", "d3dcompiler.lib" },
	releaseLibs = { "d3d11.lib", "dxguid.lib", "dxgi.lib", "d3dcompiler.lib" }
}

premakeDef.dependencies["zlib"] = {
	debugLibs = { "zlibd.lib" },
	releaseLibs = { "zlib.lib" },
	includeDirs = { "../dependencies/zlib/include" },
	libraryDirs = { "../dependencies/zlib/lib" }
}

premakeDef.dependencies["libpng"] = {
	debugLibs = { "libpng16d.lib" },
	releaseLibs = { "libpng16.lib" },
	includeDirs = { "../dependencies/libpng/include" },
	libraryDirs = { "../dependencies/libpng/lib" },
	dependencies = { "zlib" }
}

-------------------------------------------------------------------------------
-- Configurations -------------------------------------------------------------
-------------------------------------------------------------------------------

premakeDef.configurations["Debug"] = {
	defines = { "DEBUG" },
	flags = { "Symbols" },
	optimize = "Debug"
}

premakeDef.configurations["Release"] = {
	defines = { "NDEBUG" },
	optimize = "Speed"
}

premakeDef.configurations["ReleaseSymbols"] = {
	defines = { "NDEBUG" },
	flags = { "Symbols" },
	optimize = "Speed"
}

-------------------------------------------------------------------------------
-- Library Projects -----------------------------------------------------------
-------------------------------------------------------------------------------

premakeDef.projects["SulfurEngine"] = {
	language = "C++",
	kind = "StaticLib",
	dependencies = { "D3D11", "libpng" },
	pchHeader = "sfSulfurPCH.hpp",
	pchSource = "sfSulfurPCH.cpp"
}

premakeDef.projects["UnitTests"] = {
	language = "C++",
	kind = "SharedLib",
	dependencies = { "SulfurEngine", "VCUnitTests" },
	pchHeader = "stdafx.h",
	pchSource = "stdafx.cpp"
}

-------------------------------------------------------------------------------
-- Executable Projects --------------------------------------------------------
-------------------------------------------------------------------------------

premakeDef.projects["SulfurGame"] = {
	language = "C++",
	kind = "WindowedApp",
	dependencies = { "SulfurEngine" }
}


premakeDef.projects["ShaderCompiler"] = {
	language = "C++",
	kind = "ConsoleApp",
	dependencies = { "SulfurEngine" }
}


RunPremake()
