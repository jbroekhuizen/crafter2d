
#include "d3dvertexbuffer.h"

#include "core/graphics/vertexinputlayout.h"

#include "d3ddevice.h"
#include "d3drendercontext.h"

namespace Graphics
{

D3DVertexBuffer::D3DVertexBuffer(D3DDevice& device):
   VertexBuffer(),
   mDevice(device),
   mpBuffer(NULL),
   mStride(0)
{
}

D3DVertexBuffer::~D3DVertexBuffer()
{
   release();
}

bool D3DVertexBuffer::create(const VertexInputLayout& layout, int length, int usage)
{
   if ( !VertexBuffer::create(layout, length, usage) )
      return false;

   mStride = layout.getStride();

   D3D11_BUFFER_DESC desc = {0};
   desc.ByteWidth = mStride * length;
   desc.Usage = D3D11_USAGE_DYNAMIC;
   desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   desc.MiscFlags = 0;
   desc.StructureByteStride = 0;

   HRESULT hr = mDevice.getDevice().CreateBuffer(&desc, NULL, &mpBuffer);
   if ( FAILED(hr) )
      return false;

   return true;
}

void D3DVertexBuffer::release()
{
   mpBuffer->Release();
}

void* D3DVertexBuffer::lock(RenderContext& context)
{
   D3D11_MAPPED_SUBRESOURCE mappedResource;
   HRESULT hr = D3DRenderContext::asContext(context).Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
   if ( FAILED(hr) )
   {
      // meh!!
      return NULL;
   }
   return mappedResource.pData;
}

void D3DVertexBuffer::unlock(RenderContext& context)
{
   D3DRenderContext::asContext(context).Unmap(mpBuffer, 0);
}

static const UINT offset = 0;

void D3DVertexBuffer::enable(RenderContext& context) const
{
   D3DRenderContext::asContext(context).IASetVertexBuffers(0, 1, &mpBuffer, (UINT*)&mStride, &offset);
}

void D3DVertexBuffer::disable(RenderContext& context) const
{
}

} // namespace Graphics
