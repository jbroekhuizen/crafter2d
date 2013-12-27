/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "OGLdevice.h"

#include "core/log/log.h"
#include "core/smartptr/autoptr.h"
#include "core/graphics/blendstatedesc.h"
#include "core/graphics/textureinfo.h"

#include "texture/textureloaderfactory.h"
#include "texture/abstracttextureloader.h"

#include "oglblendstate.h"
#include "oglfont.h"
#include "ogltexture.h"
#include "oglrendertarget.h"
#include "oglrendercontext.h"
#include "oglindexbuffer.h"
#include "oglvertexbuffer.h"
#include "shaderpath.h"

namespace Graphics
{

OGLDevice::OGLDevice() :
   Device(),
   mFreeTypeLib(NULL)
{
}

OGLDevice::~OGLDevice()
{
   FT_Done_FreeType(mFreeTypeLib);
}

// - Query

bool OGLDevice::supportGLSL() const
{
   // GLSL is only supported if both vertex- and fragment shaders are available
   return (GLEE_ARB_shading_language_100 && GLEE_ARB_shader_objects && GLEE_ARB_vertex_shader);
}

// - Creation

bool OGLDevice::create(int windowhandle, int width, int height)
{
   Log& log = Log::getInstance();
   GLint units;

   log << "Graphics card:\t\t" << (char*)glGetString(GL_VENDOR) << "\n";
   log << "OpenGL version:\t\t" << (char*)glGetString(GL_VERSION) << "\n";

   if ( !GLEE_VERSION_3_2 )
   {
      log << "OpenGL version 3.2 is required to run this application.\n";
      return false;
   }

   // make sure that there are enough texture units
   glGetIntegerv (GL_MAX_TEXTURE_UNITS, &units);
   if ( units < 2 )
   {
      log << "Videocard does not support 2 or more texture units.\n";
   }

   log << "Max textures:\t\t" << (int)units << "\n";
   int maxTextureSize = 0;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
   log << "Max texture size:\t" << maxTextureSize << "\n";

   // see if required buffer objects are supported
   if ( !GL_ARB_vertex_buffer_object || !GL_ARB_uniform_buffer_object )
   {
      log << "Required buffer objects are not available. Can not proceed.\n";
      return false;
   }
   
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(25, 125, 0);
   
   // initialize free type
   if ( FT_Init_FreeType(&mFreeTypeLib) != 0 )
      return false;

   return Device::create(windowhandle, width, height);
}

RenderContext* OGLDevice::createRenderContext()
{
   return new OGLRenderContext();
}

CodePath* OGLDevice::createCodePath()
{
   return new ShaderPath();
}

/// \fn OGLDevice::createVertexBuffer()
/// \brief Creates a vertex buffer for storage of the geometry. When Vertex Buffer Objects are supported, one will
/// be created. A default buffer will be created otherwise.
/// \return A pointer to the vertex buffer instance.
VertexBuffer* OGLDevice::createVertexBuffer()
{
	return new OGLVertexBuffer();
}

IndexBuffer* OGLDevice::createIndexBuffer()
{
   return new OGLIndexBuffer();
}

/// \fn OGLDevice::createTexture()
/// \brief Creates a new empty texture.
/// \return A pointer to the created texture.
Texture* OGLDevice::createTexture(DataStream& data)
{
   Texture* presult = NULL;
   AutoPtr<AbstractTextureLoader> loader = TextureLoaderFactory::constructLoader(UTEXT(""));
   if ( loader.hasPointer() && loader->load(data) )
   {
      const TextureInfo& info = loader->getTextureInfo();
      presult = new OGLTexture();
      if ( !presult->create(*this, info) )
      {
      }
   }

   return presult;
}

RenderTarget* OGLDevice::createRenderTarget()
{
   return new OGLRenderTarget();
}

Font* OGLDevice::createFont(const String& name, int pointsize)
{
   FT_Face face;
   const std::string file = name.toUtf8();
   FT_Error error = FT_New_Face(mFreeTypeLib, file.c_str(), 0, &face);
	if ( error != 0 )
   {
      return NULL;
   }
  
   OGLFont* pfont = new OGLFont();
   pfont->initialize(face, pointsize);

   return pfont;
}

BlendState* OGLDevice::createBlendState(const BlendStateDesc& desc)
{
   GLenum sfactor = toGLBlendState(desc.getSourceFactor());
   GLenum dfactor = toGLBlendState(desc.getDestFactor());  

   return new OGLBlendState(sfactor, dfactor, desc.isEnabled());
}

GLenum OGLDevice::toGLBlendState(BlendStateDesc::BlendFactor blendfactor)
{
   GLenum factor;

   switch ( blendfactor )
   {
      case BlendStateDesc::BS_ONE:
         factor = GL_ONE;
         break;
      case BlendStateDesc::BS_ZERO:
         factor = GL_ZERO;
         break;
      case BlendStateDesc::BS_SRC_COLOR:
         factor = GL_SRC_COLOR;
         break;
      case BlendStateDesc::BS_SRC_INV_COLOR:
         factor = GL_ONE_MINUS_SRC_COLOR;
         break;
      case BlendStateDesc::BS_SRC_ALPHA:
         factor = GL_SRC_ALPHA;
         break;
      case BlendStateDesc::BS_SRC_INV_ALPHA:
         factor = GL_ONE_MINUS_SRC_ALPHA;
         break;
      case BlendStateDesc::BS_DST_COLOR:
         factor = GL_DST_COLOR;
         break;
      case BlendStateDesc::BS_DST_INV_COLOR:
         factor = GL_ONE_MINUS_DST_COLOR;
         break;
      case BlendStateDesc::BS_DST_ALPHA:
         factor = GL_DST_ALPHA;
         break;
      case BlendStateDesc::BS_DST_INV_ALPHA:
         factor = GL_ONE_MINUS_DST_ALPHA;
         break;
   }
   return factor;
}

// - Presenting

void OGLDevice::present()
{
}

};