
#include "codepatch.h"

#include "core/defines.h"

namespace ByteCode
{

   Patch::Patch():
      mOffset(-1)
   {
   }

   // - Get/set

   int Patch::getOffset() const
   {
      return mOffset;
   }

   void Patch::setOffset(int offset)
   {
      mOffset = offset;
   }
}
