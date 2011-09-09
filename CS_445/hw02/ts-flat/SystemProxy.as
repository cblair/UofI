package com.ts.presentation.model.map
{
	import com.ts.presentation.model.map.phase.PhaseProxy;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	import com.ts.presentation.model.map.vo.AspectVO;
	import com.ts.presentation.model.map.vo.SystemMetadataVO;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class SystemProxy extends Proxy
	{
		public static const NAME:String	= "systemProxy";
		
		public static const NOTE_FINALIZE_APPLICATION:String	= NAME + "/note/finalizeApplication";
		
		public var currentAspect:AspectVO;
		public var metadata:SystemMetadataVO;
		public var trafficModel:TrafficProxy;
		public var networkModel:NetworkProxy;
		
		public function SystemProxy( Metadata:SystemMetadataVO, TrafficModel:TrafficProxy, NetworkModel:NetworkProxy )
		{
			super( NAME );
			metadata 		= Metadata;
			trafficModel 	= TrafficModel;
			networkModel	= NetworkModel;
		}
		
		public function get minIterations()		:int{ return metadata.minIterations; }
		public function get maxIterations()		:int{ return metadata.maxIterations; }
		public function get numIntersections()	:int{ return metadata.numIntersections; }
		public function get numPhases()			:int{ return metadata.numPhases; }
		public function get numVehicles()		:int{ return metadata.numVehicles; }
		public function get timeResolution()	:Number{ return metadata.timeResolution; }
		public function get pxPerFt()			:Number{ return metadata.pxPerFt; }
		public function get aspect()			:String{ return metadata.aspect; }
		public function get intersections()		:Array{ return networkModel.intersections; }
		public function get phases()			:Array{ return networkModel.phases; }
		public function get phaseNumVOs()		:Array{ return networkModel.phaseNumVOs; }
		public function get vehicles()			:Array{ return trafficModel.vehicles; }
		public function get vehicleProperties()	:Array{ return trafficModel.supportedPropertyNames; }
		
		public function hasLink( linkNum:int ):Boolean
		{
			return networkModel.hasLink( linkNum );
		}
		
		public function getIntersectionNumAtLink( linkNum:int ):int
		{
			return networkModel.getIntersectionNumAtLink( linkNum );
		}
		
		public function getPhaseNumAtLink( linkNum:int ):int
		{
			return networkModel.getPhaseNumAtLink( linkNum );
		}
	} 
}