package com.ts.aspect.development
{
	[Bindable]
	public class HistoryLabel
	{
		public var title:String;
		public var yAxisValues:Array;
		public var units:String;
		
		public function HistoryLabel( Title:String, YAxisValues:Array, Units:String )
		{
			title 		= Title;
			yAxisValues = YAxisValues;
			units 		= Units;
		}
	}
}