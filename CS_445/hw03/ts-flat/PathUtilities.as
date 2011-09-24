package com.ts.utilities.drawing
{
	import flash.geom.Point;

	public class PathUtilities
	{
		public static const KAPPA:Number = 0.5522847498;
		
		public static function pointsToPathData( points:Array, closePath:Boolean = true, relative:Boolean = false, workingPath:String = "", compact:Boolean = true ):String
		{
			var n:int = points.length;
			
			if( n > 1 )
			{
				var M:String = relative ? "m" : "M";
				var L:String = relative ? "l" : "L";
				var Z:String = relative ? "z" : "Z";
				var SPACE:String = compact ? "" : " ";
				
				workingPath += M + SPACE + String( points[ 0 ].x ) + " " + String( points[ 0 ].y ) + SPACE;
				
				var lastPoint:Point = points[ 1 ];
				var firstLinearPoint:Point = points[ 0 ];
				
				var pointDeltaX:Number = points[ 0 ].x - lastPoint.x;
				var pointDeltaY:Number = points[ 0 ].y - lastPoint.y;
				
				var endRun:Boolean = false;
				
				for( var i:int = 1; i < n; i++ )
				{
					if( pointDeltaX == points[ i ].x - lastPoint.x )
					{
						if( pointDeltaY == points[ i ].y - lastPoint.y )
						{
							lastPoint = points[ i ];
						}
						else
						{
							endRun = true;
						}
					}
					else
					{
						endRun = true;
					}	
					
					
					if( endRun )
					{
						workingPath += L + SPACE + String( lastPoint.x ) + " " + String( lastPoint.y ) + SPACE;
						
						pointDeltaX = points[ i ].x - lastPoint.x;
						pointDeltaY = points[ i ].y - lastPoint.y;
						
						firstLinearPoint = lastPoint;
						lastPoint = points[ i ];
						
						endRun = false;
					}
				}
				
				workingPath += L + SPACE + String( lastPoint.x ) + " " + String( lastPoint.y ) + SPACE;
				workingPath += closePath ? Z + SPACE : SPACE;
			}
			
			return workingPath;
		}
		
		public static function pointSegmentsToPathData( pointSegments:Array, closePath:Boolean = true, relative:Boolean = false, workingPath:String = "", compact:Boolean = true ):String
		{
			for each( var points:Array in pointSegments )
			{
				workingPath = pointsToPathData( points, closePath, relative, workingPath, compact );
			}
			
			return workingPath;
		}
		
		public static function sideStitchPointPairs( pointPairs:Array, closePath:Boolean = true, relative:Boolean = false, workingPath:String = "", compact:Boolean = true ):String
		{
			var n:int = pointPairs.length;
			
			if( n > 1 )
			{
				var M:String = relative ? "m" : "M";
				var L:String = relative ? "l" : "L";
				var Z:String = relative ? "z" : "Z";
				var SPACE:String = compact ? "" : " ";
				
				var upperPath:String = "";
				var lowerPath:String = "";
				var workingPointPair:Array = pointPairs[ 0 ];
				
				//-- build upper path forward, lower path backwards.
				upperPath += M + SPACE + String( workingPointPair[ 0 ].x ) + " " + String( workingPointPair[ 0 ].y ) + SPACE;
				lowerPath += L + SPACE + String( workingPointPair[ 1 ].x ) + " " + String( workingPointPair[ 1 ].y ) + SPACE + ( closePath ? Z + SPACE : SPACE );
				
				var lastUpperPoint:Point = pointPairs[ 1 ][ 0 ];
				var lastLowerPoint:Point = pointPairs[ 1 ][ 1 ];
				
				var firstLinearUpperPoint:Point = pointPairs[ 0 ][ 0 ];
				var firstLinearLowerPoint:Point = pointPairs[ 0 ][ 1 ];
				
				var upperPointDeltaX:Number = workingPointPair[ 0 ].x - lastUpperPoint.x;
				var upperPointDeltaY:Number = workingPointPair[ 0 ].y - lastUpperPoint.y;
				var lowerPointDeltaX:Number = workingPointPair[ 1 ].x - lastLowerPoint.x;
				var lowerPointDeltaY:Number = workingPointPair[ 1 ].y - lastLowerPoint.y;
				
				var endUpperRun:Boolean = false;
				var endLowerRun:Boolean = false;
				
				//-- build upper path forward, lower path backwards.
				for( var i:int = 1; i < n; i++ )
				{
					workingPointPair = pointPairs[ i ];
					
					if( upperPointDeltaX == workingPointPair[ 0 ].x - lastUpperPoint.x )
					{
						if( upperPointDeltaY == workingPointPair[ 0 ].y - lastUpperPoint.y )
						{
							lastUpperPoint = workingPointPair[ 0 ];
						}
						else
						{
							endUpperRun = true;
						}
					}
					else
					{
						endUpperRun = true;
					}
					
					if( endUpperRun )
					{
						upperPath = upperPath + L + SPACE + String( lastUpperPoint.x ) + " " + String( lastUpperPoint.y ) + SPACE;
						
						upperPointDeltaX = workingPointPair[ 0 ].x - lastUpperPoint.x;
						upperPointDeltaY = workingPointPair[ 0 ].y - lastUpperPoint.y;
						
						firstLinearUpperPoint = lastUpperPoint;
						lastUpperPoint = workingPointPair[ 0 ];
						
						endUpperRun = false;
					}
					
					if( lowerPointDeltaX == workingPointPair[ 1 ].x - lastLowerPoint.x )
					{
						if( lowerPointDeltaY == workingPointPair[ 1 ].y - lastLowerPoint.y )
						{
							lastLowerPoint = workingPointPair[ 1 ];
						}
						else
						{
							endLowerRun = true;
						}
					}
					else
					{
						endLowerRun = true;
					}
					
					if( endLowerRun )
					{
						lowerPath = L + SPACE + String( lastLowerPoint.x ) + " " + String( lastLowerPoint.y ) + SPACE + lowerPath;
						
						lowerPointDeltaX = workingPointPair[ 1 ].x - lastLowerPoint.x;
						lowerPointDeltaY = workingPointPair[ 1 ].y - lastLowerPoint.y;
						
						firstLinearLowerPoint = lastLowerPoint;
						lastLowerPoint = workingPointPair[ 1 ];
						
						endLowerRun = false;
					}
				}
				
				upperPath = upperPath + L + SPACE + String( lastUpperPoint.x ) + " " + String( lastUpperPoint.y ) + SPACE;
				lowerPath = L + SPACE + String( lastLowerPoint.x ) + " " + String( lastLowerPoint.y ) + SPACE + lowerPath;
				workingPath += upperPath + lowerPath;
			}
			return workingPath;
		}
		
		public static function sideStitchPointPairGroups( pointPairGroups:Array, closePath:Boolean = true, relative:Boolean = false, workingPath:String = "", compact:Boolean = true ):String
		{
			for each( var pointPairs:Array in pointPairGroups )
			{
				workingPath = sideStitchPointPairs( pointPairs, closePath, relative, workingPath, compact );
			}
			
			return workingPath;
		}
		
		public static function XYpointsToX( points:Array ):Array
		{
			var xPoints:Array = new Array();
			
			for each( var point:Point in points )
			{
				xPoints.push( point.x );
			}
			
			return xPoints;
		}
		
		public static function XYpointsToY( points:Array ):Array
		{
			var yPoints:Array = new Array();
			
			for each( var point:Point in points )
			{
				yPoints.push( point.y );
			}
			
			return yPoints;
		}
		
		public static function ellipseFromBounds( tlPoint:Point, brPoint:Point ):String
		{
			var dX:Number = brPoint.x - tlPoint.x;
			var dY:Number = brPoint.y - tlPoint.y;
			var rX:Number = dX / 2;
			var rY:Number = dY / 2;
			
			//-- top point
			var x0:Number = rX;
			var y0:Number = 0;
			
			//-- top-right curve
			var x0c1:Number = x0 + rX * KAPPA;
			var y0c1:Number	= y0;
			var x1:Number = dX;
			var y1:Number = rY;
			var x1c0:Number	= x1;
			var y1c0:Number	= y1 - rY * KAPPA;
			
			//-- bottom-right curve
			var x1c1:Number = x1
			var y1c1:Number	= y1 + rY * KAPPA;
			var x2:Number = rX;
			var y2:Number = dY;
			var x2c0:Number	= x2 + rX * KAPPA;
			var y2c0:Number	= y2;
			
			//-- bottom-left curve
			var x2c1:Number = x2 - rX * KAPPA;
			var y2c1:Number	= y2;
			var x3:Number = 0;
			var y3:Number = rY;
			var x3c0:Number	= 0;
			var y3c0:Number	= y3 + rY * KAPPA;
			
			//-- top-left curve
			var x3c1:Number = 0;
			var y3c1:Number	= y3 - rY * KAPPA;
			var x4:Number = x0;
			var y4:Number = y0;
			var x4c0:Number	= x4 - rX * KAPPA;
			var y4c0:Number	= 0;
			
			return toMove( x0, y0 ) + 
				   toCubicArc( x0c1, y0c1, x1c0, y1c0, x1, y1 ) +
				   toCubicArc( x1c1, y1c1, x2c0, y2c0, x2, y2 ) +
				   toCubicArc( x2c1, y2c1, x3c0, y3c0, x3, y3 ) + 
				   toCubicArc( x3c1, y3c1, x4c0, y4c0, x4, y4 ) +
				   "Z";
		}
		
		public static function rectangleFromBounds( tlPoint:Point, brPoint:Point ):String
		{
			var width:Number 	= brPoint.x - tlPoint.x;
			var height:Number 	= brPoint.y - tlPoint.y;
			
			return toMove( 0, 0 ) +
				   toLine( width, 0 ) +
				   toLine( width, height ) +
				   toLine( 0, height ) +
				   toLine( 0, 0 ) +
				   "Z";
		}
		
		public static function arrowFromBounds( tlPoint:Point, brPoint:Point, tipRadius:Number = 15 ):String
		{
			var width:Number 	= brPoint.x - tlPoint.x;
			var height:Number 	= brPoint.y - tlPoint.y; 
			
			var startPoint:Point	= new Point( 0, 0 );
			var endPoint:Point		= new Point( width, height );
			
			var angle:Number 	= TrigCalculations.getAngleFromCoordinates( startPoint, endPoint );
			var tangent:Number 	= angle - 90;
			
			var length:Number		= Point.distance( startPoint, endPoint );
			var midPoint:Point  	= Point.interpolate( startPoint, endPoint, tipRadius  / length );
			var indentPoint:Point	= Point.interpolate( startPoint, endPoint, ( tipRadius - tipRadius * .15 ) / length );
			var leftPoint:Point 	= TrigCalculations.getPointFromAngle( tangent, midPoint, tipRadius / 2.25 );
			var rightPoint:Point	= TrigCalculations.getPointFromAngle( tangent, midPoint, -tipRadius / 2.25 );
			
			trace( length + ", " + angle );
			return toMove( 0, 0 ) +
				   toLine( indentPoint.x, indentPoint.y ) +
				   toLine( leftPoint.x, leftPoint.y ) +
			 	   toLine( endPoint.x, endPoint.y ) +
				   toLine( rightPoint.x, rightPoint.y ) +
				   toLine( indentPoint.x, indentPoint.y );
		}
		
		public static function lineFromBounds( tlPoint:Point, brPoint:Point ):String
		{
			var endPoint:Point = new Point( brPoint.x - tlPoint.x, brPoint.y - tlPoint.y );
			
			return toMove( 0, 0 ) +
				   toLine( endPoint.x, endPoint.y );	
		}
		
		private static function toMove( x:Number, y:Number ):String
		{
			return "M" + " " + String( x ) + " " + String( y ) + " ";
		}
		
		private static function toLine( x:Number, y:Number ):String
		{
			return "L" + " " + String( x ) + " " + String( y ) + " ";
		}
		
		private static function toCubicArc( cx0:Number, cy0:Number, cx1:Number, cy1:Number, x:Number, y:Number ):String
		{
			return "C" + " " + String( cx0 ) + " " + String( cy0 ) + " " + String( cx1 ) + " " + String( cy1 ) + " " + String( x ) + " " + String( y ) + " ";
		}
		
		public static function pointToString( point:Point ):String
		{
			return String( point.x ) + ":" + String( point.y );
		}
		
		public static function stringToPoint( pointString:String ):Point
		{
			var pointValues:Array = pointString.split( ":" );
			return new Point( Number( pointValues[ 0 ] ), Number( pointValues[ 1 ] ) );
		}
		
		public static function pointsToString( points:Array ):String
		{
			var string:String = "";
			for each( var point:Point in points )
			{
				string += pointToString( point ) + ",";
			}
			
			return string.substr( 0, string.length - 1 );
		}
		
		public static function stringToPoints( pointString:String ):Array
		{
			var pointPairs:Array 	= pointString.split( "," );
			var points:Array 		= new Array();
			for each( var pointPair:String in pointPairs )
			{
				points.push( stringToPoint( pointPair ) ); 
			}
			
			return points;
		}
		
	}
}