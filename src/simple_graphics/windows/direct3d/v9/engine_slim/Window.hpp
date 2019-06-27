#pragma once
#ifndef SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__WINDOW_HPP
#define SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__WINDOW_HPP

#include "d3d9.h"
#include "d3dx9.h"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include <Windows.h>
#include <string>
#include <functional>


#include "../engine_util/Vector.hpp"
using simple_graphics::windows::direct3d::v9::engine_util::Vector;

#include "../engine_util/converter/TextConverter.hpp"

namespace simple_graphics
{
	namespace windows
	{
		namespace direct3d
		{
			namespace v9
			{
				namespace engine_slim
				{
					class Window
					{
					private:
						HINSTANCE hInstance;
						HWND hwnd;
						int width, height;
						IDirect3DDevice9 *device;//Direct3D 设备
					public:
						class Config
						{
						public:
							LRESULT(CALLBACK *const windowProc)(
								HWND hwnd,
								UINT uMessage,
								WPARAM wParam,
								LPARAM lParam
								);

							explicit inline Config(
								LRESULT(CALLBACK *const windowProc)(
									HWND hwnd,
									UINT uMessage,
									WPARAM wParam,
									LPARAM lParam
									) = nullptr
							) :windowProc(windowProc)
							{
							}
						};

						//拷贝构造函数
						inline Window(const Window &window)
						{
							this->hInstance = window.getHInstance();
							this->hwnd = window.getHWND();
							this->device = window.getDevice();
							window.getSize(this->width, this->height);
						}

						//移动构造函数
						inline Window(Window &&window)
						{
							this->hInstance = window.getHInstance();
							this->hwnd = window.getHWND();
							this->device = window.getDevice();
							window.getSize(this->width, this->height);
						}


						inline void clearAndBegin(
							const unsigned char R = 0x7f,
							const unsigned char G = 0x7f,
							const unsigned char B = 0x7f
						) const
						{
							/*
							深度缓存，也称作z-buffer，是用于渲染的重要缓存。
							深度缓存不存储像素的颜色，而代之以像素的深度信息。
							深度缓存中的深度信息有多种不同的形式，但它们基本上都反映物体到摄像机的距离。
							实践中通常保存的都是裁剪空间的z坐标，这就是z-buffer名称的由来。

							深度缓存一般用于计算物体之间的遮挡，
							当光栅化三角形时，计算各像素的插值深度。
							在渲染像素之前，将这个深度值和深度缓存中该像素的深度值比较，
							如果新的深度比现有值离摄像机更远，则新的像素被丢弃；
							否则像素颜色被写到帧缓存，并用新的更近的值更新深度缓存。
							
							在开始进行新的渲染之前，记得要置z-buffer各值为无限远
							（在裁剪空间中，这个值为1.0），这样第一批像素才能通过深度测试，
							*/
							//这个函数调用中的1.0f的含义见上面的注释
							this->getDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
								D3DCOLOR_XRGB(R, G, B), 1.0f, 0
							);
							//https://docs.microsoft.com/zh-cn/windows/desktop/api/d3d9/nf-d3d9-idirect3ddevice9-clear
							/*
							IDirect3DDevice9::Clear will fail if you:

							Try to clear either the depth buffer or the stencil buffer of a render target that does not have an attached depth buffer.
							Try to clear the stencil buffer when the depth buffer does not contain stencil data.
							*/
							this->getDevice()->BeginScene();
						}
						inline void endAndPresent() const
						{
							this->getDevice()->EndScene();
							//将后台缓冲区的内容投射上前台
							this->getDevice()->Present(0, 0, 0, 0);
						}
						inline void render(
							const std::function<void(
								const Window &, const double&, const double &
								)> &displayFunction
						) const
						{
							MSG message;
							ZeroMemory(&message, sizeof(MSG));

							double appStartTime = (double)timeGetTime();
							double lastDisplayFunctionEnterTime = appStartTime;

							while (message.message != WM_QUIT)
							{
								if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
								{
									TranslateMessage(&message);
									DispatchMessage(&message);
								}
								else
								{
									double currentDisplayFunctionEnterTime = (double)timeGetTime();
									displayFunction(
										*this,
										currentDisplayFunctionEnterTime - appStartTime,
										currentDisplayFunctionEnterTime - lastDisplayFunctionEnterTime
									);
									lastDisplayFunctionEnterTime = currentDisplayFunctionEnterTime;
								}
							}
						}

