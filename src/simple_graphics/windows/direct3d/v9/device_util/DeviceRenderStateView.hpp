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
						//��ʹ���޲ι��캯��
						//������private����Ȩ������������������
						//�ұ�ʶΪɾ����delete���ĺ���
						DeviceRenderStateView() = delete;
					public:
						//һ����õĹ��캯��
						inline DeviceRenderStateView(
							IDirect3DDevice9 *device
						) :device(device) {}
						//���ƣ����������캯��
						inline DeviceRenderStateView(
							const DeviceRenderStateView &deviceRenderStateView
						) : device(deviceRenderStateView.device) {}
						//�ƶ����캯����һ�㲻��ʹ�ã�
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

						//����Ĭ�ϻ����⣬Ĭ��ʹ�ð׹�
						//��ʹ�����ù��մ���ʱ������������ڴ�����ɫ
						//���ﷵ�ظ�������ã����������ʽ����processLights()
						inline const DeviceRenderStateView &setDefaultAmbient(
							const D3DCOLOR &color = D3DCOLOR_XRGB(0xff, 0xff, 0xff)
						) const
						{
							device->SetRenderState(D3DRS_AMBIENT, color);
							return *this;
						}

						//�����Դ
						//ע�⣺���ù�Դ������ֹĬ�ϻ����⣬
						//��������setDefaultAmbient�����õĻ������ʧЧ
						inline bool processLights(bool enabled = true) const
						{
							DWORD lastState;
							device->GetRenderState(D3DRS_LIGHTING, &lastState);
							if (enabled) device->SetRenderState(D3DRS_LIGHTING, TRUE);
							else device->SetRenderState(D3DRS_LIGHTING, FALSE);
							return lastState? true : false;
						}

						//Direct3D 9.0Ĭ�ϲ����þ����
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
