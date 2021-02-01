#include "d3d11Helper.h"

bool CreateInterface(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContext, IDXGISwapChain*& swapChain)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; //insträllningar för refresh rate 
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM; //Formatet, 8 bitar för varje kanal
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //Default scanlindeordering
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //Default scaling

	//Default 
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1; //Buffer
	swapChainDesc.OutputWindow = window; //kopplar till fönstret
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //Hur den gör näör den byter mellan front och back buffer
	swapChainDesc.Flags = 0; //No flags
	
	UINT flags = 0;
	if (_DEBUG)
		flags = D3D11_CREATE_DEVICE_DEBUG; //Så vi får lite extra debug info

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &immadeiateContext);

	//assert(SUCCEEDED(hr)); //gnäller ifall den inte sucess, fråga Grotto igen
	return !FAILED(hr);
}

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv)
{
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	HRESULT hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
	backBuffer->Release();

	return !FAILED(hr); //why !FAILED
}

bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView) 
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		std::cerr << "Failed to crate depth stencil texture!" << std::endl;
		return false;
	}

	HRESULT hr = device->CreateDepthStencilView(dsTexture, nullptr, &dsView);

	return !FAILED(hr);
}

void SetViewPort(D3D11_VIEWPORT& viewport, UINT width, UINT height)
{
	viewport.TopLeftX = 0; //ÄNDRA INTE FFS!
	viewport.TopLeftY = 0; //ÄNDRA INTE FFS!
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContect, IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport)
{
	if (!CreateInterface(width, height, window, device, immadeiateContect, swapChain))
	{
		std::cerr << "Error creating interface!" << std::endl;
		return false;
	}

	if (!CreateRenderTargetView(device, swapChain, rtv))
	{
		std::cerr << "Error creaing render target view" << std::endl;
		return false;
	}

	if (!CreateDepthStencil(device, width, height, dsTexture, dsView))
	{
		std::cerr << "Error creaing depth stencil view!" << std::endl;
		return false;
	}

	SetViewPort(viewport, width, height);

	return true;
}