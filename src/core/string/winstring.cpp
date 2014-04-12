
#include "winstring.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iconv.h>

uint32_t conv_utf8_to_utf16(wchar_t* pdest, uint32_t size, const char* psrc, uint32_t length)
{
   if ( length == 0 )
   {
      pdest[0] = 0;
      return 0;
   }

   iconv_t cd;
   cd = iconv_open("WCHAR_T", "UTF-8");
   if ( cd == (iconv_t)-1 )
   {
      pdest[0] = 0;
      return 0;
   }

   uint32_t inleft = length;
   uint32_t outleft = size * sizeof(wchar_t);
   const char* pinput = psrc;
   char* poutput = (char*) pdest;
   size_t converted = iconv (cd, (const char **) &pinput, &inleft, (char**) &poutput, &outleft);
   if ( converted == (size_t) -1 )
   {
      if ( errno == EINVAL )
      {
          // invalid sequence, we ignore the rest
      }
      else
      {

      }
   }

   iconv(cd, NULL, NULL, &poutput, &outleft);
   iconv_close(cd);

   *((wchar_t *) poutput) = L'\0';

   return (wchar_t *) poutput - pdest;
   /*
   int len = MultiByteToWideChar(CP_ACP, 0, psrc, length, pdest, size);

   if ( len <= 0 )
   {
      DWORD err = ::GetLastError();
      switch ( err )
      {
      case ERROR_INSUFFICIENT_BUFFER:
         return -1;
      case ERROR_INVALID_FLAGS:
         return -2;
      case ERROR_INVALID_PARAMETER:
         return -3;
      case ERROR_NO_UNICODE_TRANSLATION:
         return -4;
      default:
         return -5;
      }
      len = 0;
   }

   if ( len <= length )
   {
      pdest[len] = 0;
   }
   else
   {
      pdest[length] = 0;
   }

   return len; */
}

char* conv_utf16_to_utf8(const wchar_t* psrc, uint32_t length)
{
   iconv_t cd;
   cd = iconv_open("UTF-8", "WCHAR_T");
   if ( cd == (iconv_t)-1 )
   {
      return NULL;
   }

   uint32_t inleft = length * sizeof(wchar_t);
   uint32_t outleft = inleft;

   char* presult = new char[outleft + 1];

   const wchar_t* pinput = psrc;
   char* poutput = presult;

   do
   {
      errno = 0;

      size_t converted = iconv (cd, (const char **) &pinput, &inleft, (char**) &poutput, &outleft);
      if ( converted == (size_t)-1 )
      {
         if ( errno == E2BIG )
         {
            // need to resize the buffer
         }
         else
         {
            break;
         }
      }
      else
      {
         break;
      }
   }
   while ( 1 );

   iconv(cd, NULL, NULL, &poutput, &outleft);
   iconv_close(cd);

   *poutput = '\0';

   return presult;

   /*
   int utf8len = WideCharToMultiByte(CP_UTF8, 0, psrc, length, NULL, 0, NULL, NULL);

   if ( utf8len <= 0 )
   {
      return NULL;
   }

   char* presult = new char[utf8len + 1]; // include eos

   WideCharToMultiByte(CP_UTF8, 0, psrc, length, presult, utf8len, NULL, NULL);

   presult[utf8len] = 0;

   return presult;*/
}
