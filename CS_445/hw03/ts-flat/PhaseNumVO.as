package com.ts.presentation.model.map.phase.vo
{
	public class PhaseNumVO
	{
		public var phaseNum:int;
		public var intersectionNum:int;
		
		public function PhaseNumVO( PhaseNum:int, IntersectionNum:int )
		{
			phaseNum = PhaseNum;
			intersectionNum = IntersectionNum;
		}
	}
}