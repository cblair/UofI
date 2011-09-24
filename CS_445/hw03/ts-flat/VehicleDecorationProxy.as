package com.ts.aspect.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	
	import mx.collections.ArrayCollection;

	public class VehicleDecorationProxy extends LodDecorationProxy
	{
		public var vehicleNumber:int;
		
		public function VehicleDecorationProxy( VehicleNumber:int, HistoryGraphData:Array, Host:VehicleProxy )
		{
			vehicleNumber 		= VehicleNumber;
			super( VehicleNumber, LevelOfDetail.VEHICLE, HistoryGraphData, [], Host );
		}
		
		public function get vehicle():VehicleProxy
		{
			return host as VehicleProxy;
		}
	}
}