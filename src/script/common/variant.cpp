
#include "variant.h"

#include "core/defines.h"
#include "core/conv/lexical.h"
#include "core/conv/numberconverter.h"

#include "script/vm/virtualarray.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualclass.h"

// - Variant

Variant::Variant():
   mType(eEmpty),
   mpHolder(NULL)
{
}

Variant::Variant(const Variant& that):
   mType(that.mType),
   mpHolder(isEmpty() ? NULL : that.mpHolder->clone())
{
}

Variant::Variant(int value):
   mType(eInt),
   mpHolder(new DataHolder<int>(value))
{
}

Variant::Variant(double value):
   mType(eReal),
   mpHolder(new DataHolder<double>(value))
{
}

Variant::Variant(char value):
   mType(eChar),
   mpHolder(new DataHolder<char>(value))
{
}

Variant::Variant(bool value):
   mType(eBool),
   mpHolder(new DataHolder<bool>(value))
{
}

Variant::Variant(const String& value):
   mType(eString),
   mpHolder(new StringHolder(value))
{
}

Variant::Variant(VirtualObject& object):
   mType(eObject),
   mpHolder(new ObjectHolder(&object))
{
}

Variant::Variant(VirtualObject* pvirtualobject):
   mType(eObject),
   mpHolder(new ObjectHolder(pvirtualobject))
{
}

Variant::Variant(VirtualArray& array):
   mType(eArray),
   mpHolder(new ArrayHolder(&array))
{
}

Variant::~Variant()
{
   delete mpHolder;
   mpHolder = NULL;
}

bool Variant::operator==(const Variant& that) const
{
   if ( mType == that.mType )
   {
      switch ( mType )
      {
         case eEmpty:
            return true;

         case eInt:
            return asInt() == that.asInt();

         case eReal:
            return asReal() == that.asReal();

         case eChar:
            return asChar() == that.asChar();

         case eBool:
            return asBool() == that.asBool();

         case eString:
            return asString() == that.asString();

         case eObject:
            return &asObject() == &that.asObject();

         case eArray:
            return &asArray() == &that.asArray();
      }
   }

   return false;
}

bool Variant::operator!=(const Variant& that) const
{
   return !operator==(that);
}

bool Variant::operator<=(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() <= that.asInt();

      case eReal:
         return asReal() <= that.asReal();

      case eString:
         return asString() <= that.asString();
   }

   return false;
}

bool Variant::operator<(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() < that.asInt();

      case eReal:
         return asReal() < that.asReal();

      case eString:
         return asString() < that.asString();
   }

   return false;
}

bool Variant::operator>(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() > that.asInt();

      case eReal:
         return asReal() > that.asReal();

      case eString:
         return asString() > that.asString();
   }

   return false;
}

bool Variant::operator>=(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() >= that.asInt();

      case eReal:
         return asReal() >= that.asReal();

      case eString:
         return asString() >= that.asString();
   }

   return false;
}

const Variant& Variant::operator=(const Variant& that)
{
   delete mpHolder;
   mType = that.mType;
   mpHolder = (mType == eEmpty) ? NULL : that.mpHolder->clone();

   return *this;
}

// - Getters

int Variant::asInt() const
{
   return ((DataHolder<int>*)mpHolder)->mData;
}

void Variant::setInt(int value)
{
   if ( mType == eInt )
   {
      ((DataHolder<int>*)mpHolder)->mData = value;
   }
   else
   {
      mType = eInt;
      delete mpHolder;
      mpHolder = new DataHolder<int>(value);
   }
}

double Variant::asReal() const
{
   return ((DataHolder<double>*)mpHolder)->mData;
}

void Variant::setReal(double value)
{
   if ( mType == eReal )
   {
      ((DataHolder<double>*)mpHolder)->mData = value;
   }
   else
   {
      mType = eReal;
      delete mpHolder;
      mpHolder = new DataHolder<double>(value);
   }
}

char Variant::asChar() const
{
   ASSERT(mType == eChar);
   return ((DataHolder<char>*)mpHolder)->mData;
}

