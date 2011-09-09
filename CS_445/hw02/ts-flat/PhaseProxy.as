package com.ts.presentation.model.map.phase
{
	import com.ts.presentation.view.map.controls.PhaseControl;
	import com.ts.presentation.model.map.phase.vo.LinkDataVO;
	import com.ts.presentation.model.map.phase.vo.PhaseLinkVO;
	import com.ts.presentation.model.map.phase.vo.PhaseNumVO;
	import com.ts.presentation.model.map.phase.vo.PhaseVO;
	import com.ts.utilities.drawing.PathUtilities;
	
	import flash.geom.Point;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class PhaseProxy extends Proxy
	{
		public static const NAME:String = "phaseProxy";
		
		public var linkIndex:LinkIndex;
		//public var detectorLink:LinkProxy;
		//public var detectorPos:Number;
		//public var detectorPoint:Point;
		public var startLink:LinkProxy;
		public var endLink:LinkProxy;
		public var signalIndex:SignalIndex;
		public var uniqueID:String;
		public var phaseVehIndex:PhaseVehicleIndex;
		public var phaseControl:PhaseControl;
		
		public function PhaseProxy( LinkIndx:LinkIndex, SignalIndx:SignalIndex )
		{
			super( NAME );
			linkIndex			= LinkIndx;
			//detectorLink 		= DetectorLink;
			//detectorPos 		= DetectorPos;
			startLink			= LinkIndx.startLink;
			endLink				= LinkIndx.endLink;
			signalIndex			= SignalIndx;
			uniqueID			= String( signalNumber ) + "-" + String( intersectionNumber );
			
			phaseControl		= new PhaseControl();
			phaseControl.phaseLinkData = phaseLinkData;
		}
		
		public function get phaseNumVO():PhaseNumVO{ return new PhaseNumVO( signalNumber, intersectionNumber ); }
		public function get signalNumber():int{ return signalIndex.groupNum; }
		public function get intersectionNumber():int{ return signalIndex.intersectionNum; }
		
		public function get phaseLinkData():PhaseLinkVO
		{
			return new PhaseLinkVO( linkIndex.linkData, signalNumber, signalIndex, intersectionNumber, uniqueID );
		}
		
		public function hasLink( linkNum:int ):Boolean
		{
			return linkIndex.hasLink( linkNum );
		}

		public function getDistanceAlongPath( pos:Number, linkNum:int ):Number
		{
			return linkIndex.getDistanceAlongPath( pos, linkNum );
		}
		
		public function getPercentAlongPath( pos:Number, linkNum:int ):Number
		{
			return linkIndex.getPercentAlongPath( pos, linkNum );
		}
	}
}