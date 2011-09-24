package com.ts.utilities.drawing
{
	import flash.geom.Point;

	public class TrigCalculations
	{
		public function TrigCalculations()
		{
		}
		
		/**
		 * RETURNS THE ANGLE DESCRIBED BY 2 POINTS: EXPRESSED IN RADIANS 
		 **/
		public static function getAngleInRadians( point1:Point, point2:Point ):Number
		{
			var AnchorX:Number = point1.x - point2.x;
			var AnchorY:Number = point1.y - point2.y;
			
			return Math.PI + Math.atan2( AnchorY, AnchorX );
		}
		
		/**
		 * RETURNS THE ANGLE DESCRIBED BY 2 POINTS: EXPRESSED IN DEGREES, FROM 0 to 360 
		 **/
		public static function getAngleInDegrees( point1:Point, point2:Point ):Number
		{
			var AnchorX:Number = point1.x - point2.x;
			var AnchorY:Number = point1.y - point2.y;
			
			return ( ( Math.PI + Math.atan2( AnchorY, AnchorX ) ) ) * 180 / Math.PI;
		}
		
		public static function getAngleFromCoordinates( point1:Point, point2:Point ):Number
		{
			return 360 - ( getAngleInDegrees( point1, point2 ) + 180 ) % 360;
		}
		
		/**
		 * RETURNS THE ANGLE DESCRIBED BY 2 POINTS: EXPRESSED IN DEGREES, FROM 180 to -180 
		 **/
		public static function getRelativeAngleInDegrees( point1:Point, point2:Point ):Number
		{
			var AnchorX:Number = point1.x - point2.x;
			var AnchorY:Number = point1.y - point2.y;
			
			return 180 - Math.abs( Math.atan2( AnchorY, AnchorX ) * 180 / Math.PI );
		}
		
		/**
		 * RETURNS DISTANCE BETWEEN 2 POINTS
		 **/
		public static function getDistance( point1:Point, point2:Point ):Number
		{
			return Math.sqrt( Math.pow( ( point1.x - point2.x ), 2 ) + Math.pow( ( point1.y, point2.y ), 2 ) )
		}
		
		public static function getPointFromAngle( degAngle:Number, refPoint:Point, length:Number ):Point
		{
			var radAngle:Number = degAngle * Math.PI / 180;
			return new Point( refPoint.x + length * Math.cos( radAngle ), refPoint.y - length * Math.sin( radAngle ) );
		}
		
		public static function getStdDeviation( values:Array ):Number
		{
			var mean:Number = average( values );
			
			var runningArithmaticMean:Number = 0;
			for each( var value:Number in values )
			{
				runningArithmaticMean += Math.pow( ( value - mean ), 2 );
			}
			
			runningArithmaticMean /= values.length;
			
			return Math.sqrt( runningArithmaticMean );
		}
		
		public static function getVariance( values:Array ):Number
		{
			return Math.pow( getStdDeviation( values ), 2 );
		}
		
		public static function average( values:Array ):Number
		{
			var runningSum:Number = 0;
			for each( var value:Number in values )
			{
				runningSum += value;
			}
			
			return runningSum /= values.length;
		}
		
		public static function isMidPoint( startPoint:Point, endPoint:Point, testPoint:Point ):Boolean
		{
			return getDistance( startPoint, endPoint ) + getDistance( endPoint, testPoint ) == getDistance( startPoint, testPoint );
		}
		
		public static function isBetween( startVal:Number, endVal:Number, testVal:Number ):Boolean
		{
			var range:Number = endVal - startVal;
			var negativeRange:Boolean = range < 0;
			
			if( negativeRange )
			{
				return testVal <= startVal && testVal >= endVal;
			}
			else
			{
				return testVal >= startVal && testVal <= endVal;
			}
		}
	}
}