
#include "string.h"

#include <string.h>

String::String():
   mString()
{
}

String::String(const char* pdata):
   mString(pdata)
{
}

String::String(const std::string& that):
   mString(that.c_str())
{
}

String::String(const String& that):
   mString(that.mString)
{
}

String::~String()
{
}

const char String::operator[](int index) const
{
   return mString[index];
}

char String::operator[](int index)
{
   return mString[index];
}

const String& String::operator=(const String& that)
{
   mString = that.mString;
   return *this;
}

const String& String::operator=(const char* pstring)
{
   setTo(pstring, strlen(pstring));
   return *this;
}

const String& String::operator=(const std::string& that)
{
   mString = that.c_str();
   return *this;
}

bool String::operator<=(const String& that) const
{
   return mString.compare(that.mString) <= 0;
}

bool String::operator<(const String& that) const
{
   return mString.compare(that.mString) < 0;
}

bool String::operator>(const String& that) const
{
   return mString.compare(that.mString) > 0;
}

bool String::operator>=(const String& that) const
{
   return mString.compare(that.mString) >= 0;
}

const String& String::operator+=(char c)
{
   mString += c;
   return *this;
}

const String& String::operator+=(const String& that)
{
   mString += that.mString;
   return *this;
}

String String::operator+(const String& that) const
{
   String result;
   result.mString = mString + that.mString;
   return result;
}

String String::operator+(char c) const
{
   String result;
   result.mString = mString + c;
   return result;
}

// - Query

bool String::isEmpty() const
{
   return mString.isEmpty() == TRUE;
}

int String::length() const
{
   return mString.length();
}

char* String::toUtf8(int& length) const
{
   length = mString.length();
   char* pdata = new char[length+1];
   memset(pdata, 0, length + 1);

   CheckedArrayByteSink sink(pdata, length);
   mString.toUTF8(sink);

   return pdata;
}

const char* String::getBuffer() const
{
   return (char*) mString.getBuffer();
}

char* String::asArray(char* pdest, int capacity) const
{
   UErrorCode err;
   mString.extract(pdest, capacity, NULL, err);
   return pdest;
}

int String::compare(const String& that) const
{
   return mString.compare(that.mString);
}

int String::hashCode() const
{
   return mString.hashCode();
}

// - Comparison

bool String::operator==(const String& that) const
{
   return (mString == that.mString) == TRUE;
}

bool String::operator!=(const String& that) const
{
   return (mString == that.mString) == FALSE;
}

// - Operations

const String& String::toLower()
{
   mString.toLower();
   return *this;
}

const String& String::toUpper()
{
   mString.toUpper();
   return *this;
}

const String& String::trim()
{
   mString.trim();
   return *this;
}

void String::append(const String& that)
{
   *this += that;
}

void String::setTo(const char* ptext, int length)
{
   StringPiece piece(ptext);
   mString = UnicodeString::fromUTF8(piece);
}

void String::replace(char original, char newtext)
{
   mString.findAndReplace(original, newtext);
}

void String::replace(int start, int length, const String& with)
{
   mString.replace(start, length, with.mString);
}

void String::remove(int start, int count)
{
   mString.remove(start, count);
}

String String::subStr(int start, int count) const
{
   String result;
   mString.extract(start, count, result.mString);
   return result;
}

String String::unescape() const
{
   String result;
   result.mString = mString.unescape();
   return result;
}

// - Searching

int String::indexOf(char character, int start) const
{
   return mString.indexOf(character, start);
}

int String::lastIndexOf(char character) const
{
   return mString.lastIndexOf(character);
}

// - Conversion

std::string String::toStdString() const
{
   UErrorCode status = U_ZERO_ERROR;
   char* pdata = 0;

   int32_t ln = mString.extract(pdata, 0, NULL, status);
   pdata = new char[ln+1];
   status = U_ZERO_ERROR;
   mString.extract(pdata, ln, NULL, status);
   if ( status != U_ZERO_ERROR )
   {
      // bah
   }

   pdata[ln] = 0;
   std::string result = pdata;
   delete[] pdata;

   return result;
}