void Variant::setChar(char value)
{
   if ( mType == eChar )
   {
      ((DataHolder<char>*)mpHolder)->mData = value;
   }
   else
   {
      mType = eChar;
      delete mpHolder;
      mpHolder = new DataHolder<char>(value);
   }
}

bool Variant::asBool() const
{
   return ((DataHolder<bool>*)mpHolder)->mData;
}

void Variant::setBool(bool value)
{
   if ( mType == eBool )
   {
      ((DataHolder<bool>*)mpHolder)->mData = value;
   }
   else
   {
      mType = eBool;
      delete mpHolder;
      mpHolder = new DataHolder<bool>(value);
   }
}

const String& Variant::asString() const
{
   return ((StringHolder*)mpHolder)->mData;
}

void Variant::setString(const String& value)
{
   if ( mType == eString )
   {
      ((StringHolder*)mpHolder)->mData = value;
   }
   else
   {
      mType = eString;
      delete mpHolder;
      mpHolder = new StringHolder(value);
   }
}

VirtualObject& Variant::asObject() const
{
   return *((ObjectHolder*)mpHolder)->mData;
}

void Variant::setObject(VirtualObject& object)
{
   if ( mType == eObject )
   {
      ((ObjectHolder*)mpHolder)->mData = &object;
   }
   else
   {
      mType = eObject;
      delete mpHolder;
      mpHolder = new ObjectHolder(&object);
   }
}

VirtualArray& Variant::asArray() const
{
   return *((ArrayHolder*)mpHolder)->mData;
}

void Variant::setArray(VirtualArray& array)
{
   if ( mType == eArray )
   {
      ((ArrayHolder*)mpHolder)->mData = &array;
   }
   else
   {
      mType = eArray;
      delete mpHolder;
      mpHolder = new ArrayHolder(&array);
   }
}

// - Query

bool Variant::isEmpty() const
{
   return mType == eEmpty;
}

bool Variant::isInt() const
{
   return mType == eInt;
}

bool Variant::isReal() const
{
   return mType == eReal;
}

bool Variant::isChar() const
{
   return mType == eChar;
}

bool Variant::isBool() const
{
   return mType == eBool;
}

bool Variant::isString() const
{
   return mType == eString;
}

bool Variant::isObject() const
{
   return mType == eObject;
}

bool Variant::isArray() const
{
   return mType == eArray;
}

// - Display

String Variant::typeAsString() const
{
   switch ( mType )
   {
      case eString:
         return String("string");
      case eInt:
         return String("int");
      case eReal:
         return String("real");
      case eChar:
         return String("char");
      case eBool:
         return String("boolean");
      case eObject:
         return String(asObject().getClass().getName());
      case eArray:
         return String("array");
   }
   return String();
}

String Variant::toString() const
{
   switch ( mType )
   {
      case eString:
         return asString();
      case eInt:
         return String(lexical_cast<std::string>(asInt()).c_str());
      case eReal:
         return String(lexical_cast<std::string>(asReal()).c_str());
      case eChar:
         return String(lexical_cast<std::string>(asChar()).c_str());
      case eBool:
         return asBool() ? String("true") : String("false");
      case eObject:
         return String(asObject().getClass().getName());
      case eArray:
         return String("array");
   }

   return String();
}

int Variant::toInt() const
{
   switch ( mType )
   {
   case eInt:
      break;
   case eReal:
      return static_cast<int>(asReal());
   case eString:
      return NumberConverter::getInstance().toInt(asString());

   default:
      UNREACHABLE("Can not convert this type to int!");
   }

   return asInt(); // currently no other int types
}

double Variant::toReal() const
{
   switch ( mType )
   {
      case eInt:
         return asInt();
      case eReal:
         return asReal();
   }

   return 0.0;
}

// - Conversion

void Variant::int2real()
{
   ASSERT(isInt());
   setReal((double)asInt());
}

void Variant::real2int()
{
   ASSERT(isReal());
   setInt((int)asReal());
}

