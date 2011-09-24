package com.ts.presentation.model.notification
{
	public class NotificationPlotProxy
	{
		public var infoPlotParts:Array;
		public var decorationPlotParts:Array;
		
		private var _decorationMap:Object;
		
		public function NotificationPlotProxy( InfoPlotParts:Array, DecorationPlotParts:Array )
		{
			infoPlotParts 		= InfoPlotParts;
			decorationPlotParts = DecorationPlotParts;
			
			_decorationMap = new Object();
			
			for each( var decoration:NotificationDecorationEntry in DecorationPlotParts )
			{
				_decorationMap[ decoration.state ] = decoration;
			}
		}
		
		public function getDecorationEntryAtStateNum( value:int ):NotificationDecorationEntry
		{
			return _decorationMap[ value ];
		}
	}
}