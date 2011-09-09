package com.ts.presentation.view.graphs
{
	import com.ts.aspect.DisplayAdornerVO;
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.grid.ApplicationGridProxy;
	import com.ts.presentation.model.grid.GridProxy;
	import com.ts.presentation.view.grid.GridMediator;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	import mx.collections.ArrayCollection;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class HistoryGraphMediator extends Mediator
	{
		public static const NAME:String = "historyGraphMediator";
		
		private var _gridProxy:GridProxy;
		private var _playbackProxy:PlaybackProxy;
		
		public function get view():HistoryGraphView
		{
			return viewComponent as HistoryGraphView;
		}
		
		public function HistoryGraphMediator( viewComponent:HistoryGraphView, playbackProxy:PlaybackProxy )
		{
			super( NAME, viewComponent );
			
			_playbackProxy = playbackProxy;
			_gridProxy = new GridProxy();
			
			facade.registerMediator( new HistoryGraphGridMediator( view.grid, playbackProxy, _gridProxy, NAME ) );
			
			BindingUtils.bindProperty( _gridProxy, "currentSecond",		_playbackProxy, "currentSecond" );
			BindingUtils.bindProperty( _gridProxy, "totalSeconds", 		_playbackProxy, "totalSeconds" );
			
			//BindingUtils.bindProperty( view,		"historyOffset",	_gridProxy, 	"historySpan" );
			BindingUtils.bindProperty( view, 		"hScrollPos",		_gridProxy,     "worldScrollP" );
			
			view.addEventListener( "refreshMeasure", handleEvent );
		}
		
		override public function listNotificationInterests():Array
		{
			return[ DataSelectionProxy.NOTE_UPDATE_DISPLAY ];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case DataSelectionProxy.NOTE_UPDATE_DISPLAY:
					if( notification.getBody() )
					{
						view.historyGraphDataProviders = DisplayAdornerVO( notification.getBody() ).hGraphData;
						view.hScrollPos = _gridProxy.worldScrollP;
					}
					else
					{
						view.historyGraphDataProviders = new ArrayCollection( [] );
					}
					break;
				
				default:
					break;
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
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