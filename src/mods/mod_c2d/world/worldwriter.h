
#ifndef WORLD_WRITER_H
#define WORLD_WRITER_H

#include "core/content/contentwriter.h"

class WorldWriter : public c2d::ContentWriter
{
public:

   virtual bool write(DataStream& stream, const String& filename) override;

private:

 // helpers
   String readString(DataStream& stream);
};

#endif // WORLD_WRITER_H