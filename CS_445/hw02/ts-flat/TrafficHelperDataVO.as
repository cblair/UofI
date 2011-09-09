package com.ts.importing.deriveSystem.vo
{

	/**
	 * A Virtual Object containing helper data for generating the virtual ( data calculated form imported data ) side of the required vehicle model.
	 * Used exclusively with the RequiredDataFactory static class functions.
	 **/
	public class TrafficHelperDataVO
	{
		/**
		 * Contains analysis information about the intersection, such as top left point and bottom right point.
		 * These offsets will need to be subtracted from vechicle position data to ensure the entire network has a ( 0, 0 ) x/y origin.
		 **/
		public var intersectionAnalysis:IntersectionAnalysisVO;
		
		/**
		 * Constructor.
		 * 
		 * @params IntersectionAnalysis Vehicle dependant analysis informtaion calculated on the intersection
		 **/
		public function TrafficHelperDataVO( IntersectionAnalysis:IntersectionAnalysisVO )
		{
			intersectionAnalysis = IntersectionAnalysis
		}
	}
}