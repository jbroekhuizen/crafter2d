
#ifndef OGL_GLYPH_PROVIDER_H
#define OGL_GLYPH_PROVIDER_H

#include "core/graphics/text/glyphprovider.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Graphics
{
   class OGLFont;

   class OGLGlyphProvider : public Graphics::GlyphProvider
   {
   public:
      OGLGlyphProvider(FT_Library library);

      void initialize(OGLFont& font);

      virtual Graphics::Glyph* getGlyph(UChar ch, float emsize) override;

   private:

      // data
      FT_Library mLibrary;
      OGLFont*   mpFont;
   };
}

#endif // OGL_GLYPH_PROVIDER_H