package com.ts.presentation.view.decorationAssets.converters
{
	import com.ts.presentation.view.decorationAssets.interpolators.AbstractColorInterpolator;

	public class DataToColorConverter extends AbstractDataConverter
	{
		private var _defaultColor:uint;
		public function get defaultColor():uint{ return _defaultColor; }
		
		public function DataToColorConverter( interpolator:AbstractColorInterpolator )
		{
			super( interpolator );
			_defaultColor = interpolator.defaultColor;
		}
		
		public function convertValueToColor( value:Number ):uint
		{
			return uint( _interpolator.interpolateValue( value ) );
		}
	}
}