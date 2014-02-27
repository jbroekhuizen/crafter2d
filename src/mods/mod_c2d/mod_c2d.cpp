// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "core/modules/contentmodule.h"
#include "core/modules/modulecollection.h"
#include "core/graphics/effect.h"
#include "core/system/uuid.h"

#include "effect/effectreader.h"
#include "effect/effectwriter.h"

#include "entity/entityreader.h"
#include "entity/entitywriter.h"

#include "texture/texturereader.h"
#include "texture/texturewriter.h"

#include "world/worldreader.h"
#include "world/worldwriter.h"

#ifdef WIN32
#ifdef MOD_EXPORTS
#define MOD_API __declspec(dllexport)
#else
#define MOD_API __declspec(dllimport)
#endif
#else
#define MOD_API
#endif

using namespace c2d;

// {569B9122-695E-46D5-A1F6-EC808010572D}
static const Uuid EffectUUID(0x569B9122, 0x695E, 0x46D5, 0xA1F6, 0xec808010572d);

// {89E326B3-1606-4137-BA81-8EBD5A158EEC}
static const Uuid EntityUUID(0x89E326B3, 0x1606, 0x4137, 0xBA81, 0x8EBD5A158EEC);

// {EEDD2CF4-ABA2-4831-9966-C14FE9F1CDFC}
static const Uuid TextureUUID(0xEEDD2CF4, 0xABA2, 0x4831, 0x9966, 0xC14FE9F1CDFC);

// {54034D72-41BF-4DC5-A2B8-F4DB28B3397E}
static const Uuid WorldUUID(0x54034d72, 0x41bf, 0x4dc5, 0xa2b8, 0xf4db28b3397e);

extern "C" MOD_API ModuleCollection* cdecl getModuleCollection()
{
   ModuleCollection* pmodules = new ModuleCollection();

   ContentModule* peffectmodule = new ContentModule(EffectUUID);
   peffectmodule->setSupportedFiles(UTEXT("fx"));
   peffectmodule->setReader(new EffectReader());
   peffectmodule->setWriter(new EffectWriter());
   pmodules->add(peffectmodule);

   ContentModule* pentitymod = new ContentModule(EntityUUID);
   pentitymod->setSupportedFiles(UTEXT("xml"));
   pentitymod->setReader(new EntityReader());
   pentitymod->setWriter(new EntityWriter());
   pmodules->add(pentitymod);

   ContentModule* ptexturemod = new ContentModule(TextureUUID);
   ptexturemod->setSupportedFiles(UTEXT("dds"));
   ptexturemod->setReader(new TextureReader());
   ptexturemod->setWriter(new TextureWriter());
   pmodules->add(ptexturemod);

   ContentModule* pworldmod = new ContentModule(WorldUUID);
   pworldmod->setSupportedFiles(UTEXT("world"));
   pworldmod->setReader(new WorldReader());
   pworldmod->setWriter(new WorldWriter());
   pmodules->add(pworldmod);

   return pmodules;
}
