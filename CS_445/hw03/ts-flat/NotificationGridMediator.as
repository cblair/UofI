package com.ts.presentation.view.notification
{
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.grid.ApplicationGridProxy;
	import com.ts.presentation.model.grid.GridProxy;
	import com.ts.presentation.view.grid.GridMediator;
	import com.ts.presentation.view.grid.GridView;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	
	public class NotificationGridMediator extends GridMediator
	{
		public static const NAME:String = "graphGridMediator";
		
		private var _propOrigin:Number;
		//private var _appGridProxy:ApplicationGridProxy;
		private var _historySpan:int;
		
		public function NotificationGridMediator(viewComponent:GridView, playbackProxy:PlaybackProxy, gridProxy:GridProxy, name:String = "" )
		{
			super( viewComponent, playbackProxy, gridProxy, name );
			
			//_appGridProxy = appGridProxy;
			
			//BindingUtils.bindProperty( _gridProxy, "viewportWidth", view, "width" );
			//BindingUtils.bindProperty( _gridProxy, "viewportHeight", view, "height" );
			
			BindingUtils.bindProperty( view, "currentSecond", 	_gridProxy, "currentSecond" );
			BindingUtils.bindProperty( this, "historySpan", 	_gridProxy, "historySpan" );
			BindingUtils.bindProperty( view, "historySpan", 	this, "historySpan" );
			BindingUtils.bindProperty( view, "hLines",			_gridProxy,	"hLines" );
			BindingUtils.bindProperty( view, "hOffset",			_gridProxy,	"hOffset" );
			
			view.addEventListener( "startDrag", handleEvent );
			view.addEventListener( "zoomIn", handleEvent );
			view.addEventListener( "zoomOut", handleEvent );
			
			_gridProxy.historySpan = 275;
		}
		
		[Bindable] public function get historySpan():int{ return _historySpan; }
		public function set historySpan( value:int ):void
		{
			_historySpan = value;
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
						_playbackProxy.manualySetInterval( _propOrigin - ( view.dragData.deltaX / ( _gridProxy.pxPerSecond * _playbackProxy.resolutionInSeconds ) ) );
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