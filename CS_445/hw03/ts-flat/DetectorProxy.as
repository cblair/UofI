package com.ts.presentation.model.map.phase
{
	import flash.geom.Point;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class DetectorProxy extends Proxy
	{
		public var groupNum:int;
		public var intersectionNum:int;
		public var linkNum:int;
		public var position:Point;
		public var length:Number;
		public var angle:Number;
		public var numLanes:int;
		
		public static const NAME:String	= "detectorProxy";
		public function DetectorProxy( GroupNum:int, IntersectionNum:int, LinkNum:int, Position:Point, Length:Number, Angle:Number, NumLanes:int )
		{
			super( NAME );
			
			groupNum 		= GroupNum;
			intersectionNum = IntersectionNum;
			linkNum 		= LinkNum;
			position 		= Position;
			length			= Length;
			angle			= Angle;
			numLanes		= NumLanes;
		}
	}
}