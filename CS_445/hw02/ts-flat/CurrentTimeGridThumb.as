package com.ts.presentation.view.grid
{
	import spark.components.Button;
	
	public class CurrentTimeGridThumb extends Button
	{
		[Bindable] public var thumbColor:uint;
		
		public function CurrentTimeGridThumb()
		{
			super();
			setStyle( "skinClass", com.ts.presentation.view.grid.CurrentTimeGridThumbSkin );
		}
	}
}