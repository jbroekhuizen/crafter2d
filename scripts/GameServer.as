
use server;
use engine.messages.*;

class GameServer extends Server
{
	private boolean mStarted = false;
	private Player mPlayer;
	private Hero   mHero;
	private Grudge mGrudge;
	
	public GameServer()
	{
		super();
	}
	
	public int onClientConnecting()
	{
		return 0;
	}
	
	public void onClientConnect(Player player)
	{
		start(player);
	}
	
	private void start(Player player)
	{
		addPlayer(player);
		mStarted = true;
		
		World world = getContentManager().load("../worlds/map1.jwl");
		if ( world == null )
		{
			// meh, something went terribly wrong..
		}
		
		setWorld(world);
		
		mHero = createPlayer(player);
		player.setController(mHero);
		
		createObjects();
	}
	
	private Hero createPlayer(Player player)
	{
		Vector2D position = new Vector2D();
		position.set(100, 30);
		
		ActionMap map = new ActionMap();
		map.setProcess(this);
		
		InputController controller = new InputController();
		controller.setActionMap(map);
		
		Hero actor = (Hero)Actor.construct(this, player.getName(), position, "../objects/char.xml");
		actor.setLocalController(controller);
		actor.setForceGenerator(new InputForceGenerator());
		
		// notify the client
		ControllerMessage message = new ControllerMessage(actor.getId());
		sendMessage(message, player.getClientId());
		
		return actor;
	}

	private void createObjects()
	{
		Vector2D left = new Vector2D();
		left.set(727, 422);
		
		Vector2D right = new Vector2D();
		right.set(943, 422);
		
		Bridge bridge = new Bridge();
		bridge.create(this, left, right);
		
		/*
		left.set(180.0, 80.0);
		for ( int index = 0; index < 4; index++ )
		{
			Actor.construct(this, "Box" + index, left, "../objects/box.xml");
			
			left.setY(left.getY() - 25.0);
		}
		*/
		
		Vector2D pos = new Vector2D();
		pos.set(190.0, 30);
		
		mGrudge = (Grudge) Actor.construct(this, "grudge1", pos, "../objects/grudge.xml");
		mGrudge.setLocalController(new AIController(this));
	}
}