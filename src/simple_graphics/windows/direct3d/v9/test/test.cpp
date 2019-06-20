#include<iostream>

#include "../engine_slim/Window.hpp"
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

		//Text testText = Text(window, Text::Box(Vector(1, 1, 0.001*0.001)), "Szx");
		testComposite.add(std::make_shared<Text>(
			window, Text::Box(Vector(1, 1, 0.001*0.001)), "Szx")
		);


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

	window.setStaticCamera(Vector(0, 0, -1), Vector(0, 0, 0));
	window.render(displayTest);
}