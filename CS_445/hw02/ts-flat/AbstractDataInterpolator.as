package com.ts.presentation.view.decorationAssets.interpolators
{
	public class AbstractDataInterpolator
	{
		protected var _roundValue:Boolean;
		
		public function AbstractDataInterpolator( roundValue:Boolean = true )
		{
			_roundValue = roundValue;
		}
		
		public function interpolateValue( value:Number ):*
		{
			return null;
		}
	}
}