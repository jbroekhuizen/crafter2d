
#ifndef D3D_INDEX_BUFFER_H
#define D3D_INDEX_BUFFER_H

#include "core/graphics/indexbuffer.h"
#include "core/defines.h"

struct ID3D11Buffer;

namespace Graphics
{
   class D3DDevice;

   class D3DIndexBuffer : public IndexBuffer
   {
   public:
      D3DIndexBuffer(D3DDevice& device);

      virtual bool create(IndexFormat format, int elements, void* data) override;
      virtual void release() override;

      virtual void enable(RenderContext& context) const override;
      virtual void disable(RenderContext& context) const override;

      virtual void* lock(RenderContext& context) override;
      virtual void  unlock(RenderContext& context) override;

   private:

    // data
      D3DDevice&    mDevice;
      ID3D11Buffer* mpBuffer;
      uint          mFormat;
   };
}

#endif // D3D_INDEX_BUFFER_H
