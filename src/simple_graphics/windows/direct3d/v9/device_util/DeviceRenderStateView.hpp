#pragma once
#ifndef SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__DEVICE_UTIL__DEVICERENDERSTATEVIEW_HPP
#define SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__DEVICE_UTIL__DEVICERENDERSTATEVIEW_HPP

#include "d3d9.h"
#include "d3dx9.h"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include <utility>

namespace simple_graphics
{
	namespace windows
	{
		namespace direct3d
		{
			namespace v9
			{
				namespace device_util
				{
					class DeviceRenderStateView final
					{
					private:
						IDirect3DDevice9 *const device;
						//不使用无参构造函数
						//声明在private访问权限区并仅声明不定义
						//且标识为删除（delete）的函数
						DeviceRenderStateView() = delete;
					public:
						//一般调用的构造函数
						inline DeviceRenderStateView(
							IDirect3DDevice9 *device
						) :device(device) {}
						//复制（拷贝）构造函数
						inline DeviceRenderStateView(
							const DeviceRenderStateView &deviceRenderStateView
						) : device(deviceRenderStateView.device) {}
						//移动构造函数（一般不会使用）
						inline DeviceRenderStateView(
							DeviceRenderStateView &&deviceRenderStateView
						) : device(deviceRenderStateView.device)
						{
							const_cast<IDirect3DDevice9 *>(
								deviceRenderStateView.device
								) = nullptr;
						}
					private:
						inline std::pair<DWORD, HRESULT>
							getRenderState(const D3DRENDERSTATETYPE &stateType) const
						{
							DWORD state = 0;
							HRESULT result = device->GetRenderState(stateType, &state);
							return std::pair<DWORD, HRESULT>(state, result);
						}
						inline HRESULT setRenderState(
							const D3DRENDERSTATETYPE &stateType,
							const DWORD &state
						) const
						{
							return device->SetRenderState(stateType, state);
						}
					public:
						enum ShadeMode
						{
							ShadeMode_FLAT = D3DSHADE_FLAT,
							ShadeMode_GOURAUD = D3DSHADE_GOURAUD,
							ShadeMode_PHONG = D3DSHADE_PHONG
						};
						inline enum ShadeMode getShadeMode() const
						{
							DWORD shadeMode;
							device->GetRenderState(D3DRS_SHADEMODE, &shadeMode);
							switch (shadeMode)
							{
							case D3DSHADE_FLAT:
								return ShadeMode_FLAT;
							case D3DSHADE_GOURAUD:
								return ShadeMode_GOURAUD;
							case D3DSHADE_PHONG:
								return ShadeMode_PHONG;
							}
						}
						inline HRESULT setShadeMode(
							const enum ShadeMode &shadeMode
						) const
						{
							switch (shadeMode)
							{
							case ShadeMode_FLAT:
								return device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
							case ShadeMode_GOURAUD:
								return device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
							case ShadeMode_PHONG:
								return device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);
							default:
								return device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
							}
						}

						//设置默认环境光，默认使用白光
						//以使不启用光照处理时各表面均反射期待的颜色
						//这里返回该类的引用，以在其后链式调用processLights()
						inline const DeviceRenderStateView &setDefaultAmbient(
							const D3DCOLOR &color = D3DCOLOR_XRGB(0xff, 0xff, 0xff)
						) const
						{
							device->SetRenderState(D3DRS_AMBIENT, color);
							return *this;
						}

						//处理光源
						//注意：启用光源处理会禁止默认环境光，
						//即本类中setDefaultAmbient所设置的环境光会失效
						inline bool processLights(bool enabled = true) const
						{
							DWORD lastState;
							device->GetRenderState(D3DRS_LIGHTING, &lastState);
							if (enabled) device->SetRenderState(D3DRS_LIGHTING, TRUE);
							else device->SetRenderState(D3DRS_LIGHTING, FALSE);
							return lastState? true : false;
						}

						//Direct3D 9.0默认不启用镜面光
						inline bool processSpecular(bool enabled = true) const
						{
							DWORD lastState;
							device->GetRenderState(D3DRS_SPECULARENABLE, &lastState);
							if (enabled) device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
							else device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
							return lastState ? true : false;
						}

						inline bool normalizeNormalVectors(bool enabled = true) const
						{
							DWORD lastState;
							device->GetRenderState(D3DRS_NORMALIZENORMALS, &lastState);
							if (enabled) device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
							else device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
							return lastState ? true : false;
						}
					};
				}
			}
		}
	}
}
#endif // !SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__DEVICE_UTIL__DEVICERENDERSTATEVIEW_HPP
