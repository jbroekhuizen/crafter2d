
#ifndef NUMBER_CONVERTER_H
#define NUMBER_CONVERTER_H

#include <unicode/numfmt.h>

#include "core/core_base.h"

class String;

class CORE_API NumberConverter
{
public:
   static NumberConverter& getInstance();

 // conversion
   int toInt(const String& value);
   double toDouble(const String& value);

   String& format(String& result, int value);
   String& format(String& result, double value);

private:
   NumberConverter();

   NumberFormat* mpFormat;
};

#endif // NUMBER_CONVERTER_H
