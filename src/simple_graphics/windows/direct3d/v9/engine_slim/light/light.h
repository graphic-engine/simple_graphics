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
							//复制构造函数
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
							//Ambient（环境光）没有位置和方向
							//所有的光线都均匀地从各个方向照射到物体上
							//各颜色成分取值范围在0到1之间
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
							//Diffuse（漫射光）沿着特定的方向传播
							//当它到达某一表面时将向各个方向均匀反射
							//（其强度与入射角的余弦成正比）
							//所以漫射光的空间位置和方向将对物体的照明有很大影响
							//各颜色成分取值范围在0到1之间
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
							//Specular（镜面光）照射到某一表面时
							//将严格按照另一个方向反射
							//从而形成只能在一定角度范围内观察到的高亮度照射
							//各颜色成分取值范围在0到1之间
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

							//设置光源的位置，对于方向光无意义
							inline LightConfigBuilder &setPosition(Vector position)
							{
								this->d3dLight.Position = D3DXVECTOR3(
									static_cast<float>(position.getX()),
									static_cast<float>(position.getY()),
									static_cast<float>(position.getZ())
								);
								return *this;
							}

							//设置光源的传播方向，对于点光源无意义
							inline LightConfigBuilder &setDirection(Vector direction)
							{
								this->d3dLight.Direction = D3DXVECTOR3(
									static_cast<float>(direction.getX()),
									static_cast<float>(direction.getY()),
									static_cast<float>(direction.getZ())
								);
								return *this;
							}

							//设置光线所能到达的最大范围，对于方向光无意义
							inline LightConfigBuilder &setRange(
								const float &maxRange = sqrtf(FLT_MAX)
							)
							{
								this->d3dLight.Range = maxRange;
								return *this;
							}

							//该参数仅用于聚光灯，表示光强从内锥形到外锥形的衰减方式
							//该参数一般取值1.0f
							inline LightConfigBuilder &setFalloff(const float &falloff = 1.0f)
							{
								this->d3dLight.Falloff = falloff;
								return *this;
							}

							//Attenuation0,Attenuation1,Attenuation2三个参数
							//仅用于点光源和聚光灯，对于方向光无意义
							//为光强与距离的函数关系中的系数。
							//attenuation = 1.0/(A0 + A1*D + A2*D*D)
							//其中D为观察点到光源的距离
							inline LightConfigBuilder &setAttenuation(const Vector &c = Vector(0, 0, 1))
							{
								this->d3dLight.Attenuation0 = static_cast<float>(c.getX());
								this->d3dLight.Attenuation1 = static_cast<float>(c.getY());
								this->d3dLight.Attenuation2 = static_cast<float>(c.getZ());
								return *this;
							}

							//Attenuation0,Attenuation1,Attenuation2三个参数
							//仅用于点光源和聚光灯，对于方向光无意义
							//为光强与距离的函数关系中的系数。
							//attenuation = 1.0/(A0 + A1*D + A2*D*D)
							//其中D为观察点到光源的距离
							inline LightConfigBuilder &setAttenuation(
								const double &a0 = 0, const double &a1 = 0, const double &a2 = 1
							)
							{
								this->d3dLight.Attenuation0 = static_cast<float>(a0);
								this->d3dLight.Attenuation1 = static_cast<float>(a1);
								this->d3dLight.Attenuation2 = static_cast<float>(a2);
								return *this;
							}

							//Attenuation0,Attenuation1,Attenuation2三个参数
							//仅用于点光源和聚光灯，对于方向光无意义
							//为光强与距离的函数关系中的系数。
							//attenuation = 1.0/(A0 + A1*D + A2*D*D)
							//其中D为观察点到光源的距离
							inline LightConfigBuilder &setAttenuation(
								const float &a0 = 0, const float &a1 = 0, const float &a2 = 1
							)
							{
								this->d3dLight.Attenuation0 = a0;
								this->d3dLight.Attenuation1 = a1;
								this->d3dLight.Attenuation2 = a2;
								return *this;
							}

							//仅用于指定聚光灯内部锥形的圆心角，单位为弧度
							inline LightConfigBuilder &theta(const float &theta)
							{
								this->d3dLight.Theta = theta;
								return *this;
							}

							//仅用于指定聚光灯外部锥形的圆心角，单位为弧度
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