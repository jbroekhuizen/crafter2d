
#ifndef CONTENT_MODULE_H
#define CONTENT_MODULE_H

#include "core/string/string.h"
#include "core/modules/module.h"
#include "core/system/uuid.h"
#include "core/core_base.h"
#include "core/defines.h"

class IContentReader;
class IContentWriter;

/**
 * A content module is a collection containing a compiler, reader and writer.
 *
 * The compiler is responsible for compiling the human readable file format to the
 * Crafter 2D content file format.
 *
 * @see IContentCompiler
 */

class CORE_API ContentModule : public Module
{
public:
   ContentModule();
   virtual ~ContentModule();

 // get/set
   IContentReader& getReader();
   void            setReader(IContentReader* preader);

   IContentWriter& getWriter();
   void            setWriter(IContentWriter* pwriter);

   const String& getSupportedFiles() const;
   void          setSupportedFiles(const String& supportedfiles);

   const Uuid& getUuid() const;
   void        setUuid(const Uuid& uuid);

 // query
   bool supports(const String& ext) const;

 // operations
   void write(const String& source, const String& dest);

private:

 // data
   Uuid              mUuid;
   IContentReader*   mpReader;
   IContentWriter*   mpWriter;
   String            mSupportedFiles;
};

#endif // CONTENT_MODULE_H
