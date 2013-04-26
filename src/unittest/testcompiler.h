
#ifndef TEST_COMPILER_H
#define TEST_COMPILER_H

#include <cxxtest/TestSuite.h>

#include "core/vfs/filesystem.h"

#include "script/compiler/compiler.h"
#include "script/compiler/compilecallback.h"
#include "script/vm/vminterface.h"
#include "script/vm/stackcpu/stackcpu.h"

class TestCompiler : public CxxTest::TestSuite
{
   class TestCompilerCallback : public CompileCallback
   {
      TestCompiler& mTest;
   public:
      TestCompilerCallback(TestCompiler& test): mTest(test)
      {
      }

      virtual void notify(VirtualClass* pclass)
      {
         mTest.loaded();
      }
   };

public:
   
   void testCompilation()
   {
      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath("../scripts");
      fs.addPath("../src/unittest/tests");

      ClassRegistry registry;
      VMInterface::registerCommonFunctions(registry);

      StackCPU cpu;

      Compiler compiler;
      compiler.setByteCodeGenerator(cpu.createIRGenerator());
      compiler.setClassRegistry(registry);

      // compiler required classes
      TS_ASSERT(compiler.compile("system.Object"));
      TS_ASSERT(compiler.compile("system.InternalArray"));
      TS_ASSERT(compiler.compile("system.InternalString"));
      TS_ASSERT(compiler.compile("system.ClassLoader"));
      TS_ASSERT(compiler.compile("system.System"));
   }

   void testCallback()
   {
      TestCompilerCallback callback(*this);

      mLoaded = false;

      Compiler compiler;
      compiler.setCallback(callback);
      compiler.compile("system.Object");

      TS_ASSERT(mLoaded);
   }

   void loaded()
   {
      mLoaded = true;
   }

private:

   bool mLoaded;
};

#endif // TEST_COMPILER_H
