package com.ts.global.view.components
{
	import spark.components.Group;

	public class TooltipVO
	{
		public var height:Number;
		public var width:Number;
		public var content:Group;
		public var padding:int;
		public var radius:Number;
		public var tipDirection:String;
		public var tipBreadth:int;
		public var tipLength:int;
		public var weblink:String;
		
		public function TooltipVO( Height:Number, Width:Number, Content:Group, Padding:int, Radius:Number = 10, TipDirection:String = "down", TipBreadth:int = 20, TipLength:int = 10, Weblink:String = null )
		{
			height 		= Height;
			width 		= Width;
			content 	= Content;
			padding		= Padding;
			radius 		= Radius;
			tipDirection= TipDirection;
			tipBreadth	= TipBreadth;
			tipLength	= TipLength;
			weblink		= Weblink;
		}
	}
}