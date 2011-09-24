package com.ts.utilities.dragging
{
	public class DragVO
	{
		public var xOrigin:Number;
		public var yOrigin:Number;
		public var deltaX:Number;
		public var deltaY:Number;
		
		public function DragVO( XOrigin:Number, YOrigin:Number )
		{
			xOrigin = XOrigin;
			yOrigin = YOrigin;
			deltaX = 0;
			deltaY = 0;
		}
		
		public function set yPos( value:Number ):void
		{
			deltaY = value - yOrigin;
		}
		
		public function set xPos( value:Number ):void
		{
			deltaX = value - xOrigin;
		}
	}
}