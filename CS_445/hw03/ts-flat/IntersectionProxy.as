package com.ts.presentation.model.map.intersection
{
	
	import com.ts.presentation.model.map.phase.PhaseIndex;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class IntersectionProxy extends Proxy
	{
		public static const NAME:String = "intersectionProxy";
		
		public var phaseIndex:PhaseIndex;
		public var intersectionNumber:int;
		
		public function IntersectionProxy( phaseIndx:PhaseIndex, IntersectionNumber:int )
		{
			super( NAME );
			phaseIndex 			= phaseIndx;
			intersectionNumber 	= IntersectionNumber;
		}
		
		public function get PhaseNumVOs():Array{ return phaseIndex.phaseNumVOs; }
		
		public function getPhaseAtNumber( phaseNum:int ):PhaseProxy
		{ 
			return phaseIndex.getPhaseAtNumber( phaseNum ); 
		}
		
		public function getPhaseAtUID( uniqueID:String ):PhaseProxy
		{ 
			return phaseIndex.getPhaseAtUniqueID( uniqueID ); 
		}
		
		public function getPhases():Array
		{
			return phaseIndex.phases;
		}
		
		public function get intersectionPhaseLinkData():Array{ return phaseIndex.intersectionPhaseLinkData; }
		
		public function getPhaseNumAtLink( linkNum:int ):int
		{
			return phaseIndex.getPhaseNumAtLink( linkNum );
		}
		
		public function getPhaseAtLink( linkNum:int ):PhaseProxy
		{
			return phaseIndex.getPhaseAtLink( linkNum );
		}
		
		public function hasLink( linkNum:int ):Boolean
		{
			return phaseIndex.hasLink( linkNum );
		}
		
		public function hasUniquePhase( UID:String ):Boolean
		{
			return( phaseIndex.hasUniquePhase( UID ) );
		}
	}
}