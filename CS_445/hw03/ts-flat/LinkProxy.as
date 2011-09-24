package com.ts.presentation.model.map.phase
{
	import com.ts.utilities.drawing.PathUtilities;
	import com.ts.utilities.drawing.TrigCalculations;
	
	import flash.geom.Point;
	import flash.sampler.NewObjectSample;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class LinkProxy extends Proxy
	{
		public static const NAME:String = "linkProxy";
		
		public var linkNum:int;
		public var numLanes:int;
		public var points:Array;
		public var linkLength:Number;
		public var forkLength:Number;
		public var fromAt:Number;
		public var toAt:Number;
		public var prevLinkNum:int;
		public var nextLinkNum:int;
		public var fromAngle:Number;
		public var toAngle:Number;
		public var avgAngle:Number;
		public var angleVariance:Number;
		public var isConnector:Boolean;
		public var hasSignal:Boolean;
		public var signalNum:int = -1;
		public var isTerminalLink:Boolean = false;
		
		private var _prevLink:LinkProxy;
		private var _nextLink:LinkProxy;
		
		private var _distanceMap:Array;
		private var _xDistance:Number = 0;
		private var _yDistance:Number = 0;
		
		public function get startPoint():Point
		{
			return points[0] as Point;
		}
		
		public function get endPoint():Point
		{
			return points[points.length - 1] as Point;
		}
		
		public function LinkProxy( LinkNum:int, NumLanes:int, Points:Array, LinkLength:Number, PrevLinkNum:int, NextLinkNum:int,
								   FromAngle:Number, ToAngle:Number, AvgAngle:Number, AngleVariance:Number, IsConnector:Boolean,
								   HasSignal:Boolean, SignalNum:int, FromAt:Number = NaN, ToAt:Number = NaN )
		{
			linkNum 		= LinkNum;
			numLanes 		= NumLanes;
			points 			= Points;
			linkLength 		= LinkLength;
			prevLinkNum 	= PrevLinkNum;
			nextLinkNum		= NextLinkNum;
			fromAngle		= FromAngle;
			toAngle			= ToAngle;
			avgAngle		= AvgAngle;
			angleVariance	= AngleVariance;
			isConnector		= IsConnector;
			hasSignal		= HasSignal;
			signalNum		= SignalNum;
			
			forkLength = linkLength;
			fromAt = isNaN( FromAt ) ? 0 : FromAt;
			toAt = isNaN( ToAt ) ? 0 : ToAt;
			
			_distanceMap = calculateDistanceMap();
			
			super( NAME + String( LinkNum ) );
		}
		
		private function calculateDistanceMap():Array
		{
			var dMap:Array = new Array();
			var runningLength:Number = 0;
			
			dMap.push( 0 );
			
			for( var i:int = 1; i < points.length; i++ )
			{
				dMap.push( Point.distance( points[ i - 1 ], points[ i ] ) + runningLength );
				runningLength += dMap[ i ] - runningLength;
				
				_xDistance += points[ i ].x;
				_yDistance += points[ i ].y;
			}
			
			trace ( dMap );
			return dMap;
		}
		
		public function get pathData():String
		{
			return PathUtilities.pointsToPathData( points, false, false );
		}
		
		public function get nextLink():LinkProxy{ return _nextLink; }
		public function set nextLink( value:LinkProxy ):void
		{
			_nextLink = value;
			nextLinkNum = value.linkNum;
		}
		
		public function get prevLink():LinkProxy{ return _prevLink; }
		public function set prevLink( value:LinkProxy ):void
		{
			_prevLink = value;
			prevLinkNum = value.linkNum;
		}
		
		public function get startLink():LinkProxy
		{
			if( _prevLink == null ) return this;
			else
			{
				return prevLink.startLink;
			}
		}
		
		public function get endLink():LinkProxy
		{
			if( _nextLink == null ) return this;
			else
			{
				return _nextLink.endLink;
			}
		}
		
		public function getLanePoints( pts:Array ):Array
		{
			pts.concat( points );
			if( nextLink )
			{
				pts = nextLink.getLanePoints( pts );
			}
			
			return pts;
		}
		
		//-- recursive
		public function getLengthAlongLinks( length:Number = 0 ):Number
		{
			/*length += linkLength;
			if( nextLink )
			{
				length = nextLink.getLengthAlongLinks( length );
			}
			return length
			*/
			length += linkLength;
			if( isConnector )
			{
				if( prevLink )
				{
					length -= ( prevLink.linkLength - fromAt );
				}
				length -= toAt;
			}
			
			if( nextLink )
			{
				return nextLink.getLengthAlongLinks( length );
			}
			return length;
		}
		
		public function getDistanceAlongPath( distance:Number, linkN:int, pos:Number ):Number
		{
			if( linkN == linkNum )
			{
				if( isConnector )
				{
					if( prevLink )
					{
						distance -= ( prevLink.linkLength - fromAt );
					}
				}
				return distance + pos;
				
				/*distance += _distanceMap[ boundingPointIndexes[ 0 ] ];
				return distance + Point.distance( points[ boundingPointIndexes[ 0 ] ], pos );*/
				
			}
			else
			{
				distance += linkLength;
				if( isConnector )
				{
					if( prevLink )
					{
						distance -= ( prevLink.linkLength - fromAt );
					}
					distance -= toAt;
				}
				
				if( nextLink )
				{
					return nextLink.getDistanceAlongPath( distance, linkN, pos );
				}
				trace( "this should never happen" );
				return distance;
			}
		}
		
		//-- *** unreliable, need to fix
		public function getBoundingPointIndexesAtPoint( point:Point ):Array
		{
			var failedX:Boolean = true;
			var failedY:Boolean = true;
			var startX:int = -1;
			var startY:int = -1;
			
			for( var i:int = 0; i < points.length - 1; i++ )
			{
				if( startX == -1 )
				{
					if( TrigCalculations.isBetween( points[ i ].x, points[ i + 1 ].x, point.x ) )
					{
						startX == i;
					}
				}
				
				if( startY == -1 )
				{
					if( TrigCalculations.isBetween( points[ i ].y, points[ i + 1 ].y, point.y ) )
					{
						startY == i;
					}
				}
				
			}
			
			if( startX != -1 && startY != -1 )
			{
				trace( "x,y");
				return ( _xDistance >= _yDistance ? [ startX, startX + 1 ] : [ startY, startY + 1 ] );
			}
			else if( startX != -1 )
			{
				trace( "x");
				return [ startX, startX + 1 ];
			}
			else if( startY != -1 )
			{
				trace( "y");
				return [ startY, startY + 1 ];
			}
			//trace( "default");
			return [ 0, 1 ];
		}
		
		public function getPointAtDistance( value:Number ):Point
		{
			value = Math.max( 0, Math.min( linkLength, value ) );
			
			//-- get the 2 points that surround the distance
			var boundingPoints:Array = getBoundingPointsAtDistance( value );
			
			//-- interpolating at endpoint, startpoint
			return Point.interpolate( boundingPoints[ 1 ], boundingPoints[ 0 ], ( value / linkLength ) ); 
		}
		
		private function getBoundingPointsAtDistance( value:Number ):Array
		{
			if( value == 0 )
			{
				return [ points[ 0 ], points.length > 1 ? points[ 1 ] : points[ 0 ] ]
			}
			
			var endPoint:int = 0;
			while( value > _distanceMap[ endPoint ] )
			{
				endPoint++;
			}
			
			return [ points[ endPoint - 1 ], points[ endPoint ] ];
		}

		public function toString( asList:Boolean = true ):String
		{
			return null;
		}
		
		public function getTangilinearAngle( distance:Number ):Number
		{
			//-- get the 2 points that surround the distance
			var boundingPoints:Array = getBoundingPointsAtDistance( distance );
			return TrigCalculations.getAngleInDegrees( boundingPoints[ 0 ], boundingPoints[ 1 ] ) + 90;
		}
	}
}