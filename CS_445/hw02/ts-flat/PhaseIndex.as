package com.ts.presentation.model.map.phase
{
	import com.ts.presentation.model.map.phase.vo.PhaseLinkVO;

	public class PhaseIndex
	{
		public var phaseNumbers:Array;
		public var uniqueIDs:Array;
		public var phases:Array;
		public var phaseLookup:Object;
		public var uniqueLookup:Object;
		
		public function PhaseIndex( Phases:Array )
		{
			phases 			= Phases;
			phaseLookup 	= new Object();
			phaseNumbers 	= new Array();
			uniqueLookup 	= new Object();
			uniqueIDs		= new Array();
			
			for each( var phase:PhaseProxy in Phases )
			{
				phaseLookup[ phase.signalNumber ] = phase;
				uniqueLookup[ phase.uniqueID ] = phase;
				
				phaseNumbers.push( phase.signalNumber );
				uniqueIDs.push( phase.uniqueID );
			}
		}
		
		public function get phaseNumVOs():Array
		{
			var phaseNums:Array = new Array();
			for each( var phase:PhaseProxy in phases )
			{
				phaseNums.push( phase.phaseNumVO );
			}
			return phaseNums;
		}
		
		public function getPhaseAtNumber( phaseNum:int ):PhaseProxy
		{
			return phaseLookup[ phaseNum ];
		}
		
		public function getPhaseAtUniqueID( uniqueID:String ):PhaseProxy
		{
			return uniqueLookup[ uniqueID ];
		}
		
		public function getPhaseLinkData( phaseNum:int ):PhaseLinkVO
		{
			if( !phaseLookup[ phaseNum ] ) return null;
			
			return phaseLookup[ phaseNum ].phaseLinkData;
		}
		
		public function get intersectionPhaseLinkData():Array
		{
			var linkData:Array = new Array();
			for each( var phase:PhaseProxy in phases )
			{
				linkData.push( phase.phaseLinkData );
			}
			
			return linkData;
		}
		
		public function hasLink( linkNum:int ):Boolean
		{
			for each( var phase:PhaseProxy in phases )
			{
				if( phase.hasLink( linkNum ) ) return true;
			}
			
			return false;
		}
		
		public function hasUniquePhase( UID:String ):Boolean
		{
			for each( var phase:PhaseProxy in phases )
			{
				if( phase.uniqueID == UID ) return true;
			}
			
			return false;
		}
		
		public function getPhaseNumAtLink( linkNum:int ):int
		{
			for each( var phase:PhaseProxy in phases )
			{
				if( phase.hasLink( linkNum ) ) return phase.signalNumber;
			}
			
			return -1;
		}
		
		public function getPhaseAtLink( linkNum:int ):PhaseProxy
		{
			for each( var phase:PhaseProxy in phases )
			{
				if( phase.hasLink( linkNum ) ) return phase;
			}
			
			return null;
		}
	}
}