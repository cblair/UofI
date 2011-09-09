package com.ts.presentation.model.map.phase.vo
{
	import com.ts.presentation.model.map.phase.LinkIndex;

	public class PhaseVO
	{
		public var phaseName:String;
		public var phaseNumber:int;
		public var linkIndex:LinkIndex;
		public var intersectionNumber:int
		
		public function PhaseVO( PhaseNumber:int, LinkIndx:LinkIndex, IntersectionNumber:int )
		{
			phaseName 			= String( PhaseNumber );
			phaseNumber 		= PhaseNumber;
			linkIndex 			= LinkIndx;
			intersectionNumber 	= IntersectionNumber;
		}
	}
}