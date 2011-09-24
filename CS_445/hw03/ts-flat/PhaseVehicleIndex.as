package com.ts.presentation.model.map.phase
{
	import com.ts.presentation.model.map.vehicle.VehicleProxy;

	public class PhaseVehicleIndex
	{
		public var vehTimeTable:Array;
		public var vehRangeTable:Object;
		
		public function PhaseVehicleIndex( length:int )
		{
			vehTimeTable = new Array();
			vehRangeTable = new Object();
			
			for( var i:int = 0; i < length; i++ )
			{
				vehTimeTable.push( new Array() )
			}
		}
		
		public function addVehicle( vehProxy:VehicleProxy, interval:int, pos:Number ):void
		{
			vehTimeTable[ interval ].push( vehProxy );
			if( !vehRangeTable[ vehProxy.lodID ] )
			{
				vehRangeTable[ vehProxy.lodID ] = new Array();
			}
			
			vehRangeTable[ vehProxy.lodID ].push( { time:interval, percent:pos } );
		}
	}
}