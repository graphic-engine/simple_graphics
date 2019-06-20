#pragma once


#include "../Window.hpp"
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
								const Vector getSize() const;
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

								//todo
								///////////////////////////////////////////////
								/*
								//2. 变形到原点并适配文本盒
								struct Text::Vertex *currentVertexPointer = NULL;
								//为什么要这样强制转换
								this->d3dxMesh->LockVertexBuffer(0, reinterpret_cast<void **>(&currentVertexPointer));
								struct { Vector min, max; } bound;
								bound.min = Vector(
									currentVertexPointer[0].x,
									currentVertexPointer[0].y,
									currentVertexPointer[0].z
								);
								bound.max = Vector(
									currentVertexPointer[0].x,
									currentVertexPointer[0].y,
									currentVertexPointer[0].z
								);
								//遍历所有的顶点找到边界，以移动到中心
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									//更新上界
									if (currentVertexPointer[i].x > bound.max.getX())
										bound.max.setX(currentVertexPointer[i].x);
									if (currentVertexPointer[i].y > bound.max.getY())
										bound.max.setY(currentVertexPointer[i].y);
									if (currentVertexPointer[i].z > bound.max.getZ())
										bound.max.setZ(currentVertexPointer[i].z);
									//更新下界
									if (currentVertexPointer[i].x < bound.min.getX())
										bound.min.setX(currentVertexPointer[i].x);
									if (currentVertexPointer[i].y < bound.min.getY())
										bound.min.setY(currentVertexPointer[i].y);
									if (currentVertexPointer[i].z < bound.min.getZ())
										bound.min.setZ(currentVertexPointer[i].z);
								}
								//
								Vector center(
									(bound.min.getX() + bound.max.getX()) / 2,
									(bound.min.getY() + bound.max.getY()) / 2,
									(bound.min.getZ() + bound.max.getZ()) / 2
								);
								//首先移动到中心
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									currentVertexPointer[i].x -= static_cast<float>(center.getX());
									currentVertexPointer[i].y -= static_cast<float>(center.getY());
									currentVertexPointer[i].z -= static_cast<float>(center.getZ());
								}
								center.set(0, 0, 0);//已经平移到了中心就要将中心设为0了
													//然后再缩放文字到box的大小
								double scaleTemp[3];
								scaleTemp[0] = this->textBox.getSize().getX() / (bound.max.getX() - bound.min.getX());
								scaleTemp[1] = this->textBox.getSize().getY() / (bound.max.getY() - bound.min.getY());
								scaleTemp[2] = this->textBox.getSize().getZ() / (bound.max.getZ() - bound.min.getZ());
								//选择最小的一个作为缩放倍数
								double scale = scaleTemp[0];
								if (scaleTemp[1] < scale) scale = scaleTemp[1];
								if (scaleTemp[2] < scale) scale = scaleTemp[2];
								//进行缩放
								//因为是各个方向同倍数进行缩放，所以应该不用改变法向量

								//啊啊啊啊啊啊啊啊啊啊啊我真的好想谈恋爱啊
								// 为什么没有妹子喜欢我
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									currentVertexPointer[i].x *= static_cast<float>(scale);
									currentVertexPointer[i].y *= static_cast<float>(scale);
									currentVertexPointer[i].z *= static_cast<float>(scale);
								}
								//完成缩放了，这样也就可以锁定准备给render方法进行渲染了
								//所以这个函数执行一次就要把所有的顶点遍历三次，这是不可取的
								this->d3dxMesh->UnlockVertexBuffer();
								*/
								///////////////////////////////////////////////
								//todo
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
						private:
							inline void initializeMesh()
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

								//这两个10是什么意思来着，不记得了
								hFonts.current = CreateFont(
									10, 10, 0, 0,
									this->weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, static_cast<LPCWSTR>(font));
								delete[] font;//这个是临时变量，用完要删除

								LPWSTR tempContent;
								TextConverter::stringToWideChar(this->content, tempContent);


								//创建字体
								HDC hdc = CreateCompatibleDC(0);//创建环境
																//hFonts.original = (HFONT)SelectObject(hdc, hFonts.current);
								hFonts.original = static_cast<HFONT>(SelectObject(hdc, hFonts.current));//C++形式转换
																										//将字体选入设备环境,返回上一个字体，以备重新选择
								D3DXCreateText(this->window.getDevice(), hdc, static_cast<LPCWSTR>(tempContent),
									0.001f,//0.001这是最小值了  再小就出Bug了，这都是血的经验试出来的
									(0.001f*0.001f*0.001f*0.001f*0.001f*0.001f*0.001f*0.001f),
									&this->d3dxMesh, NULL, NULL);//创建文本网格
								SelectObject(hdc, hFonts.original);//选择原来的字体
								DeleteObject(hFonts.current);//删除新创建的字体
															 //上面这两条命令是不是可以合并为同一个
								DeleteDC(hdc);

								delete[] tempContent;

								//2. 变形到原点并适配文本盒
								struct Text::Vertex *currentVertexPointer = NULL;
								//为什么要这样强制转换
								this->d3dxMesh->LockVertexBuffer(0, reinterpret_cast<void **>(&currentVertexPointer));
								struct { Vector min, max; } bound;
								bound.min = Vector(
									currentVertexPointer[0].x,
									currentVertexPointer[0].y,
									currentVertexPointer[0].z
								);
								bound.max = Vector(
									currentVertexPointer[0].x,
									currentVertexPointer[0].y,
									currentVertexPointer[0].z
								);
								//遍历所有的顶点找到边界，以移动到中心
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									//更新上界
									if (currentVertexPointer[i].x > bound.max.getX())
										bound.max.setX(currentVertexPointer[i].x);
									if (currentVertexPointer[i].y > bound.max.getY())
										bound.max.setY(currentVertexPointer[i].y);
									if (currentVertexPointer[i].z > bound.max.getZ())
										bound.max.setZ(currentVertexPointer[i].z);
									//更新下界
									if (currentVertexPointer[i].x < bound.min.getX())
										bound.min.setX(currentVertexPointer[i].x);
									if (currentVertexPointer[i].y < bound.min.getY())
										bound.min.setY(currentVertexPointer[i].y);
									if (currentVertexPointer[i].z < bound.min.getZ())
										bound.min.setZ(currentVertexPointer[i].z);
								}
								//
								Vector center(
									(bound.min.getX() + bound.max.getX()) / 2,
									(bound.min.getY() + bound.max.getY()) / 2,
									(bound.min.getZ() + bound.max.getZ()) / 2
								);
								//首先移动到中心
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									currentVertexPointer[i].x -= static_cast<float>(center.getX());
									currentVertexPointer[i].y -= static_cast<float>(center.getY());
									currentVertexPointer[i].z -= static_cast<float>(center.getZ());
								}
								center.set(0, 0, 0);//已经平移到了中心就要将中心设为0了
													//然后再缩放文字到box的大小
								double scaleTemp[3];
								scaleTemp[0] = this->textBox.getSize().getX() / (bound.max.getX() - bound.min.getX());
								scaleTemp[1] = this->textBox.getSize().getY() / (bound.max.getY() - bound.min.getY());
								scaleTemp[2] = this->textBox.getSize().getZ() / (bound.max.getZ() - bound.min.getZ());
								//选择最小的一个作为缩放倍数
								double scale = scaleTemp[0];
								if (scaleTemp[1] < scale) scale = scaleTemp[1];
								if (scaleTemp[2] < scale) scale = scaleTemp[2];
								//进行缩放
								//因为是各个方向同倍数进行缩放，所以应该不用改变法向量

								//啊啊啊啊啊啊啊啊啊啊啊我真的好想谈恋爱啊
								// 为什么没有妹子喜欢我
								for (DWORD i = 0; i < this->d3dxMesh->GetNumVertices(); ++i)
								{
									currentVertexPointer[i].x *= static_cast<float>(scale);
									currentVertexPointer[i].y *= static_cast<float>(scale);
									currentVertexPointer[i].z *= static_cast<float>(scale);
								}
								//完成缩放了，这样也就可以锁定准备给render方法进行渲染了
								//所以这个函数执行一次就要把所有的顶点遍历三次，这是不可取的
								this->d3dxMesh->UnlockVertexBuffer();
							}
						};
					}
				}
			}
		}
	}
}