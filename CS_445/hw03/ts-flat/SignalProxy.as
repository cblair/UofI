package com.ts.presentation.model.map.phase
{
	import flash.geom.Point;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class SignalProxy extends Proxy
	{
		public var groupNum:int;
		public var intersectionNum:int;
		public var linkNum:int;
		public var position:Point;
		public var at:Number;
		public var angle:Number;
		public var numLanes:int;
		
		public static const NAME:String	= "signalProxy";
		public function SignalProxy( GroupNum:int, IntersectionNum:int, LinkNum:int, Position:Point, At:Number, Angle:Number, NumLanes:int )
		{
			super( NAME );
			
			groupNum 		= GroupNum;
			intersectionNum = IntersectionNum;
			linkNum 		= LinkNum;
			position 		= Position;
			at				= At;
			angle			= Angle;
			numLanes		= NumLanes;
		}
	}
}