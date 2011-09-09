package com.ts.presentation.model.map.intersection
{
	import com.ts.presentation.model.map.phase.PhaseProxy;

	public class IntersectionIndex
	{
		public var intersectionNumbers:Array;
		public var intersections:Array;
		public var intersectionLookup:Object;
		
		public function IntersectionIndex( Intersections:Array )
		{
			intersections = Intersections;
			intersectionNumbers = new Array();
			intersectionLookup = new Object();
			
			for each( var intersection:IntersectionProxy in Intersections )
			{
				intersectionLookup[ intersection.intersectionNumber ] = intersection;
				intersectionNumbers.push( intersection.intersectionNumber );
			}
		}
		
		public function get phaseNumVOs():Array
		{
			var phaseNumVOs:Array = new Array();
			for each( var intersection:IntersectionProxy in intersections )
			{
				phaseNumVOs = phaseNumVOs.concat( intersection.PhaseNumVOs );				
			}
			return phaseNumVOs;
		}
		
		public function getIntersectionAtNumber( intersectionNum:int ):IntersectionProxy
		{
			return intersectionLookup[ intersectionNum ];
		}
		
		public function get systemPhaseLinkData():Array
		{
			var linkData:Array = new Array();
			for each( var intersection:IntersectionProxy in intersections )
			{
				linkData = linkData.concat( intersection.intersectionPhaseLinkData );				
			}
			
			return linkData;
		}
		
		public function hasLink( linkNum:int ):Boolean
		{
			for each( var intersection:IntersectionProxy in intersections )
			{
				if( intersection.hasLink( linkNum ) ) return true;
			}
			
			return false;
		}
		
		public function getPhaseNumAtLink( linkNum:int ):int
		{
			for each( var intersection:IntersectionProxy in intersections )
			{
				if( intersection.hasLink( linkNum ) ) return intersection.getPhaseNumAtLink( linkNum );
			}
			
			return -1;
		}
		
		public function getPhaseAtUID( UID:String ):PhaseProxy
		{
			for each( var intersection:IntersectionProxy in intersections )
			{
				if( intersection.hasUniquePhase( UID ) ) return intersection.getPhaseAtUID( UID );
			}
			
			return null;
		}
		
		public function getPhaseAtLink( linkNum:int ):PhaseProxy
		{
			for each( var intersection:IntersectionProxy in intersections )
			{
				if( intersection.hasLink( linkNum ) ) return intersection.getPhaseAtLink( linkNum );
			}
			
			return null;
		}
		
		public function getIntersectionNumAtLink( linkNum:int ):int
		{
			for each( var intersection:IntersectionProxy in intersections )
			{
				if( intersection.hasLink( linkNum ) ) return intersection.intersectionNumber;
			}
			
			return -1;
		}
	}
}