#pragma once
#include "../Window.hpp"
#include "../../device_util/device_util.h"
#include "DisplayObject.hpp"

#include "../../engine_util/converter/TextConverter.hpp"

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
						//文本类
						class Text :public DisplayObject
						{
						public:
							//这个Box类类似于
							//平面设计软件或office套件中常见的“文本框”概念
							//在这里用于指明文本的边界
							//因为在Direct3D 9.0中，文本以Mesh的形式存在，
							//Mesh是若干个顶点的集合
							//顶点Vertex除了位置信息以外还有法向量（用于表示光照和反射等）
							//所以不能保证对Mesh进行非比例缩放后不会产生不被期待的效果
							//而为了引擎的统一，Text类必须继承DisplayObject基类
							//所以外部调用render方法时传递的
							//absolutePosition,absoluteRotation,absoluteScale参数
							//对应的都是这个Box的绝对位置，绝对旋转和（各方向的）绝对缩放
							//而最终渲染的文本将由上述三个参数结合Box的尺寸决定
							//渲染的工作仍由重写（override）的render()方法负责
							class Box
							{
							private:
								const Vector size;//盒子的尺寸
							public:
								Box(const Vector &size) :size(size) {}

								//getter methods
								const Vector getSize() const
								{
									return size;
								}
							};

							//以下是Text类的成员部分;
						private:
							Window window;
							Box textBox;
							std::string content, fontName;
							int weight;
							ID3DXMesh *d3dxMesh;
							struct Vertex
							{
								float x, y, z;
								float nx, ny, nz;
								//指明灵活顶点格式
								static const DWORD format = D3DFVF_XYZ | D3DFVF_NORMAL;
								//DWORD format = D3DFVF_XYZ | D3DFVF_NORMAL对应的顶点格式
								//之所以知道从Text中获取的是这样的顶点格式，
								//就是调试输出了18也就是0x12，所以就是D3DFVF_XYZ | D3DFVF_NORMAL
								//另外，查阅 https://docs.microsoft.com/zh-cn/windows/desktop/direct3d9/d3dxcreatetext
								//可以知道该函数产生的Mesh是这样的灵活顶点格式
								
							};
						public:
							inline void render(
								const Vector &absolutePosition = Vector(0, 0, 0),
								const Vector &absoluteRotation = Vector(0, 0, 0),
								const Vector &absoluteScale = Vector(1, 1, 1)
							) override
							{
								//0. 如果不是NULL那就说明曾经被初始化过，要释放
								if (this->d3dxMesh != NULL) d3dxMesh->Release();
								//1. 首先创建文本Mesh
								using simple_graphics::windows::direct3d::v9::engine_util::converter::TextConverter;

								LPWSTR font;
								TextConverter::stringToWideChar(this->fontName, font);

								//我讨厌这些原始的句柄
								struct {
									HFONT original, current;
								} hFonts;

								//前两个参数的含义：
								//nHeight：指定所要求字体的字符单元或字符的逻辑单位高度。  
								//nWidth：指定所要求字体的字符逻辑单位的平均宽度。
								//如果此值为0，字体映射器选择一个closest match值，
								//closest match值是由比较当前设备的特征系数
								//与可使用字体的数字化特征系数之差的绝对值而确定的。
								hFonts.current = CreateFont(
									10, 10, 0, 0,
									this->weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, static_cast<LPCWSTR>(font));
								delete[] font;//这个是临时变量，用完要删除

								LPWSTR tempContent;
								TextConverter::stringToWideChar(this->content, tempContent);


								//创建字体
								HDC hdc = CreateCompatibleDC(0);//创建环境
								//将字体选入设备环境,返回上一个字体，以备重新选择
								//hFonts.original = (HFONT)SelectObject(hdc, hFonts.current);
								hFonts.original = static_cast<HFONT>(SelectObject(hdc, hFonts.current));
																										
								D3DXCreateText(this->window.getDevice(), hdc, static_cast<LPCWSTR>(tempContent),
									0.001f,//0.001这是最小值了  再小就出Bug了，这都是多次尝试得出的
									(0.001f*0.001f*0.001f*0.001f*0.001f*0.001f*0.001f*0.001f),
									&this->d3dxMesh, NULL, NULL);//创建文本网格
								SelectObject(hdc, hFonts.original);//选择原来的字体
								DeleteObject(hFonts.current);//删除新创建的字体
								DeleteDC(hdc);

								delete[] tempContent;

								struct { Vector min, max; } meshBound;
								struct Vertex *currentVertexPointer = NULL;
								//为什么要这样强制转换
								this->d3dxMesh->LockVertexBuffer(0, reinterpret_cast<void **>(&currentVertexPointer));
								meshBound.min = Vector(
									currentVertexPointer[0].x,
									currentVertexPointer[0].y, 
									currentVertexPointer[0].z
								);
								meshBound.max = Vector(
									currentVertexPointer[0].x,
									currentVertexPointer[0].y,
									currentVertexPointer[0].z
								);
								//遍历Mesh中所有的顶点
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									//更新上界
									if (currentVertexPointer[i].x > meshBound.max.getX())
										meshBound.max.setX(currentVertexPointer[i].x);
									if (currentVertexPointer[i].y > meshBound.max.getY())
										meshBound.max.setY(currentVertexPointer[i].y);
									if (currentVertexPointer[i].z > meshBound.max.getZ())
										meshBound.max.setZ(currentVertexPointer[i].z);
									//更新下界
									if (currentVertexPointer[i].x < meshBound.min.getX())
										meshBound.min.setX(currentVertexPointer[i].x);
									if (currentVertexPointer[i].y < meshBound.min.getY())
										meshBound.min.setY(currentVertexPointer[i].y);
									if (currentVertexPointer[i].z < meshBound.min.getZ())
										meshBound.min.setZ(currentVertexPointer[i].z);
								}
								Vector center(
									(meshBound.min.getX() + meshBound.max.getX()) / 2,
									(meshBound.min.getY() + meshBound.max.getY()) / 2,
									(meshBound.min.getZ() + meshBound.max.getZ()) / 2
								);
								//首先移动到中心
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									currentVertexPointer[i].x -= static_cast<float>(center.getX());
									currentVertexPointer[i].y -= static_cast<float>(center.getY());
									currentVertexPointer[i].z -= static_cast<float>(center.getZ());
								}
								center.set(0, 0, 0);//已经平移到了中心就要将中心设为0了

								Vector currentMeshSize(
									meshBound.max.getX() - meshBound.min.getX(),
									meshBound.max.getY() - meshBound.min.getY(),
									meshBound.max.getZ() - meshBound.min.getZ()
								);
								Vector allowedMeshSize = this->textBox.getSize() * absoluteScale;
								
								//不管是要放大还是缩小，都要选择
								//allowedMeshSize/currentMeshSiz向量三个分量中最小的一个进行等比例缩放，
								//才能保证缩放后的尺寸不会有任何一个分量超出allowedMeshSize
								double scaleRatio = allowedMeshSize.getX() / currentMeshSize.getX();
								if (allowedMeshSize.getY() / currentMeshSize.getY() < scaleRatio)
									scaleRatio = allowedMeshSize.getY() / currentMeshSize.getY();
								if (allowedMeshSize.getZ() / currentMeshSize.getZ() < scaleRatio)
									scaleRatio = allowedMeshSize.getZ() / currentMeshSize.getZ();

								//遍历所有顶点，进行缩放的处理
								//法向量在缩放的操作中不需要改变，但是在接下来旋转的操作中要改变
								//先旋转，再平移到要求的位置
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									currentVertexPointer[i].x *= static_cast<float>(scaleRatio);
									currentVertexPointer[i].y *= static_cast<float>(scaleRatio);
									currentVertexPointer[i].z *= static_cast<float>(scaleRatio);
								}
								//这次遍历中进行的是旋转的操作
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									Vector currentVertexPosition(
										currentVertexPointer[i].x,
										currentVertexPointer[i].y,
										currentVertexPointer[i].z
									);
									Vector currentVertexNormalVector(
										currentVertexPointer[i].nx,
										currentVertexPointer[i].ny,
										currentVertexPointer[i].nz
									);
									//进行旋转
									currentVertexPosition.rotate(absoluteRotation);
									currentVertexNormalVector.rotate(absoluteRotation);

									//将旋转后的结果写回顶点缓存
									currentVertexPointer[i].x = 
										static_cast<float>(currentVertexPosition.getX());
									currentVertexPointer[i].y = 
										static_cast<float>(currentVertexPosition.getY());
									currentVertexPointer[i].z = 
										static_cast<float>(currentVertexPosition.getZ());
									currentVertexPointer[i].nx = 
										static_cast<float>(currentVertexNormalVector.getX());
									currentVertexPointer[i].ny =
										static_cast<float>(currentVertexNormalVector.getY());
									currentVertexPointer[i].nz =
										static_cast<float>(currentVertexNormalVector.getZ());
								}
								//这次遍历中进行的是平移的操作
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									currentVertexPointer[i].x += 
										static_cast<float>(absolutePosition.getX());
									currentVertexPointer[i].y += 
										static_cast<float>(absolutePosition.getY());
									currentVertexPointer[i].z += 
										static_cast<float>(absolutePosition.getZ());
								}
								this->d3dxMesh->UnlockVertexBuffer();

								//文本网格的灵活顶点格式包含位置和法向量
								//在绘制前确保光源处理已打开，以根据光源及其材质确定颜色
								device_util::DeviceRenderStateView(
									this->window.getDevice()
								).processLights(true);
								this->d3dxMesh->DrawSubset(0);
							}

							Text(const Window &window, const Box &box,
								const std::string &content,
								const std::string &fontName = std::string("Microsoft YaHei"),
								const int &weight = 200,
								const Vector &relativePosition = Vector(0, 0, 0),
								const Vector &relativeRotation = Vector(0, 0, 0),
								const Vector &relativeScale = Vector(1, 1, 1)
							):
								DisplayObject(relativePosition,relativeRotation,relativeScale),
								window(window), textBox(box),content(content), fontName(fontName),
								weight(weight),d3dxMesh(NULL)//初始的时候肯定要把指针设定为NULL
							{}

							~Text()
							{
								if (this->d3dxMesh != NULL) d3dxMesh->Release();
							}
						};
					}
				}
			}
		}
	}
}