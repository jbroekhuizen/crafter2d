
package engine.ui;

use engine.shapes.*;

class EngineGraphics extends Graphics
{
	private Font  mFont;
	private Color mColor;
	
	public native EngineGraphics()
	{
		super();
	}

	public native void drawText(int x, int y, string text);
	public native void translate(int x, int y);

	private native void native_drawRect(int x, int y, int width, int height);
	private native void native_drawRoundedRect(int x, int y, int width, int height);
	private native void native_fillRect(int x, int y, int width, int height);
	private native void doSetColor(real r, real g, real b, real a);
	private native void nativeSetFont(Font font);
	
	public void fillRect(Rectangle rect)
	{
		native_fillRect(rect.x, rect.y, rect.width, rect.height);
	}
	
	public void drawRect(Rectangle rect)
	{
		native_drawRect(rect.x, rect.y, rect.width, rect.height);
	}
	
	public void drawRoundedRect(Rectangle rect)
	{
		native_drawRoundedRect(rect.x, rect.y, rect.width, rect.height);
	}
	
	public Color getColor()
	{
		return mColor;
	}
	
	public void setColor(Color color)
	{
		mColor = color;
		doSetColor(mColor.getRed(), mColor.getGreen(), mColor.getBlue(), mColor.getAlpha());
	}
	
	public void setFont(Font font)
	{
		mFont = font;
		nativeSetFont(mFont);
	}
}
