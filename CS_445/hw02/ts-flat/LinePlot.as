package com.ts.presentation.view.decorationAssets.plotters
{
	import com.ts.presentation.view.decorationAssets.converters.DataToColorConverter;
	import com.ts.utilities.drawing.PathUtilities;
	
	import flash.geom.Point;

	public class LinePlot
	{
		public static function buildDynamicLinearTrend( xData:Array, yData:Array, maxValue:Number, colorData:Array, dataConverter:DataToColorConverter, colorNaN:Boolean = false ):Array
		{
			
			var segmentMap:Object = new Object();
			var trendParts:Array = new Array();
			
			var color:uint;
			
			//-- 1a. build color lookup table, make a chunk of path string for each color
			var n:int = xData.length;
			
			if( colorData )
			{
				for( var i:int = 0; i < n; i++ )
				{
					if( !isNaN( xData[ i ] ) && !isNaN( yData[ i ] ) )
					{
						if( colorNaN )
						{
							color = dataConverter.convertValueToColor( colorData[ xData[ i ] ] );
							
							if( !segmentMap[ color ] )
							{
								segmentMap[ color ] = new Array();
							}
							segmentMap[ color ].push( i );
						}
						else
						{
							if( !isNaN( colorData[ xData[ i ] ] ) )
							{
								color = dataConverter.convertValueToColor( colorData[ xData[ i ] ] );
								
								if( !segmentMap[ color ] )
								{
									segmentMap[ color ] = new Array();
								}
								segmentMap[ color ].push( i );
							}
						}
					}
				}
			}
			else
			{
				color = dataConverter.defaultColor;
				segmentMap[ color ] = new Array();
				
				for( i = 0; i < n; i++ )
				{
					if( !isNaN( xData [ i ] ) && !isNaN( yData[ i ] ) )
					{
						segmentMap[ color ].push( i );
					}
				}
			}
			
			//-- 2. group valueData by color, & build path data for each color group.
			var segmentGroup:Array;
			var previousIndex:int = 0;
			for( var key:String in segmentMap )
			{
				color = uint( key );
				segmentGroup = segmentMap[ key ];
				previousIndex = segmentGroup[ 0 ] - 1;
				var pathSegment:Array = new Array();
				var segments:Array = new Array();
				
				//-- collect each path segment in the color group ( 2D array )
				n = segmentGroup.length;
				for( i = 0; i < n; i++ )
				{
					//-- accounting for the fact that a line has no real width, so each group of points will need a 1 px wide extender
					if( segmentGroup[ i ] != previousIndex + 1 ) //-- checking to see if the x value incremented or jumped ( jumped = new segment )
					{
						pathSegment.push( new Point( xData[ segmentGroup[ i - 1 ] ] + 1, maxValue - yData[ segmentGroup[ i - 1 ] ] ) );
						segments.push( pathSegment );
						
						pathSegment = new Array();
					}
					pathSegment.push( new Point( xData[ segmentGroup[ i ] ], maxValue - yData[ segmentGroup[ i ] ] ) );
					previousIndex = segmentGroup[ i ]; 
				}
				
				//-- push last point onto the segment
				pathSegment.push( new Point( xData[ segmentGroup[ i - 1 ] ] + 1, maxValue - yData[ segmentGroup[ i - 1 ] ] ) );
				
				//-- add the segment to the segments
				segments.push( pathSegment );
				
				trendParts.push( new ColoredTrendLineVO( color, PathUtilities.pointSegmentsToPathData( segments, false ) ) );
			}		
			return trendParts;
		}
		
		public static function buildConstantLinearTrend( value:Number, maxValue:Number, length:int, colorData:Array, dataConverter:DataToColorConverter, colorNaN:Boolean = false ):Array
		{
			var segmentMap:Object = new Object();
			var trendParts:Array = new Array();
			
			var color:uint;
			
			//-- 1. build color lookup table, make a chunk of path string for each color
			var n:int = length;
			
			if( colorData )
			{
				for( var i:int = 0; i < n; i++ )
				{
					if( colorNaN )
					{
						color = dataConverter.convertValueToColor( colorData[ i ] );
						
						if( !segmentMap[ color ] )
						{
							segmentMap[ color ] = new Array();
						}
						segmentMap[ color ].push( i );
					}
					else
					{
						if( !isNaN( colorData[ i ] ) )
						{
							color = dataConverter.convertValueToColor( colorData[ i ] );
							
							if( !segmentMap[ color ] )
							{
								segmentMap[ color ] = new Array();
							}
							segmentMap[ color ].push( i );
						}
					}
				}
			}
			else
			{
				color = dataConverter.defaultColor
				segmentMap[ color ] = new Array();
				
				for( i = 0; i < n; i++ )
				{
					segmentMap[ color ].push( i );
				}
			}
			
			//-- 2. group valueData by color, & build path data for each color group.
			var segmentGroup:Array;
			var previousIndex:int = 0;
			for( var key:String in segmentMap )
			{
				color = uint( key );
				segmentGroup = segmentMap[ key ];
				previousIndex = segmentGroup[ 0 ] - 1;
				var pathSegment:Array = new Array();
				var segments:Array = new Array();
				
				//-- collect each path segment in the color group ( 2D array )
				n = segmentGroup.length;
				for( i = 0; i < n; i++ )
				{
					//-- accounting for the fact that a line has no real width, so each group of points will need a 1 px wide extender
					if( segmentGroup[ i ] != previousIndex + 1 ) //-- checking to see if the x value incremented or jumped ( jumped = new segment )
					{
						pathSegment.push( new Point( segmentGroup[ i - 1 ] + 1, maxValue - value ) );
						segments.push( pathSegment );
						
						pathSegment = new Array();
					}
					pathSegment.push( new Point( segmentGroup[ i ], maxValue - value ) );
					previousIndex = segmentGroup[ i ]; 
				}
				
				//-- push last point onto the segment
				pathSegment.push( new Point( segmentGroup[ i - 1 ] + 1, maxValue - value ) );
				
				//-- add the segment to the segments
				segments.push( pathSegment );
				
				trendParts.push( new ColoredTrendLineVO( color, PathUtilities.pointSegmentsToPathData( segments, false ) ) );
			}		
			return trendParts;
		}
	}
}