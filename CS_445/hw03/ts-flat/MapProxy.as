package com.ts.presentation.model.map
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.presentation.view.map.controls.PhaseControl;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	import spark.components.ToggleButton;
	
	public class MapProxy extends Proxy
	{
		public static const NAME:String = "mapProxy";
		
		public static const NOTE_HIGHLIGHT_VEHICLE:String			= NAME + "/note/highlightVehicle";
		public static const NOTE_UNHIGHLIGHT_VEHICLE:String			= NAME + "/note/unhighlightVehicle";
		public static const NOTE_UPDATE_VEHICLE_SELECTION:String	= NAME + "/note/updateVehicleSelection";
		
		public static const NOTE_HIGHLIGHT_PHASE:String			= NAME + "/note/highlightPhase";
		public static const NOTE_UNHIGHLIGHT_PHASE:String		= NAME + "/note/unhighlightPhase";
		public static const NOTE_UPDATE_PHASE_SELECTION:String	= NAME + "/note/updatePhaseSelection";
		
		public var currentLOD:String;
		public var currentIntersection:int;
		public var currentLodID:String;
		
		public var currentPhaseControl:PhaseControl;
		
		private var _vehicleMap:Object;
		private var _vehicleUIDs:Array;
		private var _selectedVehicles:Array;
		private var _highlightedVehicle:String;
		
		private var _phaseMap:Object;
		private var _phaseUIDs:Array;
		private var _selectedPhases:Array;
		private var _highlightedPhase:String;
		
		public function MapProxy()
		{
			super( NAME );
			
			_vehicleMap 		= new Object();
			_vehicleUIDs		= new Array();
			_selectedVehicles 	= new Array();
			
			_phaseMap 			= new Object();
			_phaseUIDs			= new Array();
			_selectedPhases 	= new Array();
		}
		
		public function isTheOnlySelectedVehicle( UID:String ):Boolean
		{
			return _selectedVehicles.indexOf( UID ) != -1 && _selectedVehicles.length == 1;
		}
		
		public function isTheOnlySelectedPhase( UID:String ):Boolean
		{
			return _selectedPhases.indexOf( UID ) != -1 && _selectedPhases.length == 1;
		}
		
		public function registerVehicleControl( UID:String, control:ToggleButton ):void
		{
			if( _vehicleUIDs.indexOf( UID ) == -1 )
			{
				_vehicleMap[ UID ] = control;
				_vehicleUIDs.push( UID );
			}
		}
		
		public function registerPhaseControl( UID:String, control:ToggleButton ):void
		{
			if( _phaseUIDs.indexOf( UID ) == -1 )
			{
				_phaseMap[ UID ] = control;
				_phaseUIDs.push( UID );
			}
		}
		
		public function selectVehicleControl( UID:String ):void
		{
			if( _vehicleUIDs.indexOf( UID ) != -1 )
			{
				for each( var SelectedUID:String in _selectedVehicles )
				{
					if( UID != SelectedUID )
						_vehicleMap[ SelectedUID ].selected = false;
				}
				_selectedVehicles = [ UID ];
				_vehicleMap[ UID ].selected = true;
				updateVehicles();
			}
		}
		
		public function selectPhaseControl( UID:String ):void
		{
			if( _phaseUIDs.indexOf( UID ) != -1 )
			{
				for each( var SelectedUID:String in _selectedPhases )
				{
					if( UID != SelectedUID )
						_phaseMap[ SelectedUID ].selected = false;
				}
				_selectedPhases = [ UID ];
				_phaseMap[ UID ].selected = true;
				updatePhases();
			}
		}
		
		public function addVehicleSelection( UID:String ):void
		{
			var index:int = _selectedVehicles.indexOf( UID );
			if( index == -1 )
			{
				_selectedVehicles.push( UID );
				_vehicleMap[ UID ].selected = true;
				updateVehicles();
			}
		}
		
		public function addPhaseSelection( UID:String ):void
		{
			var index:int = _selectedPhases.indexOf( UID );
			if( index == -1 )
			{
				_selectedPhases.push( UID );
				_phaseMap[ UID ].selected = true;
				updatePhases();
			}
		}
		
		public function removeVehicleSelection( UID:String ):void
		{
			var index:int = _selectedVehicles.indexOf( UID );
			if( index != -1 )
			{
				_selectedVehicles.splice( index, 1 );
				_vehicleMap[ UID ].selected = false;
				updateVehicles();
			}
		}
		
		public function removePhaseSelection( UID:String ):void
		{
			var index:int = _selectedPhases.indexOf( UID );
			if( index != -1 )
			{
				_selectedPhases.splice( index, 1 );
				_phaseMap[ UID ].selected = false;
				updatePhases();
			}
		}
		
		public function unselectVehicleControls():void
		{
			for each( var SelectedUID:String in _selectedVehicles )
			{
				_vehicleMap[ SelectedUID ].selected = false;
			}
			_selectedVehicles = [];
			updateVehicles();
		}
		
		public function unselectPhaseControls():void
		{
			for each( var SelectedUID:String in _selectedPhases )
			{
				_phaseMap[ SelectedUID ].selected = false;
			}
			_selectedPhases = [];
			updatePhases();
		}
		
		public function highlightVehicleControl( UID:String ):void
		{
			if( _vehicleUIDs.indexOf( UID ) != -1 )
			{
				_highlightedVehicle = UID;
				sendNotification( NOTE_HIGHLIGHT_VEHICLE, _highlightedVehicle, NAME );
			}
		}
		
		public function highlightPhaseControl( UID:String ):void
		{
			if( _phaseUIDs.indexOf( UID ) != -1 )
			{
				_highlightedPhase = UID;
				sendNotification( NOTE_HIGHLIGHT_PHASE, _highlightedPhase, NAME );
			}
		}
		
		public function unhighlightVehicleControl():void
		{
			if( _highlightedVehicle )
			{
				sendNotification( NOTE_UNHIGHLIGHT_VEHICLE, _highlightedVehicle, NAME );
				_highlightedVehicle = null;
			}
		}
		
		public function unhighlightPhaseControl():void
		{
			if( _highlightedPhase )
			{
				sendNotification( NOTE_UNHIGHLIGHT_PHASE, _highlightedPhase, NAME );
				_highlightedPhase = null;
			}
		}
		
		private function updateVehicles():void
		{
			sendNotification( NOTE_UPDATE_VEHICLE_SELECTION, _selectedVehicles, NAME );
		}
		
		private function updatePhases():void
		{
			sendNotification( NOTE_UPDATE_PHASE_SELECTION, _selectedPhases, NAME );
		}
	}
}