package com.ts.presentation.model.map.vo
{
	public class SystemMetadataVO
	{
		public var minIterations:int;
		public var maxIterations:int;
		public var numIntersections:int;
		public var numPhases:int;
		public var numVehicles:int;
		public var timeResolution:Number;
		public var pxPerFt:Number;
		public var aspect:String;
		
		public function SystemMetadataVO( 	MinIterations:int, 		MaxIterations:int, 	NumIntersections:int, 	NumPhases:int, NumVehicles:int,
											TimeResolution:Number,	PxPerFt:Number, Aspect:String )
		{
			minIterations 		= MinIterations;
			maxIterations 		= MaxIterations;
			numIntersections 	= NumIntersections;
			numPhases 			= NumPhases;
			numVehicles 		= NumVehicles;
			timeResolution 		= TimeResolution;
			pxPerFt				= PxPerFt;
			aspect				= Aspect;
		}
	}
}