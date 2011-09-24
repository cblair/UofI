package com.ts.aspect.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	
	import mx.collections.ArrayCollection;

	public class PhaseDecorationProxy extends LodDecorationProxy
	{
		public var phaseNumber:int;
		public var intersectionNumber:int;
		
		public function PhaseDecorationProxy( PhaseNumber:int, IntersectionNumber:int, HistoryGraphProxies:Array, NotificationProxies:Array, Host:PhaseProxy )
		{
			phaseNumber 		= PhaseNumber;
			intersectionNumber 	= IntersectionNumber;
			super( PhaseNumber, LevelOfDetail.PHASE, HistoryGraphProxies, NotificationProxies, Host )
		}
		
		public function get phase():PhaseProxy
		{
			return host as PhaseProxy;
		}
	}
}