package com.ts.presentation.view.timeSpace
{
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.grid.GridProxy;
	import com.ts.presentation.view.grid.GridMediator;
	import com.ts.presentation.view.grid.GridView;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	
	public class TimeSpaceGridMediator extends GridMediator
	{
		public static const NAME:String		= "TSGM";
		
		private var _propOrigin:Number;
		
		public function TimeSpaceGridMediator( viewComponent:GridView, playbackProxy:PlaybackProxy, gridProxy:GridProxy, name:String = "" )
		{
			super( viewComponent, playbackProxy, gridProxy, name );
						
			BindingUtils.bindProperty( view, "currentSecond", 	_gridProxy, "currentSecond" );
			BindingUtils.bindProperty( view, "historySpan", 	_gridProxy, "historySpan" );
			BindingUtils.bindProperty( view, "hLines",			_gridProxy,	"hLines" );
			BindingUtils.bindProperty( view, "hOffset",			_gridProxy,	"hOffset" );
			
			view.addEventListener( "startDrag", handleEvent );
			view.addEventListener( "zoomIn", 	handleEvent );
			view.addEventListener( "zoomOut", 	handleEvent );
			
			_gridProxy.pxPerSecond = 4;
			_gridProxy.linesPerRule = 2;
			_gridProxy.secondsPerLine = 10;
			_gridProxy.historySpan = 25;
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case "startDrag":
					view.addEventListener( "updateDrag", handleEvent );
					view.addEventListener( "endDrag", handleEvent );
					if( view.dragTarget == view.historySpanBtn )
					{
						_propOrigin = _gridProxy.historySpan;
					}
					else if( view.dragTarget == view.headerPane )
					{
						_propOrigin = _playbackProxy.currentInterval;
					}
					break;
				
				case "updateDrag":
					if( view.dragTarget == view.historySpanBtn )
					{
						_gridProxy.historySpan = _propOrigin + view.dragData.deltaX;
					}
					else if( view.dragTarget == view.headerPane )
					{
						_playbackProxy.manualySetInterval( _propOrigin - ( view.dragData.deltaX /( _gridProxy.pxPerSecond * _playbackProxy.resolutionInSeconds ) ) );
					}
					break;
				
				case "endDrag":
					if( view.dragTarget == view.historySpanBtn )
					{
						_gridProxy.historySpan = _propOrigin + view.dragData.deltaX;
					}
					else if( view.dragTarget == view.headerPane )
					{
						_playbackProxy.manualySetInterval( _propOrigin - ( view.dragData.deltaX /( _gridProxy.pxPerSecond * _playbackProxy.resolutionInSeconds ) ) );
					}
					view.removeEventListener( "updateDrag", handleEvent );
					view.removeEventListener( "endDrag", handleEvent );
					break;
				
				case "zoomIn":
					_gridProxy.pxPerSecond = Math.min( 50, _gridProxy.pxPerSecond + 1 );
					break;
				
				case "zoomOut":
					_gridProxy.pxPerSecond = Math.max( 4, _gridProxy.pxPerSecond - 1 );
					break;
				
				default:
					break;

			}
		}
	}
}