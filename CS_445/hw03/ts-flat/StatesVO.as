package com.ts.data.model.vo
{
	public class StatesVO
	{
		private var _stateLookup:Array;
		private var _stateHash:Object;
		private var _nullState:NullStateVO;
		
		public var states:Array;
		
		public function StatesVO( States:Array )
		{
			states = States;
			_stateLookup = new Array();
			_stateHash = new Object();
			_nullState = new NullStateVO();
			
			for each( var state:StateVO in States )
			{
				_stateLookup[ int( state.stateNumber ) ] = state;
				_stateHash[ state.stateName ] = state;
			}
		}
		
		public function get stateNames():Array{ return _stateLookup; }
		
		public function getStateByNum( stateNum:Number ):StateVO
		{
			return isNaN( stateNum ) ? _nullState : _stateLookup[ stateNum ];
		}
		
		public function getNameByNum( stateNum:Number ):String
		{
			return isNaN( stateNum ) ? _nullState.stateName : _stateLookup[ stateNum ].stateName;
		}
		
		public function getStringByNum( stateNum:Number ):String
		{
			return isNaN( stateNum ) ? _nullState.stateString : _stateLookup[ stateNum ].stateString;
		}
		
		public function getStateByName( stateName:String ):StateVO
		{
			return _stateHash[ stateName ];
		}
		
		public function getNumByName( stateName:String ):Number
		{
			return _stateHash[ stateName ].stateNumber;
		}
		
		public function getStringByName( stateName:String ):String
		{
			return _stateHash[ stateName ].stateString;
		}
		
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "States:\n";
			for each( var state:StateVO in states )
			{
				string += state.toString( asList );
			}
			
			return string;
		}
	}
}