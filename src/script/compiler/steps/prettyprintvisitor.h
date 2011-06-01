
#ifndef PRETTY_PRINT_VISITOR_H_
#define PRETTY_PRINT_VISITOR_H_

#include "script/ast/astvisitor.h"

class PrettyPrintVisitor : public ASTVisitor
{
public:
   PrettyPrintVisitor();

   virtual void visit(const ASTRoot& root);
   virtual void visit(const ASTCompound& compound);
   virtual void visit(const ASTConcatenate& concatenate);
};

#endif // PRETTY_PRINT_VISITOR_H_