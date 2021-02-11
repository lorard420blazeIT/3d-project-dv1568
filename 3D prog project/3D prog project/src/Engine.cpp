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
	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, texture, textureSRV, sampler, textureFilePath, constantBufferObj, constantBufferLight))
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
		//input.Update();
		if (input.KeyPressed(KEY(F)))
		{
			std::cout << "pressed F" << std::endl;
		}
		input.test();
		cam.Update();
		moveCamera();

		render.render(immediateConxtex, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, textureSRV, sampler);
		swapChain->Present(0, 0);
		UpdateBuffer(constantBufferObj, constantBufferLight, immediateConxtex, &frame, currentRotation, &lightFrame, cam);
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

void Engine::moveCamera()
{
	float speed = 0.1;
	if (input.KeyPressed(KEY(W)))
	{
		cam.movefoward(speed);
		std::cout << "pressed W" << std::endl;
	}

	if (input.KeyPressed(KEY(A)))
	{
		cam.moveLeft(speed);
		std::cout << "pressed A" << std::endl;
	}

	if (input.KeyPressed(KEY(S)))
	{
		cam.moveback(speed);
		std::cout << "pressed S" << std::endl;
	}

	if (input.KeyPressed(KEY(D)))
	{
		cam.moveRight(speed);
		std::cout << "pressed D" << std::endl;
	}

	if (input.KeyPressed(KEY(V)))
	{
		cam.SetStartPos();
		std::cout << "pressed home" << std::endl;
	}
	cam.subRollPitchYaw(input.getdelta());
	float limit = dx::XM_PI / 2.0f - 0.01f;
	cam.setRollPitchYaw(sm::Vector3{ 0,std::max(-limit,cam.getRollPitchYaw().y),0 });
	cam.setRollPitchYaw(sm::Vector3{ 0,std::min(limit,cam.getRollPitchYaw().y),0 });
	

	if (cam.getRollPitchYaw().z > dx::XM_PI) 
	{
		cam.subRollPitchYaw(sm::Vector3(0,(dx::XM_PI * 2.0f),0));
	
	
	}
	else if (cam.getRollPitchYaw().z < dx::XM_PI)
	{
		cam.subRollPitchYaw(sm::Vector3(0, -(dx::XM_PI * 2.0f), 0));


	}
	

	//sm::Quaternion q = dx::XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);


}