						//设置静态相机
						inline Window &setStaticCamera(
							const Vector &position = Vector(0, 0, 0),
							const Vector &target = Vector(0, 0, 1),
							const double &angle = 0.5f*D3DX_PI,
							const double &zMin = 0.0,
							const double &zMax = 1073741824.0
						)
						{
							D3DXMATRIX transformMatrix;
							D3DXVECTOR3 d3dxPosition = D3DXVECTOR3(
								static_cast<float>(position.getX()),
								static_cast<float>(position.getY()),
								static_cast<float>(position.getZ())
							);
							D3DXVECTOR3 d3dxTarget = D3DXVECTOR3(
								static_cast<float>(target.getX()),
								static_cast<float>(target.getY()),
								static_cast<float>(target.getZ())
							);

							D3DXMatrixLookAtLH(&transformMatrix,
								&d3dxPosition, &d3dxTarget,
								&D3DXVECTOR3(0.0f, 1.0f, 0.0f)
							);
							this->device->SetTransform(D3DTS_VIEW, &transformMatrix);

							D3DXMatrixPerspectiveFovLH(&transformMatrix,
								static_cast<float>(angle),
								static_cast<float>(this->width) / static_cast<float>(this->height),
								static_cast<float>(zMin), static_cast<float>(zMax)
							);
							this->device->SetTransform(D3DTS_PROJECTION, &transformMatrix);

							return *this;
						}

