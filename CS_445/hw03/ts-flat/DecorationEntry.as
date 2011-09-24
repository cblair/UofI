package com.ts.aspect.development
{
	import com.ts.utilities.colorRanges.ColorInterpolation;

	public class DecorationEntry
	{
		public var floorValue:Number;
		public var color:uint;
		public var alpha:Number;
		public var included:Boolean;
		public var numStops:int;
		
		private var _colors:Array;
		private var _alphas:Array;
		private var _ceilValue:Number;
		private var _normalCeil:Number;
		private var _stopIncrement:Number;
		private var _ceilColor:uint;
		private var _ceilAlpha:Number;
		
		private var _lookupFunction:Function;
		private var _testFunction:Function;
		
		public function set isRanged( value:Boolean ):void
		{
			if( !isNaN( floorValue ) )
			{
				if( value )
				{
					if( !isNaN( _ceilValue ) )
					{
						_testFunction = included ? testInclusiveBoundedRange : testExclusiveBoundedRange;
					}
					else
					{
						_testFunction = included ? testInclusiveUnboundedRange : testExclusiveUnboundedRange;
						_lookupFunction = getConstantColorAlpha;
					}
				}
				else
				{
					_testFunction = testExactValue;
					_lookupFunction = getConstantColorAlpha;
				}
			}
			else
			{
				_testFunction = testNaN;
				_lookupFunction = getConstantColorAlpha;
			}
		}
		
		public function setCeilData( value:Number, ceilColor:uint, ceilAlpha:uint ):void
		{
			if( !isNaN( floorValue ) )
			{
				if( numStops > 0 )
				{
					_ceilValue = value;
					_normalCeil = value - floorValue;
					_stopIncrement = _normalCeil / numStops;
					_ceilColor = ceilColor;
					_ceilAlpha = ceilAlpha;
					
					_lookupFunction = getRangedColorAlpha;
					
					_colors = [ color ];
					_alphas = [ alpha ];
					
					for( var i:int = 1; i < numStops; i++ )
					{
						_colors.push( ColorInterpolation.interpolate2ColorRBG( 0, i * _stopIncrement, _normalCeil, color, _ceilColor ) );
						_alphas.push( ColorInterpolation.interpolate2Alphas( 0, i * _stopIncrement, _normalCeil, alpha, _ceilAlpha ) );
					}
				}
				
				_testFunction = included ? testInclusiveBoundedRange : testExclusiveBoundedRange;
			}
		}
		
		public function DecorationEntry( FloorValue:Number, Color:uint, Alpha:Number, Included:Boolean = true, NumStops:int = 0 )
		{
			floorValue 	= FloorValue;
			color 		= Color;
			alpha 		= Alpha;
			included 	= Included;
			numStops 	= NumStops;
			
			_lookupFunction = getConstantColorAlpha;
			_testFunction = isNaN( FloorValue ) ? testNaN : testExactValue;
		}
		
		public function testValue( value:Number ):Boolean
		{
			return _testFunction( value );
		}
		
		public function getColorAlpha( value:Number ):Array
		{
			return _lookupFunction( value );
		}
		
		private function testNaN( value:Number ):Boolean
		{
			return isNaN( value );
		}
		
		private function testExactValue( value:Number ):Boolean
		{
			if( !isNaN( value ) )
			{
				return value == floorValue
			}
			return false;
		}
		
		private function testInclusiveUnboundedRange( value:Number ):Boolean
		{
			if( !isNaN( value ) )
			{
				return value >= floorValue
			}
			return false;
		}
		
		private function testExclusiveUnboundedRange( value:Number ):Boolean
		{
			if( !isNaN( value ) )
			{
				return value > floorValue
			}
			return false;
		}
		
		private function testInclusiveBoundedRange( value:Number ):Boolean
		{
			if( !isNaN( value ) )
			{
				return value >= floorValue && value < _ceilValue
			}
			return false;
		}
		
		private function testExclusiveBoundedRange( value:Number ):Boolean
		{
			if( !isNaN( value ) )
			{
				return value > floorValue && value < _ceilValue
			}
			return false;
		}
		
		private function getConstantColorAlpha( value:Number ):Array
		{
			if( _testFunction( value ) )
			{
				return [ color, alpha ];
			}
			return null;
		}
		
		private function getRangedColorAlpha( value:Number ):Array
		{
			if( _testFunction( value ) )
			{
				value -= floorValue;
				value = Math.floor( value / _stopIncrement );
				return [ _colors[ value ], _alphas[ value ] ];
			}
			return null;
		}
	}
}