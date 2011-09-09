package com.ts.presentation.model.map.vehicle
{
	import com.ts.data.model.DataProxy;
	import com.ts.presentation.view.map.controls.VehicleControl;
	import com.ts.presentation.model.map.vehicle.vo.RequiredVehData;
	import com.ts.presentation.model.map.vehicle.vo.VehicleDataIndex;
	import com.ts.presentation.model.map.vehicle.vo.VehicleMetadataVO;
	
	import mx.binding.utils.BindingUtils;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class VehicleProxy extends Proxy
	{
		public static const NAME:String = "vehicleProxy";

		public var metadata:VehicleMetadataVO;
		public var requiredData:RequiredVehData;
		public var vehicleControl:VehicleControl;
		
		public function VehicleProxy( Metadata:VehicleMetadataVO, RequiredData:RequiredVehData )
		{
			metadata 		= Metadata;
			requiredData	= RequiredData;
			
			vehicleControl = new VehicleControl( String( metadata.lodID ), 2, metadata.vehLength );
			
			super( NAME );
		}
		
		public function get enterSystemIndex()	:int{ return metadata.enterSystemIndex; }
		public function get exitSystemIndex()	:int{ return metadata.exitSystemIndex; }
		public function get lodID()				:int{ return metadata.lodID; }
		public function get vehLength()			:Number{ return metadata.vehLength; }
		
		public function set vehiclePhase( value:DataProxy ):void
		{
			requiredData.phase = value;
		}
		
		public function set vehiclePercent( value:DataProxy ):void
		{
			requiredData.percent = value;
		}
		
		public function set vehicleIntersection( value:DataProxy ):void
		{
			requiredData.intersection = value;
		}
	}
}