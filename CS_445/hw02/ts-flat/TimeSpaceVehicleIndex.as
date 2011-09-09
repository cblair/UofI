package com.ts.presentation.model.timeSpace
{
	import com.ts.presentation.model.map.phase.PhaseVehicleIndex;

	public class TimeSpaceVehicleIndex
	{
		public var timeTable:Object;
		public var spaceTable:Object;
		public var vehicleIndex:Array;
		public var phaseLength:Number;
		
		public function TimeSpaceVehicleIndex( tsTable:Object, PhaseLength:Number )
		{
			phaseLength = PhaseLength;
			timeTable = new Object();
			spaceTable = new Object();
			vehicleIndex = new Array();
			
			var index:int;
			for( var key:String in tsTable )
			{
				index = int( key );
				vehicleIndex.push( index );
				timeTable[ index ] = new Array();
				spaceTable[ index ] = new Array();
				
				for each( var tsRecord:Object in tsTable[ key ] )
				{
					timeTable[ index ].push( tsRecord.time );
					spaceTable[ index ].push( Math.round( tsRecord.percent * PhaseLength * 100 ) / 100 );
				}
			}
		}
	}
}