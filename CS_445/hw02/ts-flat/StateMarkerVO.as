package com.ts.presentation.model.notification.vo
{
	public class StateMarkerVO
	{
		public var longLabelText:String;
		public var shortLabelText:String;
		public var iconColor:uint;
		public var iconAlpha:Number;
		public var startInterval:int;
		public var endInterval:int;
		
		public function StateMarkerVO( LongLabelText:String, ShortLabelText:String, IconColor:uint, IconAlpha:Number, StartInterval:int, EndInterval:int )
		{
			longLabelText 	= LongLabelText;
			shortLabelText 	= ShortLabelText;
			iconColor 		= IconColor;
			iconAlpha		= IconAlpha;
			startInterval 	= StartInterval;
			endInterval 	= EndInterval;
		}
	}
}