
#include "modulemanager.h"

#include <algorithm>

#include "core/log/log.h"
#include "core/smartptr/autoptr.h"
#include "core/system/platform.h"
#include "core/system/exception.h"
#include "core/vfs/filesystem.h"

#include "module.h"

namespace c2d
{
   ModuleManager::ModuleManager() :
      mModules()
   {
   }

   ModuleManager::~ModuleManager()
   {
      deinitialize();
   }

   // - Initialization

   bool ModuleManager::initialize()
   {
      std::vector<String> filenames;
      FileSystem& fs = FileSystem::getInstance();
   #if defined(WIN32)
      if ( !fs.find(UTEXT("mod_*.dll"), filenames, false) )
   #else
      if ( !fs.find(UTEXT("libmod_*.so"), filenames, false) )
   #endif
      {
         return false;
      }

      // load all modules
      Log::getInstance().info("Loading modules...");

      auto append = [this](const String& val){ add(val); };
      std::for_each(filenames.begin(), filenames.end(), append);

      return true;
   }

   void ModuleManager::deinitialize()
   {
      clear();

      auto close = [](void* pmodule) { c2d::Platform::getInstance().freeModule(pmodule); };
      std::for_each(mHandles.begin(), mHandles.end(), close);
   }

   // - Query

   ModuleCollection ModuleManager::filter(ModuleKind kind)
   {
      return mModules.filter(kind);
   }

   // - Maintenance

   void ModuleManager::exec(PGETMODULE pfunc)
   {
      ASSERT_PTR(pfunc);
      std::unique_ptr<Module> module((*pfunc)());
      if ( !module )
      {
         throw c2d::Exception(UTEXT("Could not get the module handle."));
      }
      add(module.release());
   }

   void ModuleManager::exec(PGETMODULECOLLECTION pfunc)
   {
      std::unique_ptr<ModuleCollection> collection((*pfunc)());
      if ( collection )
      {
         add(*collection);
      }
   }

   void ModuleManager::add(const String& filename)
   {
      Log::getInstance().info(UTEXT("Loading module ") + filename);
      
      Platform& platform = c2d::Platform::getInstance();
      void* pmodule = platform.loadModule(filename);
      if ( pmodule != NULL )
      {
         mHandles.push_back(pmodule);

         PGETMODULE pfunc = (PGETMODULE)platform.getFunctionAddress(pmodule, UTEXT("getModule"));
         if ( pfunc != NULL )
         {
            AutoPtr<Module> module = (*pfunc)();
            if ( !module.hasPointer() )
            {
               throw c2d::Exception(UTEXT("Could not get the module handle."));
            }

            add(module.release());
         }
         else
         {
            PGETMODULECOLLECTION pcolfunc = (PGETMODULECOLLECTION)platform.getFunctionAddress(pmodule, UTEXT("getModuleCollection"));
            if ( pcolfunc != NULL )
            {
               AutoPtr<ModuleCollection> collection = (*pcolfunc)();
               if ( collection.hasPointer() )
               {
                  add(*collection);
               }
            }
         }
      }
   }

   void ModuleManager::add(Module* pmodule)
   {
      ASSERT_PTR(pmodule);
      pmodule->setModuleManager(*this);
      mModules.add(pmodule);
   }

   void ModuleManager::add(ModuleCollection& collection)
   {
      ModuleCollectionIterator it = collection.getIterator();
      for ( ; it.isValid(); ++it )
      {
         Module& module = *it;
         add(&module);
      }
   }

   void ModuleManager::clear()
   {
      ModuleCollectionIterator it = mModules.getIterator();
      for ( ; it.isValid(); ++it )
      {
         Module& module = *it;
         delete &module;
      }
      mModules.clear();
   }

   // - Query

   Module* ModuleManager::lookup(const Uuid& uuid)
   {
      return mModules[uuid];
   }
}
