package com.ts.utilities.layout
{
	/**
	 * Margin provides a quick way to independantly set the left, top, right, and bottom properties of an object.
	 * Values left, right, top, bottom can all be set independantly, or at one in an array, in that order.
	 * 
	 * Intended for the target component to BIND layout to these values, as this is just a state holder.
	 **/
	[Bindable]
	public class Margin
	{
		public var left:Number;
		public var top:Number;
		public var right:Number;
		public var bottom:Number;
		
		/**
		 * Set all the margin values at once. Takes and returns an array of numbers in the following format: [ L,T,R,B ]
		 * any missing values will be set to 0, and additional values will be ignored.
		 **/
		public function get marginArray():Array{ return [ left, top, right, bottom ]; }
		public function set marginArray( value:Array ):void
		{
			if( value )
			{
				left 	= value[ 0 ] && ! isNaN( Number( value[ 0 ] ) ) ? Number( value[ 0 ] ) : 0;
				top 	= value[ 1 ] && ! isNaN( Number( value[ 1 ] ) ) ? Number( value[ 1 ] ) : 0;
				right 	= value[ 2 ] && ! isNaN( Number( value[ 2 ] ) ) ? Number( value[ 2 ] ) : 0;
				bottom 	= value[ 3 ] && ! isNaN( Number( value[ 3 ] ) ) ? Number( value[ 3 ] ) : 0;
			}
		}
		
		/**
		 * Set the margin values in the constructor, or wait to set them later. Default value = 0;
		 **/
		public function Margin( l:Number = 0, t:Number = 0, r:Number = 0, b:Number = 0 )
		{
			left = l;
			top = t;
			right = r;
			bottom = b;
		}
	}
}