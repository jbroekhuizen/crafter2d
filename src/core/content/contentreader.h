
#ifndef CONTENT_READER_H
#define CONTENT_READER_H

#include "core/core_base.h"

namespace Graphics
{
   class Device;
}

class IContent;
class ContentManager;
class DataStream;
class Simulator;

class CORE_API ContentReader
{
public:
   ContentReader();

 // get/set
   ContentManager& getContentManager();
   void            setContentManager(ContentManager& manager);

   bool              hasGraphicsDevice() const;
   Graphics::Device& getGraphicsDevice();
   void              setGraphicsDevice(Graphics::Device* device);

   bool       hasPhysicsSimulator() const;
   Simulator& getPhysicsSimulator();
   void       setPhysicsSimulator(Simulator* psimulator);

   /// Reads and instantiates the content from disc
   virtual IContent* read(DataStream& stream) = 0;

private:

 // data
   ContentManager*   mpContentManager;
   Graphics::Device* mpDevice;
   Simulator*        mpSimulator;
};

#endif // CONTENT_READER_H