package com.ts.presentation.view.perdue
{
	import com.ts.aspect.DisplayAdornerVO;
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.grid.ApplicationGridProxy;
	import com.ts.presentation.model.grid.GridProxy;
	import com.ts.presentation.model.map.MapProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	import com.ts.presentation.model.perdue.PerdueProxy;
	import com.ts.presentation.view.grid.GridMediator;
	
	import flash.events.Event;
	
	import mx.binding.utils.BindingUtils;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class PerdueMediator extends Mediator
	{
		public static const NAME:String = "perdueMediator";
		
		private var _gridProxy:GridProxy;
		private var _playbackProxy:PlaybackProxy;
		private var _perdueProxy:PerdueProxy;
		
		public function get view():PerdueView
		{
			return viewComponent as PerdueView;
		}
		
		public function PerdueMediator( viewComponent:PerdueView, playbackProxy:PlaybackProxy )
		{
			super( NAME, viewComponent );
			
			_playbackProxy = playbackProxy;
			_gridProxy = new GridProxy();
			
			facade.registerMediator( new PerdueGridMediator( view.grid, playbackProxy, _gridProxy, NAME ) );
			
			BindingUtils.bindProperty( _gridProxy, "currentSecond",		_playbackProxy, "currentSecond" );
			BindingUtils.bindProperty( _gridProxy, "totalSeconds", 		_playbackProxy, "totalSeconds" );
			view.addEventListener( "refreshMeasure", handleEvent );
			
			_playbackProxy.eventDispatcher.addEventListener( PlaybackProxy.UPDATE_INTERVAL, handleEvent );
			_gridProxy.eventDispatcher.addEventListener( GridProxy.UPDATE_PX_PER_SECOND, handleEvent );
			
			view.totalIntervals = _playbackProxy.totalIntervals;
			view.resolution		= _gridProxy.pxPerSecond * _playbackProxy.resolutionInSeconds;
			
			BindingUtils.bindProperty( view, "offsetInterval", _gridProxy, "historySpan" );			
		}
		
		override public function onRegister():void
		{
			_perdueProxy	= facade.retrieveProxy( PerdueProxy.NAME ) as PerdueProxy;
		}
		
		override public function onRemove():void
		{
			
		}
		
		override public function listNotificationInterests():Array
		{
			return [ MapProxy.NOTE_HIGHLIGHT_VEHICLE,
					 MapProxy.NOTE_UNHIGHLIGHT_VEHICLE,
					 MapProxy.NOTE_UPDATE_VEHICLE_SELECTION,
					 MapProxy.NOTE_UPDATE_PHASE_SELECTION ]
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case MapProxy.NOTE_HIGHLIGHT_VEHICLE:
					if( notification.getType() == MapProxy.NAME )
					{
						view.highlightMarker( int( notification.getBody() ) );
					}
					break;
				
				case MapProxy.NOTE_UNHIGHLIGHT_VEHICLE:
					if( notification.getType() == MapProxy.NAME )
					{
						view.unhighlightMarker( int( notification.getBody() ) );
					}
					break;
				
				case MapProxy.NOTE_UPDATE_VEHICLE_SELECTION:
					if( notification.getType() == MapProxy.NAME )
					{
						view.highlightGroup( notification.getBody() as Array );
					}
					break;
				
				case MapProxy.NOTE_UPDATE_PHASE_SELECTION:
					if( notification.getType() == MapProxy.NAME )
					{
						var phaseUIDs:Array = notification.getBody() as Array;
						var plots:Array = new Array();
						for each( var phaseUID:String in phaseUIDs )
						{
							plots.push( _perdueProxy.getPlotAtPhaseUID( phaseUID ) );
						}
						view.plotData = plots;
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
				case PlaybackProxy.UPDATE_INTERVAL:
					view.currentInterval = _playbackProxy.currentInterval;
					break;
				
				case GridProxy.UPDATE_PX_PER_SECOND:
					view.resolution = _gridProxy.pxPerSecond * _playbackProxy.resolutionInSeconds;
				
				case "refreshMeasure":
					_gridProxy.viewportWidth 	= view.gridWidth;
					_gridProxy.viewportHeight 	= view.gridHeight;
					break;
					
				default:
					break;
			}
		}
	}
}