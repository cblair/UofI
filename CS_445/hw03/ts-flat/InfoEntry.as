package com.ts.aspect.development
{
	public class InfoEntry
	{
		public var floorValue:Number;
		public var tooltip:String;
		public var weblink:String;
		public var included:Boolean;
		
		private var _ceilValue:Number;
		
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
					}
				}
				else
				{
					_testFunction = testExactValue;
				}
			}
			else
			{
				_testFunction = testNaN;
			}
		}
		
		public function setCeilData( value:Number ):void
		{
			if( !isNaN( floorValue ) )
			{
				_ceilValue = value;
				
				_testFunction = included ? testInclusiveBoundedRange : testExclusiveBoundedRange;
			}
		}
		
		public function InfoEntry( FloorValue:Number, Tooltip:String, Weblink:String, Included:Boolean = true )
		{
			floorValue 	= FloorValue;
			tooltip		= Tooltip;
			weblink		= Weblink;
			included 	= Included;
			
			_lookupFunction = getConstantTooltipWeblink;
			_testFunction = isNaN( FloorValue ) ? testNaN : testExactValue;
		}
		
		public function testValue( value:Number ):Boolean
		{
			return _testFunction( value );
		}
		
		public function getTooltipWeblink( value:Number ):Array
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
		
		private function getConstantTooltipWeblink( value:Number ):Array
		{
			if( _testFunction( value ) )
			{
				return [ tooltip, weblink ];
			}
			return null;
		}
	}
}