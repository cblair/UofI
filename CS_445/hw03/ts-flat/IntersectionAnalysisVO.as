package com.ts.importing.deriveSystem.vo
{
	import flash.geom.Point;

	public class IntersectionAnalysisVO
	{
		public var topLeftPoint:Point;
		public var bottomRightPoint:Point;
		public var numIntersections:int;
		
		public function IntersectionAnalysisVO( TopLeftPoint:Point, BottomRightPoint:Point, NumIntersections:int )
		{
			topLeftPoint 		= TopLeftPoint;
			bottomRightPoint 	= BottomRightPoint;
			numIntersections 	= NumIntersections;
		}
	}
}