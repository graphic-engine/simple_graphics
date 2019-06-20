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
						//�����൱��������ʾ����Ļ���
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

							//�����������Ǳ����õĶ���Ŀռ����λ�ã�������ת�;������ţ�
							//�ɵ�����������ģ���һ����DisplayObject�������

							//����λ�û��ܵ���һ��ֱ����һ��������ת��Ӱ�졣
							//��ת��ȻҲ���ܵ�ֱ����һ�����ۼ���ת��Ӱ��
							virtual void render(
								const Vector &absolutePosition = Vector(0, 0, 0),
								const Vector &absoluteRotation = Vector(0, 0, 0),
								const Vector &absoluteScale = Vector(1, 1, 1)
							) = 0;

							//ÿһ����ʾ�����������������getter methods
							//��Ȼû��������Բ���

							//the following getter methods are virtual method
							//instead of pure virtual methods
							//which means different type of DisplayObject
							//may have different implementation
							//and will be able to easily overrided
							//���ó��麯������Ϊ��ͬ����������в�ͬ��ʵ�ַ�ʽ
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

						//Composite ���ģʽ
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
									//��ס�����Ԫ�صľ���λ�û��ܵ��ۼ���ת��Ӱ��
									/*
									Vector currentChildAbsolutePosition;
									Vector currentChildAbsoluteRotation;
									Vector currentChildAbsoluteScale;

									//�����ϵӦ���ǳ����ģ�ҪдDemoȥ��֤
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

							//Ĭ�Ϲ��캯��
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