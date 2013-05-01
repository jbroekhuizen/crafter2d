
#include "stackirgenerator.h"

#include <vector>
#include <stack>
#include <string.h>

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "script/cil/cil.h"

#include "script/ast/ast.h"
#include "script/bytecode/block.h"
#include "script/bytecode/program.h"
#include "script/bytecode/symboltable.h"
#include "script/bytecode/functionsymbol.h"
#include "script/bytecode/valuesymbol.h"
#include "script/bytecode/jumppatch.h"
#include "script/bytecode/block.h"
#include "script/bytecode/instruction.h"
#include "script/common/variant.h"
#include "script/common/classregistration.h"
#include "script/compiler/compilecontext.h"
#include "script/compiler/functionresolver.h"
#include "script/vm/virtualstring.h"

struct OpcodeInfo
{
   int push;
   int pop;
};

#define OPCODE(a,b,c) { b, c },
const OpcodeInfo stackinfo[SBIL::SBIL_last + 1] = {
#include "sbil.def"
};

StackIRGenerator::StackIRGenerator():
   ByteCode::IRGenerator()
{
}

int StackIRGenerator::generate(CompileContext& context, const ASTFunction& function)
{
   if ( function.getName() == "add")
   {
      int aap = 5;
   }
   generateInstructions(context, context.getProgram(), function);
   checkAndFixStack(context.getProgram(), function);
   int index = buildCode(context.getProgram(), function);
   cleanup();
   return index;
}

