
#ifndef D3D_TEXTURE_H
#define D3D_TEXTURE_H

#include "core/graphics/texture.h"

struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;
struct ID3D11Resource;

namespace Graphics
{
   class D3DTexture : public Texture
   {
   public:
      D3DTexture(ID3D11ShaderResourceView* presource, ID3D11Resource* ptexture);
      
    // operations
      virtual bool   create(Device& device, const TextureInfo& info) override;

      virtual void   update(RenderContext& context, const void* pdata, int rowpitch) override;

      virtual void   enable(RenderContext& context, int stage) const override;
      virtual void   disable(RenderContext& context) const override;

      virtual bool   isValid() const override;

   private:

    // data
      ID3D11Resource*           mpTexture;
      ID3D11ShaderResourceView* mpResource;
      ID3D11SamplerState*       mpSampler;
   };
}

#endif // D3D_TEXTURE_H
