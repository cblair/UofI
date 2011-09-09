package com.ts.presentation.view.map
{
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.presentation.model.map.MapProxy;
	import com.ts.presentation.view.annotation.controls.ShapeControl;
	import com.ts.presentation.view.map.controls.VehicleControl;
	import com.ts.presentation.model.map.TrafficProxy;
	import com.ts.presentation.model.map.vehicle.VehicleProxy;
	
	import flash.events.Event;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.mediator.Mediator;
	
	public class TrafficMediator extends Mediator
	{
		public static const NAME:String	= "trafficMediator";
		
		private var _trafficProxy:TrafficProxy;
		private var _mapProxy:MapProxy;
		private var _playbackProxy:PlaybackProxy;
		
		public function get view():TrafficView
		{
			return viewComponent as TrafficView;
		}
		
		public function TrafficMediator( viewComponent:Object )
		{
			super( NAME, viewComponent );
		}

		override public function onRegister():void
		{
			_trafficProxy 	= facade.retrieveProxy( TrafficProxy.NAME ) as TrafficProxy;
			_mapProxy 		= facade.retrieveProxy( MapProxy.NAME ) as MapProxy;
			_playbackProxy	= facade.retrieveProxy( PlaybackProxy.NAME ) as PlaybackProxy;
			initializeTraffic();
			
			view.addEventListener( VehicleControl.VEH_OVER, 			handleEvent, true );
			view.addEventListener( VehicleControl.VEH_OUT, 				handleEvent, true );
			view.addEventListener( VehicleControl.VEH_SELECTED_OVER,	handleEvent, true );
			view.addEventListener( VehicleControl.VEH_SELECTED_OUT,		handleEvent, true );
			view.addEventListener( VehicleControl.VEH_SELECT, 			handleEvent, true );
			view.addEventListener( VehicleControl.VEH_ADD_SELECTED, 	handleEvent, true );
			view.addEventListener( VehicleControl.VEH_REMOVE_SELECTED, 	handleEvent, true );
			view.addEventListener( VehicleControl.VEH_UNSELECT, 		handleEvent, true );
			
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
			switch( event.type )
			{
				case PlaybackProxy.UPDATE_INTERVAL:
					updateTraffic();
					break;
				
				case VehicleControl.VEH_OVER:
					event.stopPropagation();
					_mapProxy.highlightVehicleControl( VehicleControl( event.target ).controlUID );
					break;
				
				case VehicleControl.VEH_OUT:
					event.stopPropagation();
					_mapProxy.unhighlightVehicleControl();
					break;
				
				case VehicleControl.VEH_SELECTED_OVER:
					event.stopPropagation();
					break;
				
				case VehicleControl.VEH_SELECTED_OUT:
					event.stopPropagation();
					break;
				
				case VehicleControl.VEH_SELECT:
					event.stopPropagation();
					_mapProxy.selectVehicleControl( VehicleControl( event.target ).controlUID );
					break;
				
				case VehicleControl.VEH_ADD_SELECTED:
					event.stopPropagation();
					_mapProxy.addVehicleSelection( VehicleControl( event.target ).controlUID );
					break;
				
				case VehicleControl.VEH_REMOVE_SELECTED:
					event.stopPropagation();
					_mapProxy.removeVehicleSelection( VehicleControl( event.target ).controlUID );
					break;
				
				case VehicleControl.VEH_UNSELECT:
					event.stopPropagation();
					//-- if this is the only selected control, unselect it. if not, unselect everything else but this.
					if( _mapProxy.isTheOnlySelectedVehicle( VehicleControl( event.target ).controlUID ) )
					{
						_mapProxy.unselectVehicleControls();
					}
					else
					{
						_mapProxy.selectVehicleControl( VehicleControl( event.target ).controlUID );
					}
					break;

				default:
					break;
			}
		}
		
		private function initializeTraffic():void
		{
			for each( var vehicle:VehicleProxy in _trafficProxy.vehicles )
			{
				view.addVehicle( vehicle.lodID, vehicle.vehicleControl );
				_mapProxy.registerVehicleControl( String( vehicle.lodID ), vehicle.vehicleControl );
			}
		}
		
		private function updateTraffic():void
		{
			_trafficProxy.updateInterval( _playbackProxy.currentInterval );
		}
	}
}