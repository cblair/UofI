package com.ts.presentation.model.map.vehicle.vo
{
	import com.ts.data.model.DataProxy;

	public class RequiredVehData
	{
		public var inSystem:DataProxy;
		public var vehXPos:DataProxy;
		public var vehYPos:DataProxy;
		public var vehRotation:DataProxy;
		public var phase:DataProxy;
		public var percent:DataProxy;
		public var intersection:DataProxy;
		
		public function RequiredVehData( InSystem:DataProxy, VehXPos:DataProxy, VehYPos:DataProxy, VehRotation:DataProxy )
		{
			inSystem 	= InSystem;
			vehXPos 	= VehXPos;
			vehYPos 	= VehYPos;
			vehRotation = VehRotation;
		}
	}
}