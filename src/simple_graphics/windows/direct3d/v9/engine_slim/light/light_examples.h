#pragma once
#include "light.h"

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
						namespace light_example
						{
							inline D3DLIGHT9 getPointLightExample(const unsigned int &)
							{
								return light::LightConfigBuilder()
									.setType(LightConfigBuilder::POINT)
									.setAmbient(1, 1, 1).setDiffuse(1, 1, 1).setSpecular(1, 1, 1)
									.setPosition(Vector()).setAttenuation(1.0, 0.0, 0.0).build();
							}
							inline D3DLIGHT9 getDirectionalLightExample(const unsigned int &)
							{
								return light::LightConfigBuilder()
									.setType(LightConfigBuilder::DIRECTIONAL)
									.setAmbient(0, 1, 1).setDiffuse(1, 1, 1).setSpecular(0, 0, 0)
									.setPosition(Vector()).setDirection(Vector(0, 0, 1))
									.setAttenuation(1.0, 0.0, 0.0).build();
							}
							inline D3DLIGHT9 getSpotLightExample(const unsigned int &)
							{
								return light::LightConfigBuilder()
									.setType(LightConfigBuilder::SPOT)
									.setAmbient(1, 1, 1).setDiffuse(1, 1, 1).setSpecular(1, 1, 1)
									.setPosition(Vector(0,0,-200)).setDirection(Vector(0, 0, 1))
									.theta(D3DX_PI / 6).phi(D3DX_PI / 4)
									.setAttenuation(1.0, 0.0, 0.0).build();
							}
						}
					}
				}
			}
		}
	}
}