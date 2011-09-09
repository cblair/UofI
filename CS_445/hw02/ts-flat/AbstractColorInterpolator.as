package com.ts.utilities.colorRanges
{
	import mx.graphics.IFill;
	import mx.graphics.IStroke;

	public class AbstractColorInterpolator
	{
		public function AbstractColorInterpolator()
		{
		}
		
		public function getColor( value:Number ):uint
		{
			//-- Override Me!!!
			return 0;
		}
		
		public function getHFill( value:Number ):IFill
		{
			//-- Override me!!!
			return null;
		}
		
		public function getVFill( value:Number ):IFill
		{
			//-- Override me!!!
			return null;
		}
		
		public function getHStroke( value:Number ):IStroke
		{
			//-- Override me!!!
			return null;
		}
		
		public function getVStroke( value:Number ):IStroke
		{
			//-- Override me!!!
			return null;
		}
		
		public function toString( asList:Boolean = true ):String
		{
			//-- Override Me!!
			return null;
		}
	}
}