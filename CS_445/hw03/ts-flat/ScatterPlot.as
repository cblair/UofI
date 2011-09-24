package com.ts.presentation.view.decorationAssets.plotters
{
	import com.ts.utilities.drawing.PathUtilities;
	
	import flash.geom.Point;

	public class ScatterPlot
	{
		public static function createDiamondMarker( radius:int, color:uint ):ColoredMarkerVO
		{
			var markerData:Array = new Array();
			markerData.push( new Point( 0, -radius / 2 ) );
			markerData.push( new Point( radius / 2, 0 ) );
			markerData.push( new Point( 0, radius / 2 ) );
			markerData.push( new Point( -radius / 2, 0 ) );
			
			return new ColoredMarkerVO( color, PathUtilities.pointsToPathData( markerData ) );
		}
		
		public static function createPointMarker( color:uint ):ColoredMarkerVO
		{
						return new ColoredMarkerVO( color, "M 0 0 L 0.5 0" );
		}
	}
}