void StackIRGenerator::generateInstructions(CompileContext& context, ByteCode::Program& program, const ASTFunction& function)
{
   using namespace CIL;
   using namespace SBIL;
   using namespace ByteCode;

   FunctionResolver resolver;

   if ( function.getName() == "indexOf" )
   {
      int aap = 5;
   }

   std::stack<AutoPtr<ASTType>> types;

   const CIL::Instructions& instructions = function.getInstructions();
   if ( instructions.empty() )
      return;

   buildBlocks(context, instructions);
   Block* pblock;

   for ( unsigned index = 0; index < instructions.size(); ++index )
   {
      const CIL::Instruction& inst = instructions[index];

      if ( hasBlock(index) )
         pblock = getBlock(index);

      switch ( inst.opcode )
      {
         case CIL_nop:
         case CIL_label:
            // should not get here
            break;

         case CIL_dup:
            {
               types.push(types.top()->clone());
               INSERT(SBIL_dup, 0);
            }
            break;
         
         case CIL_new:
            {
               ASTFunction& constructor = resolver.resolve(context, *inst.mString);
               ASSERT(constructor.isConstructor());

               FunctionSymbol* psymbol = new FunctionSymbol();
               psymbol->klass = & context.resolveVirtualClass(function.getClass().getFullName());
               psymbol->func = function.getResourceIndex();
               psymbol->args = constructor.getArgumentCount() - 1;   // object is created in this instruction
               psymbol->returns = !constructor.getType().isVoid();
               int i = program.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < psymbol->args; ++arg )
                  types.pop();

               types.push(function.getType().clone());

               INSERT(SBIL_new, i);
            }
            break;
         case CIL_newarray:
            {
               ASTType* ptype = ASTType::fromString(*inst.mString);

               int depth = ptype->getArrayDimension();
               for ( int arg = 0; arg < depth; ++arg )
                  types.pop();

               types.push(ptype);

               INSERT(SBIL_new_array, depth);
            }
            break;

         // Calls

         case CIL_call:
            {
               ASTFunction& func = resolver.resolve(context, *inst.mString);

               FunctionSymbol* psymbol = new FunctionSymbol();
               psymbol->func = func.getResourceIndex();
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getType().isVoid();
               int i = program.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < func.getArguments().size(); ++arg )
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getType().clone());

               INSERT(SBIL_call, i);
            }
            break;
         case CIL_call_interface:
            {
               ASTFunction& func = resolver.resolve(context, *inst.mString);

               FunctionSymbol* psymbol = new FunctionSymbol();
               psymbol->func = func.getResourceIndex();
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getType().isVoid();
               int i = program.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < func.getArguments().size(); ++arg )
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getType().clone());

               INSERT(SBIL_call_interface, i);
            }
            break;
         case CIL_call_static:
            {
               ASTFunction& func = resolver.resolve(context, *inst.mString);

               FunctionSymbol* psymbol = new FunctionSymbol;
               psymbol->klass = &context.resolveVirtualClass(func.getClass().getFullName());
               psymbol->func = func.getResourceIndex();
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getType().isVoid();
               int i = program.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < func.getArguments().size(); ++arg )
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getType().clone());
               
               INSERT(SBIL_call_static, func.getResourceIndex());
            }
            break;
         case CIL_call_native:
            {
               const FunctionRegistration& funcreg = context.getClassRegistry().getFunction(inst.mInt);
               ASTFunction& func = resolver.resolve(context, funcreg.getClassRegistration().name + "." + funcreg.getPrototype());

               FunctionSymbol* psymbol = new FunctionSymbol;
               psymbol->func = inst.mInt;
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getType().isVoid();
               int i = program.getSymbolTable().add(psymbol);

               int args = func.getArgumentCount();
               for ( int arg = 0; arg < args; ++arg )
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getType().clone());

               INSERT(SBIL_call_native, i);
            }
            break;
         case CIL_ret:
            INSERT(SBIL_ret, inst.mInt);
            break;

         // Math

         case CIL_add:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_iadd, 0);
                     types.push(new ASTType(ASTType::eInt));
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_radd, 0);
                     types.push(new ASTType(ASTType::eReal));
                     break;
                  case ASTType::eString:
                     INSERT(SBIL_sadd, 0);
                     types.push(new ASTType(ASTType::eString));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_sub:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_isub, 0);
                     types.push(new ASTType(ASTType::eInt));
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rsub, 0);
                     types.push(new ASTType(ASTType::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_mul:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_imul, 0);
                     types.push(new ASTType(ASTType::eInt));
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rmul, 0);
                     types.push(new ASTType(ASTType::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_div:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_idiv, 0);
                     types.push(new ASTType(ASTType::eInt));
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rdiv, 0);
                     types.push(new ASTType(ASTType::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_neg:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_ineg, 0);
                     types.push(new ASTType(ASTType::eInt));
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rneg, 0);
                     types.push(new ASTType(ASTType::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_rem:
            INSERT(SBIL_irem, 0);
            types.push(new ASTType(ASTType::eInt));
            break;
         case CIL_shl:
            INSERT(SBIL_shl, 0);
            types.push(new ASTType(ASTType::eInt));
            break;
         case CIL_shr:
            INSERT(SBIL_shr, 0);
            types.push(new ASTType(ASTType::eInt));
            break;

         case CIL_xor:
            INSERT(SBIL_xor, 0);
            break;
         case CIL_and:
            INSERT(SBIL_and, 0);
            break;
         case CIL_or:
            INSERT(SBIL_or, 0);
            break;
         case CIL_not:
            INSERT(SBIL_not, 0); // push & pop a boolean
            break;

         case CIL_cmpeq:
            {
               AutoPtr<ASTType> ptype = types.top().release();
               types.pop();
               types.pop();

               switch ( ptype->getKind() )
               {
                  case ASTType::eBoolean:
                     INSERT(SBIL_bcmpeq, 0);
                     break;
                  case ASTType::eInt:
                     INSERT(SBIL_icmpeq, 0);
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rcmpeq, 0);
                     break;
                  case ASTType::eChar:
                     INSERT(SBIL_ccmpeq, 0);
                     break;
                  case ASTType::eString:
                     INSERT(SBIL_scmpeq, 0);
                     break;
                  case ASTType::eGeneric:
                  case ASTType::eObject:
                     INSERT(SBIL_ocmpeq, 0);
                     break;
                  case ASTType::eArray:
                     INSERT(SBIL_acmpeq, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }

               types.push(new ASTType(ASTType::eBoolean));
            }
            break;
         case CIL_cmpne:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eBoolean:
                     INSERT(SBIL_bcmpne, 0);
                     break;
                  case ASTType::eInt:
                     INSERT(SBIL_icmpne, 0);
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rcmpne, 0);
                     break;
                  case ASTType::eChar:
                     INSERT(SBIL_ccmpne, 0);
                     break;
                  case ASTType::eString:
                     INSERT(SBIL_scmpne, 0);
                     break;
                  case ASTType::eGeneric:
                  case ASTType::eObject:
                     INSERT(SBIL_ocmpeq, 0);
                     INSERT(SBIL_not, 0);
                     break;
                  case ASTType::eArray:
                     INSERT(SBIL_acmpeq, 0);
                     INSERT(SBIL_not, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.push(new ASTType(ASTType::eBoolean));
            }
            break;
         case CIL_cmpgt:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_icmpgt, 0);
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rcmpgt, 0);
                     break;
                  case ASTType::eChar:
                     INSERT(SBIL_ccmpgt, 0);
                     break;
                  case ASTType::eString:
                     INSERT(SBIL_scmpgt, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.push(new ASTType(ASTType::eBoolean));
            }
            break;
         case CIL_cmpge:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_icmpge, 0);
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rcmpge, 0);
                     break;
                  case ASTType::eChar:
                     INSERT(SBIL_ccmpge, 0);
                     break;
                  case ASTType::eString:
                     INSERT(SBIL_scmpge, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.push(new ASTType(ASTType::eBoolean));
            }
            break;
         case CIL_cmple:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_icmple, 0);
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rcmple, 0);
                     break;
                  case ASTType::eChar:
                     INSERT(SBIL_ccmple, 0);
                     break;
                  case ASTType::eString:
                     INSERT(SBIL_scmple, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.push(new ASTType(ASTType::eBoolean));
            }
            break;
         case CIL_cmplt:
            {
               AutoPtr<ASTType> type = types.top().release();
               types.pop();
               types.pop();

               switch ( type->getKind() )
               {
                  case ASTType::eInt:
                     INSERT(SBIL_icmplt, 0);
                     break;
                  case ASTType::eReal:
                     INSERT(SBIL_rcmplt, 0);
                     break;
                  case ASTType::eChar:
                     INSERT(SBIL_ccmplt, 0);
                     break;
                  case ASTType::eString:
                     INSERT(SBIL_scmplt, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.push(new ASTType(ASTType::eBoolean));
            }
            break;
         case CIL_isnull:
            INSERT(SBIL_isnull, 0);
            types.push(new ASTType(ASTType::eBoolean));
            break;

         case CIL_jump:
            INSERT(SBIL_jump, 0);
            break;
         case CIL_jump_true:
            ASSERT(types.top()->isBoolean());
            INSERT(SBIL_jump_true, 0);
            types.pop();
            break;
         case CIL_jump_false:
            ASSERT(types.top()->isBoolean());
            INSERT(SBIL_jump_false, 0);
            types.pop();
            break;

         case CIL_ldint:
            if ( inst.mInt == 0 )
            {
               INSERT(SBIL_push_i0, 0);
            }
            else if ( inst.mInt == 1 )
            {
               INSERT(SBIL_push_i1, 0);
            }
            else if ( inst.mInt == 2 )
            {
               INSERT(SBIL_push_i2, 0);
            }
            else
            {
               INSERT(SBIL_pushi, inst.mInt);
            }
            types.push(new ASTType(ASTType::eInt));
            break;
         case CIL_ldreal:
            if ( inst.mReal == 0.0 )
            {
               INSERT(SBIL_push_r0, 0);
            }
            else if ( inst.mReal == 1.0 )
            {
               INSERT(SBIL_push_r1, 0);
            }
            else if ( inst.mReal == 2.0 )
            {
               INSERT(SBIL_push_r2, 0);
            }
            else
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setReal(inst.mReal);
               int i = program.getSymbolTable().add(psymbol);

               INSERT(SBIL_push, i);
            }
            types.push(new ASTType(ASTType::eReal));
            break;
         case CIL_ldchar:
            INSERT(SBIL_pushc, inst.mInt);
            types.push(new ASTType(ASTType::eChar));
            break;
         case CIL_ldstr:
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(VirtualString(*inst.mString));
               int i = program.getSymbolTable().add(psymbol);

               INSERT(SBIL_push, i);
               types.push(new ASTType(ASTType::eString));
            }
            break;
         case CIL_ldtrue:
            INSERT(SBIL_push_true, 0);
            types.push(new ASTType(ASTType::eBoolean));
            break;
         case CIL_ldfalse:
            INSERT(SBIL_push_false, 0);
            types.push(new ASTType(ASTType::eBoolean));
            break;
         case CIL_ldclass:
            {
               ASTType* ptype = new ASTType(ASTType::eObject);
               ptype->setObjectName("system.Class");
               ptype->setObjectClass(context.resolveClass("system.Class"));

               INSERT(SBIL_push_class, 0);
               types.push(ptype);
            }
            break;
         case CIL_ldnull:
            INSERT(SBIL_push_null, 0);
            types.push(new ASTType(ASTType::eNull));
            break;

         case CIL_ldfield:
            {
               AutoPtr<ASTType> type = types.top().release();
               ASSERT(type->getKind() == ASTType::eObject || type->getKind() == ASTType::eArray);
               types.pop();

               const ASTClass& klass = type->getObjectClass();
               if ( klass.getFullName() == "system.InternalArray" )
               {
                  // load length attribute of the array
                  INSERT(SBIL_ldfield, 0);
                  types.push(new ASTType(ASTType::eInt));
               }
               else
               {
                  const ASTField& field = *klass.getFields()[inst.mInt];
                  INSERT(SBIL_ldfield, inst.mInt);
                  types.push(field.getVariable().getType().clone());
               }
            }
            break;
         case CIL_stfield:
            INSERT(SBIL_stfield, inst.mInt);
            types.pop(); // value
            types.pop(); // object
            break;
         case CIL_ldarg:
            {
               const ASTType& argtype = function.getArguments()[inst.mInt];
               INSERT(SBIL_ldlocal, inst.mInt);
               types.push(argtype.clone());
            }
            break;
         case CIL_starg:
            INSERT(SBIL_stlocal, inst.mInt);
            types.pop();
            break;
         case CIL_ldloc:
            {
               const ASTType& asttype = function.getLocals()[inst.mInt];
               INSERT(SBIL_ldlocal, inst.mInt + function.getArguments().size());
               types.push(asttype.clone());
            }
            break;
         case CIL_stloc:
            INSERT(SBIL_stlocal, inst.mInt + function.getArguments().size());
            types.pop();
            break;
         case CIL_ldelem:
            {
               // pop indices
               for ( int ei = 0; ei < inst.mInt; ++ ei )
                  types.pop();
            
               INSERT(SBIL_ldelem, inst.mInt);
               
               AutoPtr<ASTType> type = types.top();
               types.pop();                              // pop array
               types.push(type->getArrayType().clone()); // push element
            }
            break;
         case CIL_stelem:
            {
               // pop indices
               for ( int ei = 0; ei < inst.mInt; ++ei )
                  types.pop();

               INSERT(SBIL_stelem, inst.mInt);
               types.pop();   // pop element
               types.pop();   // pop array
            }
            break;
         case CIL_ldstatic:
            {
               const ASTField& field = *function.getClass().getStatics()[inst.mInt];
               INSERT(SBIL_ldstatic, inst.mInt);
               types.push(field.getVariable().getType().clone());
            }
            break;
         case CIL_ststatic:
            INSERT(SBIL_ststatic, inst.mInt);
            types.pop();
            break;
      }
   }
}

