require "xcode"
require "qt" 
local qt = premake.extensions.qt

-- Premake definition
premakeDef = {

	-- Array of dependency definitions. Definitions can include these properties:
	--
	-- debugLibs : (Required) Array of library files to link for debug configs (ex. debugLibs = { d3d11.lib })
	-- releaseLibs : (Required) Array of library files to link for release configs (ex. releaseLibs = { d3d11.lib })
	-- includeDirs : Array of include directories
	-- libraryDirs : Array of library directories
	-- dependencies : Array of dependency names that this dependency depends on
	dependencies = {},

	-- Workspace definition. Can include these properties:
	--
	-- name : (Required) The name of the workstation. Will also be the file name
	-- architectures : (Required) Array of supported architectures (any combination of x86, x86_64)
    -- flags : Array of flags (see https://github.com/premake/premake-core/wiki/flags)
	-- defines : Array of global defines for the workspace
	-- startupProject : The name of the default startup project
	-- characterSet : Character set to use ( MBCS or Unicode )
	workspace = {},

	-- Array of configurations. Configurations can include these properties:
	--
	-- defines : Array of defines for this configuration
	-- flags : Array of flags (see https://github.com/premake/premake-core/wiki/flags)
	-- optimize : Optimization setting (see https://github.com/premake/premake-core/wiki/optimize)
	configurations = {},

	-- Array of projects. Projects can include these properties:
	--
	-- language : (Required) The language of this project (see https://github.com/premake/premake-core/wiki/language)
	-- kind : (Required) The project type (ConsoleApp, WindowedApp, SharedLib, StaticLib)
	-- dependencies : Array of dependency names that this project depends on
	-- includeDirs : Array of additional include directories for this project
	-- defines : Array of defines for this project
	-- pch : Precompiled header file name, without extension
	-- qtModules : Array of qt modules to link (see qt/qtmodules.lua for modules)
	projects = {},

}

-- GetLibs
-- Gets all libs of the list of dependencies, including sub-dependencies
function GetLibs(release, depends, out)
	if (depends ~= nil) then
		for i=1, #depends do
			depend = premakeDef.dependencies[depends[i]]
			
			if release then
				dependLibs = depend["releaseLibs"]
			else
				dependLibs = depend["debugLibs"]
			end
			
			if (dependLibs ~= nil) then
				for j=1, #dependLibs do
					out[#out + 1] = dependLibs[j]
				end
			end
			
			GetLibs(release, depend.dependencies, out)
		end
	end
end

-- GetLibDirsForDependencies
-- Gets all library directories for the given list of dependencies, including sub-dependencies
function GetLibDirsForDependencies(depends, out)
	if (depends ~= nil) then
		for i=1, #depends do
			depend = premakeDef.dependencies[depends[i]]
			
			if (depend.libraryDirs ~= nil) then
				for j=1, #depend.libraryDirs do
					out[#out + 1] = depend.libraryDirs[j]
				end
			end
			
			GetLibDirsForDependencies(depend.dependencies, out)
		end
	end
end

-- GetIncludeDirsForDependencies
-- Gets all include directories for the given list of dependencies, including sub-dependencies
function GetIncludeDirsForDependencies(depends, out)
	if (depends ~= nil) then
		for i=1, #depends do
			depend = premakeDef.dependencies[depends[i]]
			
			if (depend.includeDirs ~= nil) then
				for j=1, #depend.includeDirs do
					out[#out + 1] = depend.includeDirs[j]
				end
			end
			
			GetIncludeDirsForDependencies(depend.dependencies, out)
		end
	end
end

-- GetIncludeDirsForProject
-- Gets all include directories for the given project, including dependencies
function GetIncludeDirsForProject(proj, out)
	if (proj.includeDirs ~= nil) then
		for i=1, #proj.includeDirs do
			out[#out + 1] = proj.includeDirs[i]
		end
	end
	
	GetIncludeDirsForDependencies(proj.dependencies, out)
end

-- GetConfigurationNames
-- Gets a list of configuration names
function GetConfigurationNames()
	local configNames = {}
	local n = 0
	
	for configName, config in pairs(premakeDef.configurations) do
		n = n + 1
		configNames[n] = configName
	end
	
	return configNames
end

-- CreateConfigurations
-- Generates premake code for all configurations
function CreateConfigurations()
	for configName, config in pairs(premakeDef.configurations) do
		configuration(configName)
		defines(config.defines)
		flags(config.flags)
		optimize(config.optimize)
	end
end

-- CreatePlatforms
-- Generates premake code for all platforms
function CreatePlatforms()
	platforms(premakeDef.workspace.architectures)
	
	for i=1, #premakeDef.workspace.architectures do
		arch = premakeDef.workspace.architectures[i]
        filter { "platforms:"..arch }
		architecture(arch)
    end
end

-- CreateProjects
-- Generates premake code for the given project
function CreateProject(projName, projKind, proj)
	if (projKind == "WindowedApp" or projKind == "ConsoleApp" or projKind == "Qt") then
		group("Executables")
	else
		group("Libraries")
	end
	
	project(projName)
	language(proj.language)
	
	if (projKind == "Qt") then
		kind "WindowedApp"
	else
		kind(projKind)
	end
	
	filename(projName)
	location("../workspaces/"..premakeDef.workspace.name.."/".._ACTION.."/Projects/")
	files { "../src/"..projName.."/**.*" }
	defines(proj.defines)
	
	-- Include dirs
	includes = { "../src/"..projName }
	GetIncludeDirsForProject(proj, includes)
	includedirs(includes)
	
	-- Precompiled header
	if (proj.pchHeader ~= null) then
		pchheader(proj.pchHeader)
		pchsource("../src/"..projName.."/"..proj.pchSource)
		forceincludes(proj.pchHeader)
	end

	if (projKind == "Qt") then
		language("C++")
		qt.enable()
		qtpath "../dependencies/qt"
		qtgenerateddir("../src/"..projName.."/GeneratedFiles")
		qtmodules(proj.qtModules)
		qtprefix "Qt5"
		debugenvs { "QT_QPA_PLATFORM_PLUGIN_PATH=../bin/platforms" }
	end
	
	-- Project configurations
	for configName, config in pairs(premakeDef.configurations) do
		configuration(configName)
		objdir("../builds/obj/".._ACTION)
		
		isRelease = config.optimize == "Speed"
		
		-- Static libs
		if (projKind == "StaticLib") then
			if (isRelease) then
				targetname(projName)
			else
				targetname(projName.."_d")
			end
		elseif (projKind == "WindowedApp") then
			flags { "WinMain" }
		end
		
		-- Link libs
		libs = {}
		GetLibs(isRelease, proj.dependencies, libs)
		links(libs)
	end
	
	-- Lib dirs
	libDirectories = {}
	GetLibDirsForDependencies(proj.dependencies, libDirectories)
	
	-- Platform specific
	for i=1, #premakeDef.workspace.architectures do
		arch = premakeDef.workspace.architectures[i]
		
        filter { "platforms:"..arch }
		targetdir("../builds/bin/"..projName.."/".._ACTION)
		debugdir("../bin/")
		libdirs(libDirectories)		
    end
end

-- CreateProjects
-- Generates premake code for all projects
function CreateProjects()	
	for projName, proj in pairs(premakeDef.projects) do
		CreateProject(projName, proj.kind, proj)
	end
end

-- AddLibraryProjectDepends
-- Adds dependencies for all library projects
function AddLibraryProjectDepends()
	for projName, proj in pairs(premakeDef.projects) do
		if proj.kind == "SharedLib" or proj.kind == "StaticLib" then
			premakeDef.dependencies[projName] = {
				includeDirs = { "../src/"..projName },
				libraryDirs = { "../builds/bin/"..projName.."/".._ACTION },
				dependencies = proj.dependencies
			}
			premakeDef.dependencies[projName]["debugLibs"] = { projName.."_d" }
			premakeDef.dependencies[projName]["releaseLibs"] = { projName }
		end
	end
end

-- RunPremake
-- Generates the premake code to generate the workspace
function RunPremake()
	if _ACTION == "clean" then
		os.rmdir("../workspaces")
		os.rmdir("../builds")
	  
	  -- Remove GeneratedFiles folder from Qt projects
		for projName, proj in pairs(premakeDef.projects) do
			if proj.kind == "Qt" then
				os.rmdir("../src/"..projName.."/GeneratedFiles")
			end
		end
	  
	  os.exit()
	else
		AddLibraryProjectDepends()
		
		workspace(premakeDef.workspace.name)
		startproject(premakeDef.workspace.startupProject)
		filename(premakeDef.workspace.name)
		location("../workspaces/"..premakeDef.workspace.name.."/".._ACTION)
		flags(premakeDef.workspace.flags)
		defines(premakeDef.workspace.defines)
		configurations(GetConfigurationNames())
		
		if (_ACTION == "gmake") then
			toolset "clang"
		end
		
		if (premakeDef.workspace.characterSet ~= nil) then
			characterset(premakeDef.workspace.characterSet)
		end
		
		CreatePlatforms()
		CreateConfigurations()
		CreateProjects()
	end
end
