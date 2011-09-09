package com.ts.presentation.model.grid
{
	public class TimeLineVO
	{
		public var time:String;
		public var span:int;
		
		public function TimeLineVO( Time:String, Span:int )
		{
			time 	= Time;
			span 	= Span;
		}
	}
}