void StackIRGenerator::checkAndFixStack(const ByteCode::Program& program, const ASTFunction& function)
{
   using namespace SBIL;
   using namespace ByteCode;

   typedef std::vector<Instruction*> Calls;
   Calls calls;

   Blocks& blocks = getBlocks();

   // fill the stack
   for ( std::size_t index = 0; index < blocks.size(); ++index )
   {
      if ( !hasBlock(index) )
         continue;
         
      Block* pblock = blocks[index];

      Instruction* pinst = pblock->pstart;
      while ( pinst != NULL )
      {
         int opcode = INST_OPCODE(pinst->inst);

         switch ( opcode ) 
         {
            case SBIL_call:
            case SBIL_call_interface:
            case SBIL_call_static:
               {
                  int instarg = INST_ARG(pinst->inst);
                  const FunctionSymbol& symbol = static_cast<const FunctionSymbol&>(program.getSymbolTable()[instarg]);
                  for ( int arg = 0; arg < symbol.args; ++arg )
                     calls.pop_back();

                  // push return value
                  if ( symbol.returns )
                     calls.push_back(pinst);
               }
               break;
            default:
               {
                  int pop = stackinfo[opcode].pop;
                  for ( int i = 0; i < pop; ++i )
                     calls.pop_back();

                  if ( stackinfo[opcode].push == 1 )
                     calls.push_back(pinst);
               }
         }

         pinst = pinst->next;
      }
   }

   // now see what indices are still there that need an additional pop
   // skip the return value at the top of the stack

   std::size_t size = calls.size() - function.getType().isVoid() ? 0 : 1;
   for ( std::size_t index = 0; index < size; ++index )
   {
      Instruction* inst = new Instruction;
      inst->inst = MAKE_INST(SBIL_pop,0);
      inst->next = calls[index]->next;

      calls[index]->next = inst;
   }
}

