package com.ts.presentation.model.grid
{
	public class GridLineVO
	{
		public var lineWeight:int;
		public var gap:int;
		public var color:uint;
		public var alpha:Number;
		public var height:Number;
		
		public function GridLineVO( LineWeight:int, Gap:int, Color:uint, Alpha:Number, Height:Number )
		{
			lineWeight 	= LineWeight;
			gap 		= Gap;
			color 		= Color;
			alpha 		= Alpha;
			height		= Height;
		}
	}
}