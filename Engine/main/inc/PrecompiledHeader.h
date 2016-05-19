#pragma once

#include <json.hpp>
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>
#include <set>
#include <iostream>

using json = nlohmann::json;

using std::shared_ptr;
using std::vector;
using std::queue;
using std::string;
using std::wstring;
using std::function;
using std::unordered_map;
using std::set;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include <ITPMemory.h>
#include <singleton.h>
#include <StringUtils.h>
#include <dbg_assert.h>
#include <itpmath.h>
#include <Timing.h>

#include <GraphicsDriver.h>

#include <Camera.h>
#include <Mesh.h>
#include <Material.h>
#include <Cube.h>
#include <ObjMesh.h>

#include <InputManager.h>
#include <MessageManager.h>
#include <Renderer.h>
