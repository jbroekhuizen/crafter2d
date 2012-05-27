/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _NETCONNECTION_H_
#define _NETCONNECTION_H_

#include "engine/engine_base.h"

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "core/memory/objectallocator.h"
#include "core/memory/objecthandle.h"

#include "netclients.h"
#include "netpackage.h"
#include "sortedpackagelist.h"

class NetAddress;
class NetStream;
class Process;

const int   MAX_PACKAGE_NUMBER            = 0xAFFFFFFF;
const int   INVALID_CLIENTID              = -1;
const float ALIVE_MSG_INTERVAL            = 2.0f;
const float WAIT_INTERVAL                 = 0.2f;

/// NetConnection
/// Handles the socket connections for the server and the clients. A NetConnection instance
/// can handle one or more simultanious connections with other NetConnection instances via
/// both Internet and a local network.
class ENGINE_API NetConnection
{
   enum SocketError
   {
      eConnReset,
      eConnTimeout,
      eUnsupportedError
   };

   typedef ObjectAllocator<NetPackage> PackageAllocator;

public:
   enum Flags
   {
      eConnected = 1,
      eAccept    = 2,
      eKeepAlive = 4
   };

   explicit    NetConnection(Process& process);
               ~NetConnection();

   static bool initialize();

 // query
   void        setAccepting(bool a);
   void        setSendAliveMessages(bool yes);

   bool        isConnected();

 // operations
   bool        create(int port=0);
   int         connect(const std::string& serverName, int port);
   void        disconnect();
   void        update();

 // sending
   void        send(int clientid, const NetStream& stream, NetPackage::Reliability reliability = NetPackage::eReliableOrdered);
   void        send(int clientid, const NetObject& object, NetPackage::Reliability reliability = NetPackage::eReliableOrdered);

private:
 // query   
   SocketError getErrorNumber() const;

 // operations
   int         addNewClient(const NetAddress& address);
   int         findOrCreate(const NetAddress& address);

   void        process(int clientid);
   void        processPackage(int clientid, NetPackage& package);

   bool        select (bool read, bool write);
   void        receive();

   void        send(NetAddress& client, const NetStream& stream, NetPackage::Reliability reliability);
   void        sendAck(NetAddress& client);
   void        sendAlive(NetAddress& client, float tick);

   void           doSend(NetAddress& client, const NetPackage& package);
   PackageHandle  doReceive(NetAddress& address);

   void        handleError(NetAddress& client, SocketError error);

 // members
   Process&          mProcess;
   NetClients        mClients;
   PackageAllocator  mAllocator;
   int               mSock;
   int               mFlags;
};

#ifdef JENGINE_INLINE
#  include "netconnection.inl"
#endif

#endif
