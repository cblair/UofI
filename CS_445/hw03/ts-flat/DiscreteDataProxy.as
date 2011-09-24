package com.ts.data.model
{
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.data.model.vo.StateVO;
	import com.ts.data.model.vo.StatesVO;
	
	import mx.states.State;

	public class DiscreteDataProxy extends DataProxy
	{
		[Bindable] private var currentState:StateVO;
		
		public var states:StatesVO;
		
		public function DiscreteDataProxy( metadata:MetadataVO, data:DataVO, States:StatesVO )
		{
			super( metadata, data );
			
			states = States;
		}
		
		public function getStateAt( index:int ):StateVO
		{ 
			return states.getStateByNum( getHashValueAt( index ) );
		}
		
		override public function setValueTo( index:int ):void
		{
			super.setValueTo( index );
			currentState = states.getStateByNum( currentValue );
		}
		
		public function getStateByName( stateName:String ):StateVO
		{
			return states.getStateByName( stateName );
		}
		
		/**
		 * Utility Function - converts the DataProxy into an output string. - includes state output.
		 * 
		 * @param asList Flag, if true, the string is rendered in column form, if false, it is rendered in row form.
		 * 
		 * @return a string representation of the DataProxy.
		 **/
		override public function toString( asList:Boolean = true ):String
		{
			var string:String = longName + "(" + dataName + ") Data Proxy:";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ", ";
			
			string += "\n";
			string += metadata.toString( asList );
			string += states.toString( asList );
			string += dataVO.toString( asList );
			
			return string;
		}
	}
}