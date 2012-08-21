
#ifndef VIRTUAL_STACK_H
#define VIRTUAL_STACK_H

#include "script/script_base.h"

#include <vector>

#include "script/common/variant.h"

class String;
class VirtualArray;

class SCRIPT_API VirtualStack
{
public:
   explicit VirtualStack(int initialsize = 32);
            ~VirtualStack();

   const Variant& operator[](int index) const;
         Variant& operator[](int index);

 // stack operations
   void push(const Variant& value);
   void pushInt(int value);
   void pushReal(double value);
   void pushBool(bool value);
   void pushChar(char value);
   void pushString(VirtualString& value);
   void pushObject(VirtualObject& object);
   void pushArray(VirtualArray& array);

   Variant              pop();
   void                 pop(int count);
   int                  popInt();
   double               popReal();
   bool                 popBool();
   char                 popChar();
   const String&        popString();
   VirtualObject&       popObject();
   VirtualArray&        popArray();

   void insert(int index, const Variant& value);

 // query
   Variant& back();

   int size() const;

 // operations
   void mark();

private:
   typedef std::vector<Variant*> Stack;

 // operations
   Variant& top();
   void     grow(int amount);

   Stack mStack;
   int   mSize;
};

#endif // VIRTUAL_STACK_H