int StackIRGenerator::buildCode(ByteCode::Program& program, const ASTFunction& function)
{
   using namespace SBIL;
   using namespace ByteCode;

   unsigned int pos = 0;
   unsigned int size = 32;
   char* pcode = (char*)malloc(size);
   memset(pcode, 0, size);

   Blocks& blocks = getBlocks();

   // fill the stack
   for ( std::size_t index = 0; index < blocks.size(); ++index )
   {
      if ( !hasBlock(index) )
         continue;

      Block* pblock = blocks[index];
      pblock->codepos = pos;

      Instruction* pinst = pblock->pstart;
      while ( pinst != NULL )
      {
         int opcode = INST_OPCODE(pinst->inst);
         if ( opcode == SBIL_jump || opcode == SBIL_jump_true || opcode == SBIL_jump_false )
         {
            JumpPatch* ppatch = new JumpPatch();
            ppatch->pos = pos;
            ppatch->to = pblock->to[0];

            addPatch(ppatch);
         }

         *((int*)&pcode[pos]) = pinst->inst;

         pos += sizeof(int);
         if ( pos > size )
         {
            size *= 2;
            pcode = (char*)realloc(pcode, size);
         }

         pinst = pinst->next;
      }
   }

   applyPatches(pcode);
   
   int index = program.linkCode(pcode, pos);

   free(pcode);

   return index;
}
