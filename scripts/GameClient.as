
use engine.ui.*;
use engine.messages.*;
use engine.collections.*;

class GameClient extends Client
{
	public GameClient()
	{
		super();
	}
	
	public boolean create()
	{
		if ( super.create() )
		{			
			MessageBox.showInformation("This is a messagebox!");
			
			return true;
		}
		
		return false;
	}
	
	public void onConnected()
	{
		setActionMap(new ActionMap());
		
		KeyMap map = new KeyMap();
		map.bind(276, 1); 	// left
		map.bind(275, 2); 	// right
		map.bind(32, 3); 	// space -> jump
		setKeyMap(map);
	}
	
	protected void registerMessages(MessageMap messagemap)
	{
		//super.registerMessages(messagemap);
		
		messagemap.register(ControllerMessage.ID, ControllerMessage.class);
	}
	
	protected void onMessageReceived(Message message)
	{
		Map<int, Message> messagemap;
		
		if ( message instanceof ControllerMessage ) // player controller
		{
			ControllerMessage msg = (ControllerMessage)message;
			World world = getWorld();
			
			int controllerid = msg.getControllerId();
			Actor controller = (Actor) world.findEntity(controllerid);
			getPlayer().setController(controller);
			
			world.setFollowMode(0);
			world.setObjectLayer(0);
			world.setFollowActor(controller);
			world.setFollowBorders(150, 650, 100, 500);
		}
	}
	
	public void onWorldChanged()
	{
	}
}