						//这个构造函数最复杂，最初是直接抄书上的例程
						inline Window(int width, int height, bool fullScreen = false,
							HINSTANCE hInstance = GetModuleHandle(NULL),
							std::string title = std::string("Direct3D"),
							Config config = Config()
						)
						{
							this->width = width;
							this->height = height;
							this->hInstance = hInstance;

							//设计窗口类
							WNDCLASS windowClass;
							windowClass.style = CS_HREDRAW | CS_VREDRAW;
							//windowClass.lpfnWndProc = (WNDPROC)defaultWindowProc;
							if (config.windowProc != nullptr)
							{
								windowClass.lpfnWndProc = config.windowProc;
							}
							else
							{
								windowClass.lpfnWndProc = defaultWindowProc;
							}
							windowClass.cbClsExtra = 0;
							windowClass.cbWndExtra = 0;
							windowClass.hInstance = hInstance;
							windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
							windowClass.hCursor = LoadCursor(0, IDC_ARROW);
							windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
							windowClass.lpszMenuName = 0;
							windowClass.lpszClassName = TEXT("Direct3D Window");
							//注册窗口类
							if (!RegisterClass(&windowClass))
							{
								MessageBoxA(0, "RegisterClass Failed", 0, 0);
								exit(-1);//非正常退出
							}
							//创建窗口
							//这里的第一个参数必须是上面创建的类名
							//HWND hwnd = CreateWindow(TEXT("Direct3D Window"),TEXT("Direct3D project"),WS_EX_TOPMOST|WS_OVERLAPPEDWINDOW,0,0,width,height,0,0,hInstance,0);
							//这里的改变是为了禁止窗口改变大小
							//this->hwnd = CreateWindow(TEXT("Direct3D Window"),TEXT("Direct3D project"),WS_EX_TOPMOST|(WS_OVERLAPPEDWINDOW^WS_THICKFRAME),0,0,width,height,0,0,hInstance,0);
							//这里的改变是为了外部输入的窗口标题
							using simple_graphics::windows::direct3d::v9::engine_util::converter::TextConverter;

							LPWSTR windowTitle = NULL;
							TextConverter::stringToWideChar(title, windowTitle);
							this->hwnd = CreateWindow(TEXT("Direct3D Window"), windowTitle, WS_EX_TOPMOST | (WS_OVERLAPPEDWINDOW^WS_THICKFRAME), 0, 0, this->width, this->height, 0, 0, hInstance, 0);
							delete[] windowTitle;
							//显示及更新窗口
							if (!this->hwnd)
							{
								MessageBoxA(0, "Direct3D CreateWindow Failed", 0, 0);
								exit(-1);
							}

							ShowWindow(this->hwnd, SW_SHOW);
							UpdateWindow(this->hwnd);

							//创建Direct3D 9接口
							//这是最原始的防止污染全局命名空间的方法
							IDirect3D9 *direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
							if (!direct3D9)
							{
								MessageBoxA(0, "Direct3DCreate9() Failed", 0, 0);
								exit(-1);
							}

							D3DCAPS9 caps;
							direct3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
							//DWORD vertexProcessingByHardwareOrSoftwareFlag = (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)?D3DCREATE_HARDWARE_VERTEXPROCESSING:D3DCREATE_SOFTWARE_VERTEXPROCESSING;
							DWORD vertexProcessingByHardwareOrSoftwareFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
							if (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
								vertexProcessingByHardwareOrSoftwareFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
							else vertexProcessingByHardwareOrSoftwareFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

							//创建参数
							D3DPRESENT_PARAMETERS presentParameters;
							presentParameters.BackBufferWidth = width;
							presentParameters.BackBufferHeight = height;
							presentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
							presentParameters.BackBufferCount = 1;
							//presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
							presentParameters.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;;
							presentParameters.MultiSampleQuality = 0;
							presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
							presentParameters.hDeviceWindow = hwnd;
							presentParameters.Windowed = !fullScreen;
							presentParameters.EnableAutoDepthStencil = true;
							presentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
							presentParameters.Flags = 0;
							presentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
							presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

							//DepthFormat 枚举
							//https://docs.microsoft.com/zh-cn/previous-versions/dotnet/netframework-3.5/ms129550%28v%3dvs.90%29
							//D32	一个 32 位 z 缓冲区位深度。
							//D15S1	对于 16 位 z 缓冲区位深度，其中 15 位是为深度通道保留的，1 位是为模具通道保留的。
							//D24S8	一个包含 24 位深度（使用 24 位浮点格式 - 20E4）和 8 位模具的非锁定格式。
							//D16	16 位 z 缓冲区位深度。
							//D24X8	一个将 24 位用于深度通道的 32 位 z 缓冲区位深度。
							//D24X4S4	一个 32 位 z 缓冲区位深度，其中 24 位用于深度通道，4 位用于模具通道。

							//创建设备
							//todo:在这里是不是可以改成promise那样的回调？？
							//但是又要写一个工具的话是不是问题太大了？

							D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
							//long
							HRESULT hResult = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, this->hwnd, vertexProcessingByHardwareOrSoftwareFlag, &presentParameters, &(this->device));
							if (FAILED(hResult))
							{
								presentParameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
								hResult = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, this->hwnd, vertexProcessingByHardwareOrSoftwareFlag, &presentParameters, &(this->device));
								if (FAILED(hResult))
								{
									presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
									presentParameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
									hResult = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, this->hwnd, vertexProcessingByHardwareOrSoftwareFlag, &presentParameters, &(this->device));
									if (FAILED(hResult))
									{
										presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
										hResult = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, this->hwnd, vertexProcessingByHardwareOrSoftwareFlag, &presentParameters, &(this->device));
										if (FAILED(hResult))
										{
											MessageBoxA(0, "Direct3D9CreateDeviceFailed", 0, 0);
											//释放资源
											direct3D9->Release();
											exit(-1);
										}
									}
								}
							}
							direct3D9->Release();
						}


						//getter methods
						inline HINSTANCE getHInstance() const
						{
							return this->hInstance;
						}
						inline HWND getHWND() const
						{
							return this->hwnd;
						}
						inline IDirect3DDevice9 *getDevice() const
						{
							return this->device;
						}
						inline void getSize(int &width, int &height) const
						{
							width = this->width;
							height = this->height;
						}
					private:
						//窗口处理进程
						static LRESULT CALLBACK defaultWindowProc(
							HWND hwnd,
							UINT uMessage,
							WPARAM wParam,
							LPARAM lParam
						)
						{
							switch (uMessage)
							{
							case WM_DESTROY:
								::PostQuitMessage(0);
								break;
							case WM_KEYDOWN:
								if (wParam == VK_ESCAPE)
									::DestroyWindow(hwnd);
								break;
							}
							return DefWindowProc(hwnd, uMessage, wParam, lParam);
						}
					};
				}
			}
		}
	}
}
#endif // !SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__WINDOW_HPP