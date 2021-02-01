#include "Engine.h"


Engine::Engine(HINSTANCE& hinstance, HINSTANCE& hPrevIntance, LPWSTR& lpmCmdLine, int& nCmdShow)
	:nCmdShow(nCmdShow), instance(hinstance)
{
	//stuff in update
	msg = {};
	deltaTime = 0;
	currentRotation = 0;
	speed = 0.5f;

	//Light stuff
	lightFrame.ambient = { 0.3f, 0.3f, 0.3f };
	lightFrame.pos = { 0, 0, -3 };
	lightFrame.color = { 1, 1, 1 };
	lightFrame.specPower = 128.0f;

	textureFilePath = "../Texture/this-is-fine-Charlie.png";

}

bool Engine::SetUp()
{
	//Setup stuff
	if (!windowMain.setUpWindow(instance, WIDTH, HEIGHT, nCmdShow, winHWND))
	{
		std::cerr << "Failed to setup Window" << std::endl;
		return -1;
	}
	if (!windowMain.setUpConsole())
	{
		std::cerr << "Failed to setup console" << std::endl;
		return -1;
	}
	if (!SetupD3D11(HEIGHT, WIDTH, winHWND, device, immediateConxtex, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to create D3D11!" << std::endl;
		return -1;
	}
	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, texture, textureSRV, sampler, textureFilePath, constantBufferObj, constantBufferLight))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	return true;
}


void Engine::Update()
{
	auto timeStart = clock.now();
	auto timeStop = clock.now();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		timeStop = clock.now();

		using ms = std::chrono::duration<float, std::milli>;
		deltaTime = std::chrono::duration_cast<ms>(timeStop - timeStart).count() / 1000;

		currentRotation = deltaTime * speed;

		render.render(immediateConxtex, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, textureSRV, sampler);
		swapChain->Present(0, 0);
		UpdateBuffer(constantBufferObj, constantBufferLight, immediateConxtex, &frame, currentRotation, &lightFrame);
	}
}

void Engine::ReleaseAll()
{
	constantBufferLight->Release();
	constantBufferObj->Release();
	texture->Release();
	textureSRV->Release();
	sampler->Release();
	vertexBuffer->Release();
	inputLayout->Release();
	vShader->Release();
	pShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateConxtex->Release();
	device->Release();
}

void Engine::Run()
{
	if (!SetUp())
	{
		std::cerr << "Failed to setup" << std::endl;
	}
	Update();
	ReleaseAll();
}