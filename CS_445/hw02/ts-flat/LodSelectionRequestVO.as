package com.ts.presentation.model.vo
{
	public class LodSelectionRequestVO
	{
		public var levelOfDetail:String;
		public var lodID:int;
		public var intersectionNumber:int;
		
		public function LodSelectionRequestVO( LevelOfDetail:String, LodID:int, IntersectionNumber:int )
		{
			levelOfDetail = LevelOfDetail;
			lodID = LodID;
			intersectionNumber = IntersectionNumber;
		}
	}
}