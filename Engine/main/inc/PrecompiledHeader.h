#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <set>

using std::shared_ptr;
using std::vector;
using std::string;
using std::wstring;
using std::function;
using std::unordered_map;
using std::set;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include <singleton.h>
#include <StringUtils.h>
#include <dbg_assert.h>
#include <Timing.h>

#include <GraphicsDriver.h>

#include <App.h>