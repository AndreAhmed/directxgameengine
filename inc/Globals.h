#pragma once


#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define DeleteObjectPointer(object) if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if((objects) != NULL) { delete[] objects; objects = NULL; }

// INCLUDES ///////////////////////////////////////////////

#include <windows.h>   // include important windows stuff
#include <WindowsX.h>
#include <tchar.h>
#include <sstream>
#include <fstream>

#include <wrl.h>
// DirectX includes

#include <d3dcommon.h>
#include <d3d11_1.h>
#include <directxcolors.h>
#include <d3dcompiler.h>
#include <directxmath.h>

// STL
#include <vector>
#include <memory>