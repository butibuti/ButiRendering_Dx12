#ifndef STDAFX_H
#define STDAFX_H
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "comctl32.lib" )
#include <memory.h>
#include <tchar.h>
#include <wrl.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include<ctime>
#include<chrono>
// STL
#include <vector>
#include <list>
#include <map>
#include<unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include"ButiMath/ButiMath.h"
#include"ButiMath/Transform.h"
#include "Common.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ButiUtil/ButiUtil/Helper/StringHelper.h"
#include"ButiUtil/ButiUtil/BinaryObject.h"
#include"ButiRendering_Dx12/Header/RenderingStatus.h"
#include"ButiRendering_Dx12/Header/ResourceInterface.h"
#include"ButiRendering_Dx12/Header/GraphicDevice.h"
#include"ButiRendering_Dx12/Header/ShaderVariable.h"
#include"ButiRendering_Dx12/Header/CBuffer.h"
#include"ButiRendering_Dx12/Header/Camera.h"

#endif //STDAFX_H
