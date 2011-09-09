package com.ts.aspect.development
{
	import com.ts.utilities.drawing.PathUtilities;
	
	import flash.geom.Point;

	public class InfoInterpolator
	{
		public var infoEntries:Array;
		public var isRange:Boolean;
		public var dataSet:Array;
		
		public function InfoInterpolator( DataSet:Array, InfoEntries:Array, IsRange:Boolean = false )
		{
			infoEntries 	= InfoEntries;
			isRange 		= IsRange;
			dataSet			= DataSet;
			
			//-- is a range info interpolator, meaning a wide range of values will be fit
			//-- to a dynamic range of info.
			if( IsRange )
			{
				//-- find the null info entry if it exists, and temporarily remove it.
				var nullEntry:InfoEntry;
				for( var i:int = 0; i < InfoEntries.length; i++ )
				{
					if( isNaN( infoEntries[ i ].floorValue ) )
					{
						nullEntry = infoEntries.splice( i, 1 )[ 0 ];
						break;
					}
				}
				
				//-- sort the range, and set up the ceil values from the other entries.
				//-- leave the last entry unbounded ( since it doesn't have a ceiling )
				infoEntries = InfoEntries.sortOn( "floorValue" );
				for( i = 0; i < InfoEntries.length; i++ )
				{
					if( i + 1 < InfoEntries.length )
					{
						infoEntries[ i ].setCeilData( infoEntries[ i + 1 ].floorValue );	
					}
					infoEntries[ i ].isRanged = true;
				}
				
				//-- re-insert the null info entry if it exists.
				if( nullEntry )
				{
					infoEntries.push( nullEntry );
				}
				else
				{
					infoEntries.push( new InfoEntry( NaN, "null", "null" ) );
				}
			}
			//-- is a state interpolator, meaning each value should map 1 to 1 to a info. if a value is not supported,
			//-- the null info is returned.
			else
			{
				//-- Check for the null value, add it if it doesn't exist.
				var hasNull:Boolean = false;
				for( i = 0; i < InfoEntries.length; i++ )
				{
					if( isNaN( infoEntries[ i ].floorValue ) )
					{
						hasNull = true;
						break;
					}
				}
				
				if( !hasNull )
				{
					infoEntries.push( new InfoEntry( NaN, "null", "null" ) );
				}
			}
		}
		
		public function buildInfoPlotParts():Array
		{
			var partMap:Object = new Object();
			var plotParts:Array = new Array();
			
			var info:Array;
			var tooltip:String;
			var weblink:String;
			
			//-- build tooltip/weblink lookup table, group each index by its tooltip & weblink
			var n:int = dataSet.length;
			for( var i:int = 0; i < n; i++ )
			{
				info = getColorAlpha( dataSet[ i ] );
				tooltip = info[ 0 ];
				weblink = info[ 1 ];
				
				if( !partMap[ tooltip ] )
				{
					partMap[ tooltip ] = new Object();
				}
				if( !partMap[ tooltip ][ weblink ] )
				{
					partMap[ tooltip ][ weblink ] = new Array();
				}
				partMap[ tooltip ][ weblink ].push( i );
			}
			
			//-- convert index groups into a path data string
			var indexes:Array;
			var pointPairGroups:Array;
			var pointPairs:Array;
			var lastIndex:int = 0;
			for ( var tooltipKey:String in partMap )
			{
				tooltip = tooltipKey;
				for ( var alphaKey:String in partMap[ tooltipKey ] )
				{
					weblink = alphaKey;
					indexes = partMap[ tooltipKey ][ alphaKey ];
					lastIndex = indexes[ 0 ] - 1;
					pointPairGroups = new Array();
					pointPairs = new Array();
					n = indexes.length;
					for( i = 0; i < n; i++ )
					{
						//-- accounting for the fact that a line has no real width, so each group of points will need a 1 px wide extender
						if( indexes[ i ] != lastIndex + 1 )
						{
							pointPairs.push( [ new Point( indexes[ i - 1 ] + 1, 0 ), new Point( indexes[ i - 1 ] + 1, 1 ) ] );
							pointPairGroups.push( pointPairs );
							
							pointPairs = new Array();
						}
						
						pointPairs.push( [ new Point( indexes[ i ], 0 ), new Point( indexes[ i ], 1 ) ] );

						lastIndex = indexes[ i ]; 
					}
					
					pointPairs.push( [ new Point( indexes[ i - 1 ] + 1, 0 ), new Point( indexes[ i - 1 ] + 1, 1 ) ] );
					pointPairGroups.push( pointPairs );
					
					plotParts.push( new InfoPlotPartVO( tooltip, weblink, PathUtilities.sideStitchPointPairGroups( pointPairGroups, true, false, "", true ) ) );
				}
			}		
			return plotParts;
		}
		
		private function getColorAlpha( value:Number ):Array
		{
			var info:Array;
			for each( var infoEntry:InfoEntry in infoEntries )
			{
				info = infoEntry.getTooltipWeblink( value );
				if( info ) return info;	
			}
			return [ "null", "null" ];
		}
	}
}