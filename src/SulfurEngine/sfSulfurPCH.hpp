#pragma once

#define NOMINMAX
#include <Windows.h>
#include <comdef.h>
#include <fileapi.h>
#include <string>
#include <iostream>
#include <functional>
#include <type_traits>
#include <utility>

// STL
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
// Graphics API
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

// Engine
#include "Types/sfTypes.hpp"
#include "Settings/sfEngineSettings.hpp"
#include "Logger/sfLogger.hpp"
#include "Error/sfError.hpp"