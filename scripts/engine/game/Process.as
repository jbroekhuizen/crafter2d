
package engine.game;

use engine.ui.*;
use engine.core.*;
use engine.collections.*;
use engine.messages.*;
use engine.net.*;
use engine.streams.*;

// Class : Process
// Abstract base class for the client and server processes

abstract class Process
{
	protected BufferedStream mBufferedStream = new BufferedStream();
	protected NetStream mStream = new NetStream(mBufferedStream);
	
	private MessageMap mMessageMap     = new MessageMap();
	private ArrayList<Player> mPlayers = new ArrayList<Player>();
	
	private World mWorld;
	
	public static int CONNECT_ALLOW = 0;
	public static int CONNECT_DENY_STARTED = 1;
	
	/// called by the native process when it is created
	private boolean onCreated()
	{		
		boolean success = create();
		if ( success )
		{	
			// Register the messages
			registerMessages(mMessageMap);
		}
		return success;
	}
	
	public abstract boolean create();
	
	public void onScriptEvent(NetStream stream)
	{
		int msgid = stream.readInt();
		Message message = mMessageMap.getMessage(msgid);
		message.read(stream);
			
		onMessageReceived(message);
	}
	
	// - Get/set
	
	public void addPlayer(Player player)
	{
		System.console.println("Added player to ");
		mPlayers.add(player);
	}
	
	public Player getPlayers()
	{
		return mPlayers.get(0);
	}
	
	public World getWorld()
	{
		return mWorld;
	}
	
	/// does not call the native function
	protected void setInternalWorld(World world)
	{
		mWorld = world;
	}
	
	public void setWorld(World world)
	{
		mWorld = world;
		native_setWorld(world);
	}
	
	// - Overloadables
	
	protected void registerMessages(MessageMap messagemap)
	{
	}
	
	protected void onMessageReceived(Message message)
	{
	}
	
	// - Memory leak detection
	
	public native void swapLeakDetection();
	
	// - Natives
	
	public native void destroy();
	public native ScriptManager getScriptManager();
	public native void setScriptManager(ScriptManager scriptmanager);
	public native Font getFont(string name, int size);
	public native Texture getTexture(string name);
	public native ContentManager getContentManager();
	public native void native_setWorld(World world);
}