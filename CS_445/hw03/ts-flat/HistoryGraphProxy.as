package com.ts.aspect.development
{
	[Bindable]
	public class HistoryGraphProxy
	{
		public var label:HistoryLabel;
		public var plots:Array;
		public var stackPolicy:String;
		
		public function HistoryGraphProxy( Label:HistoryLabel, Plots:Array, StackPolicy:String )
		{
			label = Label;
			plots = Plots;
			stackPolicy = StackPolicy;
		}
	}
}