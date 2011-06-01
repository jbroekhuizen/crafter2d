
#ifndef VIRTUAL_FUNCTION_TABLE_ENTRY_H_
#define VIRTUAL_FUNCTION_TABLE_ENTRY_H_

#include <string>

class VirtualFunctionTableEntry
{
public:
   VirtualFunctionTableEntry():
      mName(),
      mInstruction(-1),
      mOriginalInstruction(-1),
      mArguments(0),
      mInterface(-1)
   {
   }

   VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that): 
      mName(that.mName),
      mInstruction(that.mInstruction),
      mOriginalInstruction(that.mOriginalInstruction),
      mArguments(that.mArguments),
      mInterface(that.mInterface)
   {
   }

   VirtualFunctionTableEntry* clone() const {
      return new VirtualFunctionTableEntry(*this);
   }

   std::string mName;
   int         mInstruction;
   int         mOriginalInstruction;
   int         mArguments;
   int         mInterface;
};

#endif // VIRTUAL_FUNCTION_TABLE_ENTRY_H_