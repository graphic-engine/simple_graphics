#pragma once
#ifndef SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__VISUAL_COMPONENT__DISPLAY_OBJECT_HPP
#define SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__VISUAL_COMPONENT__DISPLAY_OBJECT_HPP

#include <memory>
#include <vector>
#include "../../engine_util/Vector.hpp"


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
						using simple_graphics::windows::direct3d::v9::engine_util::Vector;
						//该类相当于所有显示组件的基类
						class DisplayObject
						{
						private:
							Vector relativePosition;
							Vector relativeRotation;
							Vector relativeScale;
						protected:
							inline DisplayObject(
								const Vector &relativePosition = Vector(0, 0, 0),
								const Vector &relativeRotation = Vector(0, 0, 0),
								const Vector &relativeScale = Vector(1, 1, 1)
							)
							{
								this->relativePosition = relativePosition;
								this->relativeRotation = relativeRotation;
								this->relativeScale = relativeScale;
							}
						public:
							virtual ~DisplayObject()
							{
							}

							//这三个参数是被调用的对象的空间绝对位置，绝对旋转和绝对缩放，
							//由调用这个函数的（上一级）DisplayObject计算而来

							//绝对位置会受到上一级直到上一级的总旋转的影响。
							//旋转显然也会受到直到上一级的累加旋转的影响
							virtual void render(
								const Vector &absolutePosition = Vector(0, 0, 0),
								const Vector &absoluteRotation = Vector(0, 0, 0),
								const Vector &absoluteScale = Vector(1, 1, 1)
							) = 0;

							//每一个显示组件都必须有这三个getter methods
							//不然没法计算绝对参数

							//the following getter methods are virtual method
							//instead of pure virtual methods
							//which means different type of DisplayObject
							//may have different implementation
							//and will be able to easily overrided
							//设置成虚函数是因为不同的组件可能有不同的实现方式
							virtual inline Vector getRelativePosition()
							{
								return this->relativePosition;
							}
							virtual inline Vector getRelativeRotation()
							{
								return this->relativeRotation;
							}
							virtual inline Vector getRelativeScale()
							{
								return this->relativeScale;
							}
						};

						//Composite 组合模式
						class Composite :public DisplayObject
						{
						private:
							std::vector<std::shared_ptr<DisplayObject>> children;
						public:
							inline void render(
								const Vector &absolutePosition = Vector(0, 0, 0),
								const Vector &absoluteRotation = Vector(0, 0, 0),
								const Vector &absoluteScale = Vector(1, 1, 1)) override
							{
								for (std::vector<std::shared_ptr<DisplayObject>>::const_iterator childrenIterator = this->children.begin();
									childrenIterator != this->children.end(); ++childrenIterator
									)
								{
									//记住：这个元素的绝对位置会受到累加旋转的影响
									/*
									Vector currentChildAbsolutePosition;
									Vector currentChildAbsoluteRotation;
									Vector currentChildAbsoluteScale;

									//这个关系应该是成立的，要写Demo去验证
									currentChildAbsoluteRotation =
									absolutePosition + ((*childrenIterator)->getRelativePosition()).rotate(absoluteRotation);
									currentChildAbsoluteRotation =
									absoluteRotation + ((*childrenIterator)->getRelativeRotation());
									currentChildAbsoluteScale =
									absoluteScale * ((*childrenIterator)->getRelativeScale());

									(*childrenIterator)->render(
									currentChildAbsolutePosition,
									currentChildAbsoluteRotation,
									currentChildAbsoluteScale
									);
									*/
									(*childrenIterator)->render(
										absolutePosition + ((*childrenIterator)->getRelativePosition()).rotate(absoluteRotation),
										absoluteRotation + ((*childrenIterator)->getRelativeRotation()),
										absoluteScale * ((*childrenIterator)->getRelativeScale()));
								}
							}

							//默认构造函数
							explicit inline Composite(
								const Vector &relativePosition = Vector(0, 0, 0),
								const Vector &relativeRotation = Vector(0, 0, 0),
								const Vector &relativeScale = Vector(1, 1, 1)
							) :DisplayObject(relativePosition, relativeRotation, relativeScale)
							{}

							inline std::vector<std::shared_ptr<DisplayObject>>::size_type 
								add(const std::shared_ptr<DisplayObject> &displayObject)
							{
								this->children.push_back(displayObject);
								return this->children.size();
							}

							void clear()
							{
								this->children.clear();
							}
						};
					}
				}
			}
		}
	}
}
#endif // !SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__VISUAL_COMPONENT__DISPLAY_OBJECT_HPP