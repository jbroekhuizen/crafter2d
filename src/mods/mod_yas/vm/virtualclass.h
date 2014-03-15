
#ifndef VIRTUAL_CLASS_H_
#define VIRTUAL_CLASS_H_

#include <vector>

#include "core/string/string.h"

#include "mod_yas/script_base.h"

#include "virtualclasses.h"
#include "virtualfunctiontable.h"

namespace yasc
{
   class Types;
}

class VirtualField;
class VirtualFunction;
class VirtualValue;
class VirtualObject;

class SCRIPT_API VirtualClass
{
public:
   typedef std::vector<VirtualFunction*> Functions;
   typedef std::vector<VirtualField*> Fields;

   enum Flags {
      eNone = 0, 
      eInstantiatable = 1,
      eNative = 2
   };

   VirtualClass();
   ~VirtualClass();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   bool          hasBaseName() const;
   const String& getBaseName() const;
   void          setBaseName(const String& name);

   bool                hasBaseClass() const;
   const VirtualClass& getBaseClass() const;
   void                setBaseClass(VirtualClass& pbaseclass);

   const VirtualFunctionTable& getVirtualFunctionTable() const;
         VirtualFunctionTable& getVirtualFunctionTable();

   const Fields& getFields() const;
         Fields& getFields();

   const Fields& getStaticFields() const;
         Fields& getStaticFields();

   int getVariableCount() const;
   int getStaticCount() const;
   
   VirtualObject& getClassObject() const;
   void           setClassObject(VirtualObject* pobject);

   const int*  getInterfaceLookupTable() const;
   void        setInterfaceLookupTable(int* ptable);

   Functions& getFunctions();

   void setFlags(Flags flags);

 // query
   bool isNative() const;
   bool canInstantiate() const;

   bool isBaseClass(const VirtualClass& base) const;
   bool implements(const VirtualClass& interfce) const;

   int getTotalVariables() const;

   const VirtualFunctionTableEntry* getDefaultConstructor() const;

 // operations
   void addField(VirtualField* pfield);
   void addStaticField(VirtualField* pfield);
   void addFunction(VirtualFunction* pfunction);
   void addInterface(VirtualClass& klass);

   void collectInterface(VirtualClasses& interfces);

   void build();

   void instantiate(VirtualObject& object) const;

   const VirtualValue& getStatic(int index) const;
         VirtualValue& getStatic(int index);
   void                setStatic(int index, const VirtualValue& value);

   void offsetCode(int offset);

 // search
   VirtualFunction* findExactMatch(const String& name, const yasc::Types& args);
   
private:

 // operations
   void buildVariables();
   void buildVirtualTable();
   void buildInterfaceTable();
   
 // search
   VirtualFunction* findExactMatch(const VirtualFunction& function);

 // data
   String                  mName;
   String                  mBaseName;
   VirtualClass*           mpBaseClass;
   VirtualClasses          mInterfaces;
   Fields                  mFields;
   Fields                  mStaticFields;
   Functions               mFunctions;
   VirtualFunctionTable    mVTable;
   VirtualObject*          mpClassObject;
   VirtualValue*           mpStatics;
   int*                    mpInterfaceLookupTable;
   Flags                   mFlags;
};

#endif // VIRTUAL_CLASS_H_