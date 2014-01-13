
#ifndef __YasScriptRegistrator_h__
#define __YasScriptRegistrator_h__

#include "core/script/scriptregistrator.h"

#include "script/common/classregistry.h"
#include "script/script_base.h"

class YasScriptManager;

class SCRIPT_API YasScriptRegistrator : public c2d::ScriptRegistrator
{
public:
   YasScriptRegistrator(YasScriptManager& manager);

   virtual void addClass(const String& name) override;
   
   virtual void addFunction(const String& prototype, CallbackFnc callback) override;

   virtual void registerCallbacks(c2d::ScriptManager& manager) override;

private:

   YasScriptManager& mScriptManager;
   ClassRegistry     mRegistry;
};

#endif // __YasScriptRegistrator_h__
