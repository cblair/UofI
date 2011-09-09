package com.ts.importing.deriveSystem.vo
{

	/**
	 * A Virtual Object containing helper data for generating the virtual ( data calculated form imported data ) side of the required intersection model.
	 * Used exclusively with the RequiredIntDataFactory static class functions.
	 **/
	public class NetworkHelperDataVO
	{
		/**
		 * Contains analysis information about the intersection, such as top left point and bottom right point.
		 * These offsets will need to be subtracted from vechicle position data to ensure the entire network has a ( 0, 0 ) x/y origin.
		 **/
		public var intersectionAnalysis:IntersectionAnalysisVO;
		
		/**
		 * Constructor.
		 * 
		 * @params IntersectionAnalysis Intersection dependant analysis
		 **/
		public function NetworkHelperDataVO( IntersectionAnalysis:IntersectionAnalysisVO )
		{
			intersectionAnalysis = IntersectionAnalysis
		}
	}
}