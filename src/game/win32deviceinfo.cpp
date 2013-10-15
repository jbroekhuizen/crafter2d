
#include "win32deviceinfo.h"

#include "d3d/d3ddevice.h"

#include <d3d11_1.h>

Win32DeviceInfo::Win32DeviceInfo(uint32_t wnd):
   IDeviceInfo(),
   mWnd(wnd)
{
}

bool Win32DeviceInfo::needsCustomDevice() const
{
   return true;
}

bool Win32DeviceInfo::buildCustomDevice(Graphics::Device& device)
{
   Graphics::D3DDevice& d3ddevice = static_cast<Graphics::D3DDevice&>(device);
   ID3D11Device1& d3ddevice1 = d3ddevice.getDevice();

   DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
	swapChainDesc.Width = static_cast<UINT>(800); // Match the size of the window.
	swapChainDesc.Height = static_cast<UINT>(600);
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1; // On phone, only single buffering is supported.
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH; // On phone, only stretch and aspect-ratio stretch scaling are allowed.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // On phone, no swap effects are supported.
	swapChainDesc.Flags = 0;

   IDXGIDevice1* dxgiDevice;
   d3ddevice1.QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
		
	IDXGIAdapter* dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);
		
	IDXGIFactory2* dxgiFactory;
	dxgiAdapter->GetParent(
      __uuidof(IDXGIFactory2), 
			(void**) &dxgiFactory
		);

   IDXGISwapChain1* pswapChain;
   HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(&d3ddevice1, (HWND)mWnd, &swapChainDesc, NULL, NULL, &pswapChain);
   if ( FAILED(hr) )
   {
      return false;
   }

   d3ddevice.setSwapChain(pswapChain);

   return true;
}