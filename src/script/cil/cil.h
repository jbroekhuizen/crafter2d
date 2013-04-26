
#ifndef CIL_H
#define CIL_H

#include <vector>

class String;

namespace CIL
{
   enum Opcode {
      CIL_nop,
      CIL_label,
      CIL_dup,
      CIL_new,
      CIL_newarray,
      CIL_conv_int,
      CIL_conv_real,
      CIL_conv_string,
      CIL_call,
      CIL_call_static,
      CIL_call_native,
      CIL_ret,
      CIL_add,
      CIL_sub,
      CIL_mul,
      CIL_div,
      CIL_neg,
      CIL_rem,
      CIL_shl,
      CIL_shr,
      CIL_xor,
      CIL_or,
      CIL_and,
      CIL_not,
      CIL_cmpeq,
      CIL_cmpne,
      CIL_cmpgt,
      CIL_cmpge,
      CIL_cmple,
      CIL_cmplt,
      CIL_isnull,
      CIL_jump,
      CIL_jump_true,
      CIL_jump_false,
      CIL_ldint,
      CIL_ldstr,
      CIL_ldchar,
      CIL_ldreal,
      CIL_ldtrue,
      CIL_ldfalse,
      CIL_ldclass,
      CIL_ldnull,
      CIL_ldfield,
      CIL_stfield,
      CIL_ldarg,
      CIL_starg,
      CIL_ldloc,
      CIL_stloc,
      CIL_ldelem,
      CIL_stelem,
      CIL_ldstatic,
      CIL_ststatic,
      CIL_switch,
      CIL_instanceof,
      CIL_enter_guard,
      CIL_enter_guard_f,
      CIL_leave_guard,
      CIL_throw
   };

   struct Instruction
   {
      CIL::Opcode opcode;

      union
      {
         int mInt;
         double mReal;
         String* mString;
         void* mPtr;
      };
   };

   typedef std::vector<Instruction> Instructions;
};

#endif // CIL_H
