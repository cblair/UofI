package com.ts.aspect.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.presentation.model.map.intersection.IntersectionProxy;
	
	import mx.collections.ArrayCollection;

	public class IntersectionDecorationProxy extends LodDecorationProxy
	{
		public var intersectionNumber:int;
		
		public function IntersectionDecorationProxy( IntersectionNumber:int, HistoryGraphData:Array, Host:IntersectionProxy )
		{
			intersectionNumber 	= IntersectionNumber;
			super( IntersectionNumber, LevelOfDetail.INTERSECTION, HistoryGraphData, [], Host );
		}
		
		public function get intersection():IntersectionProxy
		{
			return host as IntersectionProxy;
		}
	}
}