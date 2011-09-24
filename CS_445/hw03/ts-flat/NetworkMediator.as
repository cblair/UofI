package com.ts.presentation.view.map
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.presentation.model.DataSelectionProxy;
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.map.MapProxy;
	import com.ts.presentation.model.vo.LodSelectionRequestVO;
	import com.ts.presentation.view.map.controls.PhaseControl;
	import com.ts.presentation.model.map.NetworkProxy;
	import com.ts.presentation.model.map.intersection.IntersectionProxy;
	import com.ts.presentation.model.map.phase.PhaseProxy;
	
	import flash.events.Event;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class NetworkMediator extends Mediator
	{
		public static const NAME:String	= "networkMediator";
		
		private var _networkProxy:NetworkProxy;
		private var _mapProxy:MapProxy;
		private var _playbackProxy:PlaybackProxy;
		
		public function get view():NetworkView
		{
			return viewComponent as NetworkView;
		}
		
		public function NetworkMediator( viewComponent:Object )
		{
			super( NAME, viewComponent );
		}
		
		public function addPhaseControl( value:PhaseControl ):void
		{
			view.addPhaseControl( value );
		}
		
		override public function onRegister():void
		{
			_networkProxy 	= facade.retrieveProxy( NetworkProxy.NAME ) as NetworkProxy;
			_mapProxy 		= facade.retrieveProxy( MapProxy.NAME ) as MapProxy;
			_playbackProxy	= facade.retrieveProxy( PlaybackProxy.NAME ) as PlaybackProxy;
			initializeNetwork();
			
			view.addEventListener( PhaseControl.PHASE_OVER, 			handleEvent, true );
			view.addEventListener( PhaseControl.PHASE_OUT, 				handleEvent, true );
			view.addEventListener( PhaseControl.PHASE_SELECTED_OVER, 	handleEvent, true );
			view.addEventListener( PhaseControl.PHASE_SELECTED_OUT, 	handleEvent, true );
			view.addEventListener( PhaseControl.PHASE_SELECT, 			handleEvent, true );
			view.addEventListener( PhaseControl.PHASE_ADD_SELECTED, 	handleEvent, true );
			view.addEventListener( PhaseControl.PHASE_REMOVE_SELECTED, 	handleEvent, true );
			view.addEventListener( PhaseControl.PHASE_UNSELECT, 		handleEvent, true );
			
			_playbackProxy.eventDispatcher.addEventListener( PlaybackProxy.UPDATE_INTERVAL, handleEvent );
		}
		
		override public function onRemove():void
		{
			
		}
		
		override public function listNotificationInterests():Array
		{
			return [];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				default:
					break;
			}
		}
		
		private function handleEvent( event:Event ):void
		{
			event.stopPropagation();
			switch( event.type )
			{
				case PlaybackProxy.UPDATE_INTERVAL:
					updateNetwork();
					break;
				
				case PhaseControl.PHASE_OVER:
					_mapProxy.highlightPhaseControl( PhaseControl( event.target ).controlUID );
					break;
				
				case PhaseControl.PHASE_OUT:
					_mapProxy.unhighlightPhaseControl();
					break;
				
				case PhaseControl.PHASE_SELECTED_OVER:
					break;
				
				case PhaseControl.PHASE_SELECTED_OUT:
					break;
				
				case PhaseControl.PHASE_SELECT:
					_mapProxy.selectPhaseControl( PhaseControl( event.target ).controlUID );
					
					//--- temporary
					sendNotification( DataSelectionProxy.NOTE_REQUEST_DATA_CHANGE, new LodSelectionRequestVO( LevelOfDetail.PHASE, PhaseControl( event.target ).phaseNumber, PhaseControl( event.target ).intersectionNumber ) );
					//---------------------------------------------
					break;
				
				case PhaseControl.PHASE_ADD_SELECTED:
					_mapProxy.addPhaseSelection( PhaseControl( event.target ).controlUID );
					
					//-- temporary
					sendNotification( DataSelectionProxy.NOTE_REQUEST_DATA_CHANGE, new LodSelectionRequestVO( LevelOfDetail.PHASE, PhaseControl( event.target ).phaseNumber, PhaseControl( event.target ).intersectionNumber ) );
					break;
				
				case PhaseControl.PHASE_REMOVE_SELECTED:
					_mapProxy.removePhaseSelection( PhaseControl( event.target ).controlUID );
					break;
				
				case PhaseControl.PHASE_UNSELECT:
					//-- if this is the only selected control, unselect it. if not, unselect everything else but this.
					if( _mapProxy.isTheOnlySelectedPhase( PhaseControl( event.target ).controlUID ) )
					{
						_mapProxy.unselectPhaseControls();
						
						//-- temporary
						sendNotification( DataSelectionProxy.NOTE_REQUEST_DATA_CHANGE );
					}
					else
					{
						_mapProxy.selectPhaseControl( PhaseControl( event.target ).controlUID );
						
						//-- temporary
						sendNotification( DataSelectionProxy.NOTE_REQUEST_DATA_CHANGE, new LodSelectionRequestVO( LevelOfDetail.PHASE, PhaseControl( event.target ).phaseNumber, PhaseControl( event.target ).intersectionNumber ) );
					}
					break;
				
				default:
					break;
			}
		}
		
		private function initializeNetwork():void
		{
			view.backgroundLinks 	= _networkProxy.backgroundNetworkPathData;
			view.detectorIndex 		= _networkProxy.detectorIndex;
			
			for each( var intersection:IntersectionProxy in _networkProxy.intersectionIndex.intersections )
			{
				for each( var phase:PhaseProxy in intersection.phaseIndex.phases )
				{
					addPhaseControl( phase.phaseControl );
					_mapProxy.registerPhaseControl( phase.uniqueID, phase.phaseControl );
				}
			}
		}
		
		private function updateNetwork():void
		{
			_networkProxy.updateInterval( _playbackProxy.currentInterval );
		}
	}
}