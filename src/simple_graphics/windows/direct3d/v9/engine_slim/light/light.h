#pragma once
#ifndef SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__LIGHT__LIGHT_H
#define SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__LIGHT__LIGHT_H

#include "../Window.hpp"
using simple_graphics::windows::direct3d::v9::engine_slim::Window;
#include "../../engine_util/Vector.hpp"
using simple_graphics::windows::direct3d::v9::engine_util::Vector;

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
					namespace light
					{
						class LightConfigBuilder final
						{
						private:
							D3DLIGHT9 d3dLight;
						public:
							LightConfigBuilder()
							{
								this->d3dLight.Range = sqrtf(FLT_MAX);
								this->d3dLight.Falloff = 1.0f;
								this->d3dLight.Attenuation0 = 0.0f;
								this->d3dLight.Attenuation1 = 0.0f;
								this->d3dLight.Attenuation2 = 1.0f;
							}
							//���ƹ��캯��
							LightConfigBuilder(
								const LightConfigBuilder &lightConfigBuilder
							)
							{
								this->d3dLight = lightConfigBuilder.d3dLight;
							}

							inline explicit operator D3DLIGHT9()
							{
								return d3dLight;
							}
							inline D3DLIGHT9 build()
							{
								return d3dLight;
							}

							enum Type
							{
								POINT = D3DLIGHT_POINT,
								SPOT = D3DLIGHT_SPOT,
								DIRECTIONAL = D3DLIGHT_DIRECTIONAL
							};
							inline LightConfigBuilder &setType(Type type)
							{
								switch (type)
								{
								case POINT:
									this->d3dLight.Type = D3DLIGHT_POINT;
									break;
								case SPOT:
									this->d3dLight.Type = D3DLIGHT_SPOT;
									break;
								case DIRECTIONAL:
									this->d3dLight.Type = D3DLIGHT_DIRECTIONAL;
									break;
								}
								return *this;
							}
							//Ambient�������⣩û��λ�úͷ���
							//���еĹ��߶����ȵشӸ����������䵽������
							//����ɫ�ɷ�ȡֵ��Χ��0��1֮��
							inline LightConfigBuilder &setAmbient(
								const float &r, const float &g, const float &b,
								const float &a = 1.0f
							)
							{
								this->d3dLight.Ambient.r = r;
								this->d3dLight.Ambient.g = g;
								this->d3dLight.Ambient.b = b;
								this->d3dLight.Ambient.a = a;
								return *this;
							}
							//Diffuse������⣩�����ض��ķ��򴫲�
							//��������ĳһ����ʱ�������������ȷ���
							//����ǿ��������ǵ����ҳ����ȣ�
							//���������Ŀռ�λ�úͷ��򽫶�����������кܴ�Ӱ��
							//����ɫ�ɷ�ȡֵ��Χ��0��1֮��
							inline LightConfigBuilder &setDiffuse(
								const float &r, const float &g, const float &b,
								const float &a = 1.0f
							)
							{
								this->d3dLight.Diffuse.r = r;
								this->d3dLight.Diffuse.g = g;
								this->d3dLight.Diffuse.b = b;
								this->d3dLight.Diffuse.a = a;
								return *this;
							}
							//Specular������⣩���䵽ĳһ����ʱ
							//���ϸ�����һ��������
							//�Ӷ��γ�ֻ����һ���Ƕȷ�Χ�ڹ۲쵽�ĸ���������
							//����ɫ�ɷ�ȡֵ��Χ��0��1֮��
							inline LightConfigBuilder &setSpecular(
								const float &r, const float &g, const float &b,
								const float &a = 1.0f
							)
							{
								this->d3dLight.Specular.r = r;
								this->d3dLight.Specular.g = g;
								this->d3dLight.Specular.b = b;
								this->d3dLight.Specular.a = a;
								return *this;
							}

							//���ù�Դ��λ�ã����ڷ����������
							inline LightConfigBuilder &setPosition(Vector position)
							{
								this->d3dLight.Position = D3DXVECTOR3(
									static_cast<float>(position.getX()),
									static_cast<float>(position.getY()),
									static_cast<float>(position.getZ())
								);
								return *this;
							}

							//���ù�Դ�Ĵ������򣬶��ڵ��Դ������
							inline LightConfigBuilder &setDirection(Vector direction)
							{
								this->d3dLight.Direction = D3DXVECTOR3(
									static_cast<float>(direction.getX()),
									static_cast<float>(direction.getY()),
									static_cast<float>(direction.getZ())
								);
								return *this;
							}

							//���ù������ܵ�������Χ�����ڷ����������
							inline LightConfigBuilder &setRange(
								const float &maxRange = sqrtf(FLT_MAX)
							)
							{
								this->d3dLight.Range = maxRange;
								return *this;
							}

							//�ò��������ھ۹�ƣ���ʾ��ǿ����׶�ε���׶�ε�˥����ʽ
							//�ò���һ��ȡֵ1.0f
							inline LightConfigBuilder &setFalloff(const float &falloff = 1.0f)
							{
								this->d3dLight.Falloff = falloff;
								return *this;
							}

							//Attenuation0,Attenuation1,Attenuation2��������
							//�����ڵ��Դ�;۹�ƣ����ڷ����������
							//Ϊ��ǿ�����ĺ�����ϵ�е�ϵ����
							//attenuation = 1.0/(A0 + A1*D + A2*D*D)
							//����DΪ�۲�㵽��Դ�ľ���
							inline LightConfigBuilder &setAttenuation(const Vector &c = Vector(0, 0, 1))
							{
								this->d3dLight.Attenuation0 = static_cast<float>(c.getX());
								this->d3dLight.Attenuation1 = static_cast<float>(c.getY());
								this->d3dLight.Attenuation2 = static_cast<float>(c.getZ());
								return *this;
							}

							//Attenuation0,Attenuation1,Attenuation2��������
							//�����ڵ��Դ�;۹�ƣ����ڷ����������
							//Ϊ��ǿ�����ĺ�����ϵ�е�ϵ����
							//attenuation = 1.0/(A0 + A1*D + A2*D*D)
							//����DΪ�۲�㵽��Դ�ľ���
							inline LightConfigBuilder &setAttenuation(
								const double &a0 = 0, const double &a1 = 0, const double &a2 = 1
							)
							{
								this->d3dLight.Attenuation0 = static_cast<float>(a0);
								this->d3dLight.Attenuation1 = static_cast<float>(a1);
								this->d3dLight.Attenuation2 = static_cast<float>(a2);
								return *this;
							}

							//Attenuation0,Attenuation1,Attenuation2��������
							//�����ڵ��Դ�;۹�ƣ����ڷ����������
							//Ϊ��ǿ�����ĺ�����ϵ�е�ϵ����
							//attenuation = 1.0/(A0 + A1*D + A2*D*D)
							//����DΪ�۲�㵽��Դ�ľ���
							inline LightConfigBuilder &setAttenuation(
								const float &a0 = 0, const float &a1 = 0, const float &a2 = 1
							)
							{
								this->d3dLight.Attenuation0 = a0;
								this->d3dLight.Attenuation1 = a1;
								this->d3dLight.Attenuation2 = a2;
								return *this;
							}

							//������ָ���۹���ڲ�׶�ε�Բ�Ľǣ���λΪ����
							inline LightConfigBuilder &theta(const float &theta)
							{
								this->d3dLight.Theta = theta;
								return *this;
							}

							//������ָ���۹���ⲿ׶�ε�Բ�Ľǣ���λΪ����
							inline LightConfigBuilder &phi(const float &phi)
							{
								return *this;
								this->d3dLight.Phi = phi;
							}
						};

						class Light
						{
						private:
							const Window window;
							const DWORD index;
							inline Light() = delete;
						public:
							inline Light(
								const Window &window, const DWORD &index
							) :window(window), index(index) {}
							inline Light &set(const D3DLIGHT9 lightConfig)
							{
								window.getDevice()->SetLight(index, &lightConfig);
								return *this;
							}
							inline Light &on()
							{
								window.getDevice()->LightEnable(index, true);
								return *this;
							}
							inline Light &off()
							{
								window.getDevice()->LightEnable(index, false);
								return *this;
							}
						};
					}
				}
			}
		}
	}
}
#endif // !SIMPLE_GRAPHICS__WINDOWS__DIRECT3D__V9__ENGINE_SLIM__LIGHT__LIGHT_H