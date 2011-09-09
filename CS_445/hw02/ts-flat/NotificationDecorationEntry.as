package com.ts.presentation.model.notification
{
	public class NotificationDecorationEntry
	{
		public var state:int;
		public var color:uint;
		public var alpha:Number;
		
		public function NotificationDecorationEntry( State:int, Color:uint, Alpha:Number )
		{
			state = State;
			color = Color;
			alpha = Alpha;
		}
	}
}