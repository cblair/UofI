package com.ts.importing.configuration.model.vo
{
	public class ExternalImportStatesVO
	{
		public var states:Array;
		public var numUniqueValues:int;
		
		private var _stateLookup:Array;
		private var _stateHash:Object;
		
		public function ExternalImportStatesVO( States:Array )
		{
			states = States;
			numUniqueValues = states.length;
			_stateLookup = new Array();
			_stateHash = new Object();
			for each( var state:ExternalImportStateVO in States )
			{
				_stateLookup[ int( state.stateNumber ) ] = state;
				_stateHash[ state.externalStateString ] = state;
			}
		}
		
		public function get externalStateStrings():Array
		{
			return _stateLookup;
		}
		
		public function getStateByNum( stateNum:int ):ExternalImportStateVO
		{
			return _stateLookup[ stateNum ];
		}
		
		public function getNameByNum( stateNum:int ):String
		{
			return _stateLookup[ stateNum ].stateName;
		}
		
		public function getExternalStringByNum( stateNum:int ):String
		{
			return _stateLookup[ stateNum ].externalStateString;
		}
		
		public function getStringByNum( stateNum:int ):String
		{
			return _stateLookup[ stateNum ].stateString;
		}
		
		public function getStateByExternalString( externalString:String ):ExternalImportStateVO
		{
			return _stateHash[ externalString ];
		}
		
		public function getNameByExternalString( externalString:String ):String
		{
			return _stateHash[ externalString ].stateName;
		}
		
		public function getStringByExternalString( externalString:String ):String
		{
			return _stateHash[ externalString ].stateString;
		}
		
		public function getNumByExternalString( externalString:String ):Number
		{
			var state:ExternalImportStateVO = _stateHash[ externalString ];
			return state ? state.stateNumber : NaN;
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "States:\n";
			for each( var state:ExternalImportStateVO in states )
			{
				string += state.toString( asList ) + "\n";
			}
			
			return string;
		}
	}
}