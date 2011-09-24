package com.ts.presentation.model.map.vehicle.vo
{
	public class VehicleMetadataVO
	{
		public var lodID:int;
		public var enterSystemIndex:int;
		public var exitSystemIndex:int;
		public var vehLength:Number;
		
		public function VehicleMetadataVO( LodID:int, EnterSystemIndex:int, ExitSystemIndex:int, VehLength:Number )
		{
			lodID 				= LodID;
			enterSystemIndex 	= EnterSystemIndex;
			exitSystemIndex 	= ExitSystemIndex;
			vehLength 			= VehLength; 
		}
	}
}