package com.ts.presentation.view.decorationAssets.converters
{
	import com.ts.presentation.view.decorationAssets.interpolators.AbstractDataInterpolator;

	public class AbstractDataConverter
	{
		protected var _interpolator:AbstractDataInterpolator;
		
		public function AbstractDataConverter( interpolator:AbstractDataInterpolator )
		{
			_interpolator 	= interpolator;
		}
		
		public function convertValue( value:Number ):*
		{
			return _interpolator.interpolateValue( value );
		}
	}
}