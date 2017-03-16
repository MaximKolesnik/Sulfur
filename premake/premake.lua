require "common"

-------------------------------------------------------------------------------
-- Workspace Definition -------------------------------------------------------
-------------------------------------------------------------------------------

premakeDef.workspace = {
	name = "Sulfur",
	architectures = { "x86_64" },
    flags = { "FatalCompileWarnings", "MultiProcessorCompile", "C++14", "FatalLinkWarnings", "NoIncrementalLink", "LinkTimeOptimization" }, 
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
	debugLibs = { "libpngd.lib" },
	releaseLibs = { "libpng.lib" },
	includeDirs = { "../dependencies/libpng/include" },
	libraryDirs = { "../dependencies/libpng/lib" },
	dependencies = { "zlib" }
}

premakeDef.dependencies["libjpeg"] = {
	debugLibs = { "jpegd.lib" },
	releaseLibs = { "jpeg.lib" },
	includeDirs = { "../dependencies/libjpeg/include" },
	libraryDirs = { "../dependencies/libjpeg/lib" }
}

premakeDef.dependencies["openvr"] = {
	debugLibs = { "openvr_api64d.lib" },
	releaseLibs = { "openvr_api64.lib" },
	includeDirs = { "../dependencies/openvr/include" },
	libraryDirs = { "../dependencies/openvr/lib" }
}

premakeDef.dependencies["fbx"] = {
	debugLibs = { "libfbxsdk-mdd.lib" },
	releaseLibs = { "libfbxsdk-md.lib" },
	includeDirs = { "../dependencies/fbx/include" },
	libraryDirs = { "../dependencies/fbx/lib" }
}

premakeDef.dependencies["dds"] = {
	includeDirs = { "../dependencies/dds/include" }
}

premakeDef.dependencies["WinApi"] = {
	debugLibs = { "Shlwapi.lib" },
	releaseLibs = { "Shlwapi.lib" }
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
	defines = { "SF_BUILDING_LIB" },
	dependencies = { "D3D11", "libpng", "libjpeg", "fbx", "dds", "WinApi", "openvr" },
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

premakeDef.projects["SulfurEditor"] = {
	language = "C++",
	qtModules = { "core", "gui", "widgets" },
	kind = "Qt",
	dependencies = { "SulfurEngine" },
	defines = "SF_EDITOR",
	pchHeader = "sfEditorPCH.hpp",
	pchSource = "sfEditorPCH.cpp"
}

premakeDef.projects["SkyboxGen"] = {
	language = "C++",
	qtModules = { "core", "gui", "widgets" },
	kind = "Qt",
	dependencies = { "SulfurEngine" },
	defines = "SF_EDITOR",
	pchHeader = "sfEditorPCH.hpp",
	pchSource = "sfEditorPCH.cpp"
}

premakeDef.projects["SulfurGame"] = {
	language = "C++",
	kind = "WindowedApp",
	dependencies = { "SulfurEngine" }
}

premakeDef.projects["SulfurSandbox"] = {
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
