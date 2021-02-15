#include "Engine.h"


Engine::Engine(HINSTANCE& hinstance, HINSTANCE& hPrevIntance, LPWSTR& lpmCmdLine, int& nCmdShow)
	:nCmdShow(nCmdShow), instance(hinstance), cam(WIDTH, HEIGHT)
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
	if (!windowMain.setUpWindow(instance, WIDTH, HEIGHT, nCmdShow))
	{
		std::cerr << "Failed to setup Window" << std::endl;
		return false;
	}
	if (!windowMain.setUpConsole())
	{
		std::cerr << "Failed to setup console" << std::endl;
		return false;
	}
	if (!SetupD3D11(HEIGHT, WIDTH, windowMain.getHWND(), device, immediateConxtex, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to create D3D11!" << std::endl;
		return false;
	}

	defRender.Initialize(device, immediateConxtex, rtv, dsView, viewport, textureFilePath);

	if (!defRender.SetupPipeline())
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return false;
	}

	input.SetUp(&windowMain);

	return true;
}

void Engine::Update()
{
	auto timeStart = clock.now();
	auto timeStop = clock.now();

	while (msg.message != WM_QUIT && !input.KeyPressed(KEY(Escape)))
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

		input.Update();
		cam.Update();
		moveCamera();
		defRender.Render();
		swapChain->Present(0, 0);
		defRender.Update(&frame, currentRotation, &lightFrame, cam);
	}
}

void Engine::ReleaseAll()
{
	//texture->Release();
	//textureSRV->Release();
	//sampler->Release();
	//vertexBuffer->Release();
	//indexBuffer->Release();
	//inputLayout->Release();
	//vShader->Release();
	//pShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateConxtex->Release();
	device->Release();
	defRender.Release();
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

void Engine::moveCamera()
{
	float speed = 0.003;
	if (input.KeyPressed(KEY(W)))
	{
		cam.movefoward(speed);
	}

	if (input.KeyPressed(KEY(A)))
	{
		cam.moveLeft(speed);
	}

	if (input.KeyPressed(KEY(S)))
	{
		cam.moveback(speed);
	}

	if (input.KeyPressed(KEY(D)))
	{
		cam.moveRight(speed);
	}

	if (input.KeyPressed(KEY(V)))
	{
		cam.SetStartPos();
	}
	
	if (input.UpdateMouse())
	{
		std::cout << "go in the if sats " << std::endl;

		cam.subYawPitchRoll(-input.getdelta());

		//Limits to only look 180 degres down and up
		float limit = dx::XM_PI / 2.0f - 0.01f;
		cam.setYawPitchRoll(sm::Vector3{ cam.getYawPitchRoll().x ,std::max(-limit, cam.getYawPitchRoll().y) ,cam.getYawPitchRoll().z });
		cam.setYawPitchRoll(sm::Vector3{ cam.getYawPitchRoll().x ,std::min(limit, cam.getYawPitchRoll().y), cam.getYawPitchRoll().z });

		if (cam.getYawPitchRoll().x > dx::XM_PI)
		{
			cam.subYawPitchRoll(sm::Vector3((dx::XM_PI * 2.0f), 0,0));
		}
		else if (cam.getYawPitchRoll().x < dx::XM_PI)
		{
			cam.subYawPitchRoll(sm::Vector3(-(dx::XM_PI * 2.0f), 0,0));
		}
	}
}

