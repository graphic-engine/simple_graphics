#include<iostream>

#include "../engine_slim/Window.hpp"
#include "../device_util/device_util.h"
#include "../engine_slim/light/light.h"
#include "../engine_slim/light/light_examples.h"
#include "../engine_slim/visual_component/visual_component.h"

int main()
{
	using simple_graphics::windows::direct3d::v9::engine_slim::Window;
	using simple_graphics::windows::direct3d::v9::engine_slim::visual_component::Composite;
	using simple_graphics::windows::direct3d::v9::engine_slim::visual_component::LineList;
	using simple_graphics::windows::direct3d::v9::engine_slim::visual_component::Text;


	Window window = Window(800,800);
	auto displayTest = [](
		const Window &window, const double &totalTime, const double &deltaTime
		)->void
	{
		/*
		window.getDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0x7f, 0x7f, 0x7f), 1.0f, 0
		);
		window.getDevice()->BeginScene();
		*/
		window.clearAndBegin();

		Composite testComposite = Composite();

		std::vector<std::pair<LineList::Vertex, LineList::Vertex>> testVertexPairVector;
		LineList::Vertex vertexArray[4];


		vertexArray[0].x = -500;
		vertexArray[0].y = 500;
		vertexArray[0].z = 1000;
		vertexArray[0].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[1].x = 500;
		vertexArray[1].y = -500;
		vertexArray[1].z = 1000;
		vertexArray[1].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[2].x = -500;
		vertexArray[2].y = -500;
		vertexArray[2].z = 1000;
		vertexArray[2].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[3].x = 500;
		vertexArray[3].y = 500;
		vertexArray[3].z = 1000;
		vertexArray[3].color = D3DCOLOR_XRGB(0, 0, 0xff);

		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 0), *(vertexArray + 1)));
		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 2), *(vertexArray + 3)));

		testComposite.add(std::make_shared<LineList>(window, testVertexPairVector));
		testVertexPairVector.clear();



		vertexArray[0].x = -200;
		vertexArray[0].y = 200;
		vertexArray[0].z = 1000;
		vertexArray[0].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[1].x = 200;
		vertexArray[1].y = -200;
		vertexArray[1].z = 1000;
		vertexArray[1].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[2].x = -200;
		vertexArray[2].y = -200;
		vertexArray[2].z = 1000;
		vertexArray[2].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[3].x = 200;
		vertexArray[3].y = 200;
		vertexArray[3].z = 1000;
		vertexArray[3].color = D3DCOLOR_XRGB(0, 0, 0xff);

		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 0), *(vertexArray + 1)));
		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 2), *(vertexArray + 3)));

		testComposite.add(
			std::make_shared<LineList>(
				window,
				testVertexPairVector,
				Vector(-500, 500),
				Vector(0, 0, 0.003*totalTime)
				)
		);
		testVertexPairVector.clear();



		vertexArray[0].x = -200;
		vertexArray[0].y = 200;
		vertexArray[0].z = 1000;
		vertexArray[0].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[1].x = 200;
		vertexArray[1].y = -200;
		vertexArray[1].z = 1000;
		vertexArray[1].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[2].x = -200;
		vertexArray[2].y = -200;
		vertexArray[2].z = 1000;
		vertexArray[2].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[3].x = 200;
		vertexArray[3].y = 200;
		vertexArray[3].z = 1000;
		vertexArray[3].color = D3DCOLOR_XRGB(0, 0, 0xff);

		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 0), *(vertexArray + 1)));
		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 2), *(vertexArray + 3)));

		testComposite.add(
			std::make_shared<LineList>(
				window,
				testVertexPairVector,
				Vector(500, 500),
				Vector(0, 0, 0.003*totalTime)
				)
		);
		testVertexPairVector.clear();
		testVertexPairVector.clear();


		vertexArray[0].x = -200;
		vertexArray[0].y = 200;
		vertexArray[0].z = 1000;
		vertexArray[0].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[1].x = 200;
		vertexArray[1].y = -200;
		vertexArray[1].z = 1000;
		vertexArray[1].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[2].x = -200;
		vertexArray[2].y = -200;
		vertexArray[2].z = 1000;
		vertexArray[2].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[3].x = 200;
		vertexArray[3].y = 200;
		vertexArray[3].z = 1000;
		vertexArray[3].color = D3DCOLOR_XRGB(0, 0, 0xff);

		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 0), *(vertexArray + 1)));
		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 2), *(vertexArray + 3)));

		testComposite.add(
			std::make_shared<LineList>(
				window,
				testVertexPairVector,
				Vector(-500, -500),
				Vector(0, 0, 0.003*totalTime)
				)
		);
		testVertexPairVector.clear();


		vertexArray[0].x = -200;
		vertexArray[0].y = 200;
		vertexArray[0].z = 1000;
		vertexArray[0].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[1].x = 200;
		vertexArray[1].y = -200;
		vertexArray[1].z = 1000;
		vertexArray[1].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[2].x = -200;
		vertexArray[2].y = -200;
		vertexArray[2].z = 1000;
		vertexArray[2].color = D3DCOLOR_XRGB(0, 0, 0xff);
		vertexArray[3].x = 200;
		vertexArray[3].y = 200;
		vertexArray[3].z = 1000;
		vertexArray[3].color = D3DCOLOR_XRGB(0, 0, 0xff);

		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 0), *(vertexArray + 1)));
		testVertexPairVector.push_back(std::pair<LineList::Vertex, LineList::Vertex>(*(vertexArray + 2), *(vertexArray + 3)));

		testComposite.add(
			std::make_shared<LineList>(
				window,
				testVertexPairVector,
				Vector(500, -500),
				Vector(0, 0, 0.003*totalTime)
				)
		);
		testVertexPairVector.clear();
		testVertexPairVector.clear();

		using simple_graphics::windows::direct3d::v9::device_util::DeviceRenderStateView;
		DeviceRenderStateView renderStateView(window.getDevice());

		Text testText = Text(
			window, Text::Box(Vector(200, 100, 10)),
			"Tsinghua University"
		);
		//文本网格的灵活顶点格式包含位置和法向量
		//要开启光源处理，以根据外部光线及其材质确定颜色
		renderStateView.processLights();
		testText.render(Vector(0, 0, 100));

		//在渲染灵活顶点格式为(D3DFVF_XYZ|D3DFVF_DIFFUSE)的图元时
		//要关闭光源处理，以根据其自身定义的颜色渲染
		//否则其颜色由外部环境光确定
		renderStateView.setDefaultAmbient().processLights(false);
		testComposite.render(
			Vector(),
			Vector(0, 0, -0.001*totalTime)
		);

		window.endAndPresent();
		/*
		window.getDevice()->EndScene();
		//将后台缓冲区的内容投射上前台
		window.getDevice()->Present(0, 0, 0, 0);
		*/
	};

	window.setStaticCamera(Vector(0, 0, 0), Vector(0, 0, 1));
	using simple_graphics::windows::direct3d::v9::engine_slim::light::Light;
	Light(window, 0).set(
		simple_graphics::windows::direct3d::v9::engine_slim::light::light_example::getDirectionalLightExample(0)
	).on();
	using simple_graphics::windows::direct3d::v9::device_util::DeviceRenderStateView;
	DeviceRenderStateView(window.getDevice()).setShadeMode(DeviceRenderStateView::ShadeMode_GOURAUD);


	D3DMATERIAL9 material;
	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;
	material.Specular.r = 1.0f;
	material.Specular.g = 1.0f;
	material.Specular.b = 1.0f;
	material.Specular.a = 1.0f;
	material.Emissive = { 0,0,0,0 };

	window.getDevice()->SetMaterial(&material);


	window.render(displayTest);
}