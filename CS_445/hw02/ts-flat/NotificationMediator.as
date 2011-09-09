package com.ts.presentation.view.notification
{
	import com.ts.aspect.DisplayAdornerVO;
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.grid.ApplicationGridProxy;
	import com.ts.presentation.model.grid.GridProxy;
	import com.ts.presentation.view.grid.GridMediator;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class NotificationMediator extends Mediator
	{
		public static const NAME:String = "notificationMediator";
		
		private var _gridProxy:GridProxy;
		private var _playbackProxy:PlaybackProxy;
		private var _appGridProxy:ApplicationGridProxy;
		
		public function get view():NotificationView
		{
			return viewComponent as NotificationView;
		}
		
		public function NotificationMediator( viewComponent:NotificationView, playbackProxy:PlaybackProxy )
		{
			super( NAME, viewComponent );
			
			_playbackProxy = playbackProxy;
			_gridProxy = new GridProxy( NAME );
			_gridProxy.linesPerRule = 1;
			_gridProxy.secondsPerLine = 5;
			
			facade.registerMediator( new NotificationGridMediator( view.notificationGrid, playbackProxy, _gridProxy, NAME ) );
			BindingUtils.bindProperty(  _gridProxy, "currentSecond",	playbackProxy, "currentSecond" );
			BindingUtils.bindProperty(  _gridProxy, "totalSeconds", 	playbackProxy, "totalSeconds" );
			
			view.addEventListener( "refreshMeasure", handleEvent );
			_playbackProxy.eventDispatcher.addEventListener( PlaybackProxy.UPDATE_INTERVAL, handleEvent );
			view.notificationList.totalIntervals = _playbackProxy.totalIntervals;
			view.notificationList.resolution = _gridProxy.pxPerSecond * _playbackProxy.resolutionInSeconds;
			//view.notificationList.offsetInterval = _gridProxy.hOffset;
			
			BindingUtils.bindProperty( view.notificationList, "offsetInterval", _gridProxy, "historySpan" );
			
		}
		
		override public function listNotificationInterests():Array
		{
			return [ DataSelectionProxy.NOTE_UPDATE_DISPLAY ]
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case DataSelectionProxy.NOTE_UPDATE_DISPLAY:
					var adorner:DisplayAdornerVO = notification.getBody() as DisplayAdornerVO;
					if( adorner )
					{
						view.notificationList.notificationProxies = adorner.notificationData;
					}
					else
					{
						view.notificationList.notificationProxies = [];
					}
					break;
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case PlaybackProxy.UPDATE_INTERVAL:
					view.notificationList.currentInterval = _playbackProxy.currentInterval;
					break;
				
				case "refreshMeasure":
					_gridProxy.viewportWidth = view.gridWidth;
					_gridProxy.viewportHeight = view.gridHeight;
					break;
				
				default:
					break;
					
			}
		}
	}
}