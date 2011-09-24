package com.ts.aspect.model
{
	public class NullDecoration extends PlotDecoration2VO
	{
		public function NullDecoration( MaxValue:Number, MinValue:Number )
		{
			super( MaxValue, MinValue, MinValue, MinValue, 0x000000, 0, "", "" );
		}
	}
}