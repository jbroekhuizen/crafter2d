
#include "d3dindexbuffer.h"

#include "d3ddevice.h"
#include "d3drendercontext.h"

namespace Graphics
{

D3DIndexBuffer::D3DIndexBuffer(D3DDevice& device):
   IndexBuffer(),
   mDevice(device)
{
}

bool D3DIndexBuffer::create(IndexFormat format, int elements, void* data)
{
   mFormat = DXGI_FORMAT_R16_UINT;

   D3D11_BUFFER_DESC indexBufferDesc;
   indexBufferDesc.ByteWidth = sizeof(unsigned short) * elements;
   indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
   indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
   indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   indexBufferDesc.MiscFlags = 0;
   indexBufferDesc.StructureByteStride = 0;
   
   HRESULT hr = mDevice.getDevice().CreateBuffer(&indexBufferDesc, NULL, &mpBuffer);

   return SUCCEEDED(hr);
}

void D3DIndexBuffer::release()
{
   mpBuffer->Release();
   mpBuffer = NULL;
}

void D3DIndexBuffer::enable(RenderContext& context) const
{
   D3DRenderContext::asContext(context).IASetIndexBuffer(mpBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void D3DIndexBuffer::disable(RenderContext& context) const
{
}

void* D3DIndexBuffer::lock(RenderContext& context)
{
   D3D11_MAPPED_SUBRESOURCE mappedResource;
   D3DRenderContext::asContext(context).Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
   return mappedResource.pData;
}

void D3DIndexBuffer::unlock(RenderContext& context)
{
   D3DRenderContext::asContext(context).Unmap(mpBuffer, 0);
}

} // namespace Graphics
