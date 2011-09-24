package com.ts.presentation.model.map.phase.vo
{
	import com.ts.presentation.model.map.phase.SignalIndex;
	
	import flash.geom.Point;

	[Bindable]
	public class PhaseLinkVO
	{
		public var linkData:Array;
		public var phaseNumber:int;
		public var phaseUID:String;
		public var signalIndex:SignalIndex;
		//public var detectorPosition:Point;
		//public var detectorRotation:Number;
		//public var detectorLength:Number;
		public var intersectionNum:int;
		
		public function PhaseLinkVO( LinkData:Array, PhaseNumber:int, SignalIndx:SignalIndex, IntersectionNum:int, PhaseUID:String )
		{
			linkData 			= LinkData;
			phaseNumber 		= PhaseNumber;
			signalIndex			= SignalIndx;
			//detectorPosition 	= DetectorPosition;
			//detectorRotation 	= DetectorRotation;
			//detectorLength 		= DetectorLength;
			intersectionNum		= IntersectionNum;
			phaseUID			= PhaseUID;
		}
	}
}