
#ifndef AST_CLASS_H_
#define AST_CLASS_H_

#include <map>
#include <vector>

#include "core/string/string.h"

#include "script/compiler/functiontable.h"
#include "script/compiler/classresolver.h"
#include "script/common/literaltable.h"

#include "astnode.h"
#include "astmodifier.h"
#include "asttypelist.h"
#include "astfunctionmap.h"

class ASTMember;
class ASTFunction;
class ASTField;
class ASTType;
class ASTTypeList;
class ASTTypeVariables;
class Scope;
class Signature;
class CompileContext;

class ASTClass : public ASTNode
{
public:
   enum SearchScope { eLocal, eAll };
   enum Kind { eClass, eInterface };
   enum State { eParsed, eLoaded, eCompiled };

   typedef std::vector<ASTField*> Fields;

   ASTClass(); 
   virtual ~ASTClass();

 // get/set
   Kind getKind() const;
   void setKind(Kind kind);

   bool           hasBaseType() const;
   const ASTType& getBaseType() const;
         ASTType& getBaseType();
   void           setBaseType(ASTType* pbasetype);

   bool            hasBaseClass() const;
   const ASTClass& getBaseClass() const;
         ASTClass& getBaseClass();
   void            setBaseClass(ASTClass& baseclass);

   const String& getName() const;
   void          setName(const String& name);

   const String& getFullName() const;
   void          setFullName(const String& name);

   const ASTModifiers& getModifiers() const;
         ASTModifiers& getModifiers();

   const ASTTypeVariables& getTypeVariables() const;
         ASTTypeVariables& getTypeVariables();
   void                    setTypeVariables(ASTTypeVariables* ptypes);

   const FunctionTable& getFunctionTable() const;
         FunctionTable& getFunctionTable();

   const ASTTypeList& getInterfaces() const;
         ASTTypeList& getInterfaces();

   const Fields& getStatics() const;
   const Fields& getFields() const;

   State getState() const;
   void setState(State state) const;

 // query
   bool isBase(const ASTClass& base) const;
   bool isImplementing(const ASTClass& intrface) const;
   bool isLocal(const ASTFunction& function) const;
   bool isMember(const String& name) const;
   bool isNative() const;

   bool isGeneric() const;
   bool isTypeName(const String& name) const;

   bool hasConstructor() const;
   bool hasAbstractFunction() const;
   bool hasNativeFunction() const;
   bool hasNativeConstructor() const;

   int getTotalStatics() const;
   int getTotalVariables() const;

 // operations
   void addInterface(ASTType* pinterfacetype);
   void addMember(ASTMember* pmember);

   const ClassResolver& getResolver() const;
   void setResolver(const ClassResolver& resolver);

   void registerVariables(Scope& scope) const;

   void calculateResources();

 // search
   const ASTField* findStatic(const String& name, SearchScope scope = eAll) const;
         ASTField* findStatic(const String& name, SearchScope scope = eAll);

   const ASTField* findField(const String& name, SearchScope scope = eAll) const;
         ASTField* findField(const String& name, SearchScope scope = eAll);

   const ASTFunction* findBestMatch(const String& name, const Signature& signature, const ASTTypeList& types) const;
         ASTFunction* findBestMatch(const String& name, const Signature& signature, const ASTTypeList& types);

   const ASTFunction* findExactMatch(const String& name, const Signature& signature) const;
         ASTFunction* findExactMatch(const String& name, const Signature& signature);

 // visitor
   ACCEPT;

private:
 // operations
   void indexStatics();
   void indexVariables();
   void indexFunctions();

 // search
   ASTFunction* findExactMatchLocal(const String& name, const Signature& signature);

   Kind              mKind;
   ClassResolver     mResolver;
   ASTModifiers      mModifiers;
   ASTType*          mpBaseType;
   ASTTypeList       mInterfaces;
   String            mName;
   String            mFullName;
   ASTTypeVariables* mpTypeVariables;
   FunctionTable     mTable;
   Fields            mStatics;
   Fields            mFields;
   ASTFunctionMap    mFunctions;
   mutable State     mState;
};

#endif // AST_CLASS_H_
