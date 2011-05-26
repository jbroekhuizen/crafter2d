
use box2d.box2dsimulator;

use bound;
use scenegraph;
use creature;

class World
{
	public native World();
	
	public native string getName();
	public native SceneGraph getSceneGraph();
	public native void add(Creature creature);
	public native void setObjectLayer(int layer);
	public native void setFollowMode(int mode);
	public native void setFollowObject(Creature creature);
	public native void setFollowBorders(int left, int right, int top, int bottom);
	public native Box2DSimulator getSimulator();
	
	public void onObjectWorldCollision(Creature creature, Bound bound, int side, boolean begin)
	{
		if ( side == 1 ) // bottom
		{
			creature.setOnGround(begin);
		}
	}
}
