
#include "literaltable.h"

#include "literal.h"

LiteralTable::LiteralTable():
   mLiterals()
{
}

const Literal& LiteralTable::operator[](int index) const
{
   return *mLiterals[index];
}

// - Query

bool LiteralTable::contains(const Literal& literal) const
{
   return find(literal.getValue()) != NULL;
}

int LiteralTable::indexOf(const Literal& literal) const
{ 
   return indexOf(literal.getValue());
}

int LiteralTable::indexOf(const Variant& value) const
{
   for ( std::size_t index = 0; index < mLiterals.size(); index++ )
   {
      Literal* pliteral = mLiterals[index];

      if ( pliteral->getValue() == value )
      {
         return index;
      }
   }
   return mLiterals.size();
}

int LiteralTable::size() const
{
   return mLiterals.size();
}

// - Maintenance

int LiteralTable::insert(Literal* pliteral)
{
   int index = indexOf(*pliteral);
   if ( index == mLiterals.size() )
   {
      mLiterals.push_back(pliteral);

      pliteral->setTableIndex(index);
   }
   return index;
}

// - Search

const Literal* LiteralTable::find(const Variant& value) const
{
   return const_cast<LiteralTable*>(this)->find(value);
}

Literal* LiteralTable::find(const Variant& value)
{
   for ( Literals::size_type index = 0; index < mLiterals.size(); index++ )
   {
      Literal* pliteral = mLiterals[index];

      if ( pliteral->getValue() == value )
      {
         return pliteral;
      }
   }
   return NULL;
}
