package com.ts.presentation.view.decorationAssets.interpolators
{
	public class StateColorInterpolator extends AbstractColorInterpolator
	{
		private var _colorLookup:Object;

		public function StateColorInterpolator( valueMap:Array, colorMap:Array, defaultColor:uint = 0x000000 )
		{
			super( defaultColor );
			
			_colorLookup = new Object();
			for( var i:int = 0; i < valueMap.length; i++ )
			{
				if( colorMap.length > i )
				{
					_colorLookup[ valueMap[ i ] ] = colorMap[ i ];
				}
				else //-- for the case of the valueMap being greater than the color map.
				{
					_colorLookup[ valueMap[ i ] ] = defaultColor;
				}
			}
		}
		
		override public function interpolateValueToColor( value:Number ):uint
		{
			if( _colorLookup[ value ] ) return _colorLookup[ value ];
			return defaultColor;
		}
	}
}