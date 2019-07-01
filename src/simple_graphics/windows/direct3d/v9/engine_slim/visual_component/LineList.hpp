#pragma once
#include "../Window.hpp"
#include "../../device_util/device_util.h"
#include "DisplayObject.hpp"

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
					namespace visual_component
					{
						class LineList :public DisplayObject
						{
						public:
							struct Vertex
							{
								float x, y, z;
								D3DCOLOR color;
								static const DWORD format = D3DFVF_XYZ | D3DFVF_DIFFUSE;
							};


							inline LineList(
								const Window &window,
								const std::vector<std::pair<Vertex, Vertex>> &lines,
								const Vector &relativePosition = Vector(0, 0, 0),
								const Vector &relativeRotation = Vector(0, 0, 0),
								const Vector &relativeScale = Vector(1, 1, 1)
							) :
								DisplayObject(
									relativePosition,
									relativeRotation,
									relativeScale
								), window(window)
							{
								this->vertexBuffer = NULL;//在初始阶段把这个设为0
								this->lines = lines;
							}

							//这个析构函数的主要作用是释放VertexBuffer的内存
							~LineList()
							{
								if (this->vertexBuffer != NULL)
								{
									vertexBuffer->Release();//释放
								}
							}

							inline void render(
								const Vector &absolutePosition = Vector(0, 0, 0),
								const Vector &absoluteRotation = Vector(0, 0, 0),
								const Vector &absoluteScale = Vector(1, 1, 1)) override
							{
								//这部分才是最难的，因为要根据绝对位置、绝对旋转和绝对缩放
								//计算顶点坐标
								//缩放应当根据中心，为了简单，在本程序中不使用锚点anchor的概念
								std::vector<std::pair<Vertex, Vertex>> vertexToRender = this->lines;
								//选择新建一个而不是改变原来的是为了可以多次调用
								for (std::vector<std::pair<Vertex, Vertex>>::iterator lineIterator = vertexToRender.begin();
									lineIterator != vertexToRender.end(); lineIterator++
									)
								{
									//首先先缩放，再旋转，再平移，否则行为就反常了
									//1.缩放
									lineIterator->first.x *= static_cast<float>(absoluteScale.getX());
									lineIterator->first.y *= static_cast<float>(absoluteScale.getY());
									lineIterator->first.z *= static_cast<float>(absoluteScale.getZ());
									lineIterator->second.x *= static_cast<float>(absoluteScale.getX());
									lineIterator->second.y *= static_cast<float>(absoluteScale.getY());
									lineIterator->second.z *= static_cast<float>(absoluteScale.getZ());
									//2.旋转
									//emmmmm旋转的话…………可以调用Vector的rotate方法去做
									//直接转顶点就可以了
									Vector temp;
									temp = Vector(
										lineIterator->first.x,
										lineIterator->first.y,
										lineIterator->first.z
									).rotate(absoluteRotation);
									lineIterator->first.x = static_cast<float>(temp.getX());
									lineIterator->first.y = static_cast<float>(temp.getY());
									lineIterator->first.z = static_cast<float>(temp.getZ());

									temp = Vector(
										lineIterator->second.x,
										lineIterator->second.y,
										lineIterator->second.z
									).rotate(absoluteRotation);
									lineIterator->second.x = static_cast<float>(temp.getX());
									lineIterator->second.y = static_cast<float>(temp.getY());
									lineIterator->second.z = static_cast<float>(temp.getZ());
									//3.平移
									temp = Vector(
										lineIterator->first.x,
										lineIterator->first.y,
										lineIterator->first.z
									) + absolutePosition;
									lineIterator->first.x = static_cast<float>(temp.getX());
									lineIterator->first.y = static_cast<float>(temp.getY());
									lineIterator->first.z = static_cast<float>(temp.getZ());

									temp = Vector(
										lineIterator->second.x,
										lineIterator->second.y,
										lineIterator->second.z
									) + absolutePosition;
									lineIterator->second.x = static_cast<float>(temp.getX());
									lineIterator->second.y = static_cast<float>(temp.getY());
									lineIterator->second.z = static_cast<float>(temp.getZ());

								}

								//如果vertexBuffer指针不为NULL，一定要释放，以避免内存泄漏
								if (this->vertexBuffer != NULL) this->vertexBuffer->Release();

								//创建VertexBuffer
								this->window.getDevice()->CreateVertexBuffer(
									2 * vertexToRender.size() * sizeof(Vertex),
									D3DUSAGE_WRITEONLY, Vertex::format,
									D3DPOOL_MANAGED, &(this->vertexBuffer), 0
								);

								Vertex *vertexPointer = NULL;
								this->vertexBuffer->Lock(0, 0, (void **)&vertexPointer, 0);

								UINT index = 0;
								for (std::vector<std::pair<Vertex, Vertex>>::iterator lineIterator = vertexToRender.begin();
									lineIterator != vertexToRender.end(); lineIterator++)
								{
									*(vertexPointer + index) = lineIterator->first;
									++index;
									*(vertexPointer + index) = lineIterator->second;
									++index;
									//当然如果换一种写法只需要两行，但是现在我还不知道能不能跑起来。
								}
								this->vertexBuffer->Unlock();

								//在渲染灵活顶点格式为(D3DFVF_XYZ|D3DFVF_DIFFUSE)的图元时
								//要关闭光源处理，以根据顶点中所定义的颜色进行渲染
								//否则其颜色由外部环境光(Ambient)确定
								using device_util::DeviceRenderStateView;
								DeviceRenderStateView deviceRenderStateView(this->window.getDevice());
								deviceRenderStateView.processLights(false);//关闭光源处理
								this->window.getDevice()->SetStreamSource(0, this->vertexBuffer, 0, sizeof(Vertex));
								this->window.getDevice()->SetFVF(Vertex::format);//设置灵活顶点格式
								this->window.getDevice()->DrawPrimitive(D3DPT_LINELIST, 0, vertexToRender.size());
								deviceRenderStateView.processLights(true);//恢复光源处理
								//绘制图元
							}

						private:
							const Window window;
							IDirect3DVertexBuffer9 *vertexBuffer;
							std::vector<std::pair<Vertex, Vertex>> lines;
						};
					}
				}
			}
		}
	}
}