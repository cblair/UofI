package com.ts.presentation.model.map.phase
{
	public class SignalIndex
	{
		public var signals:Array;
		public var groupNum:int;
		public var intersectionNum:int;
		
		public function SignalIndex( Signals:Array )
		{
			if( Signals )
			{
				if( Signals.length > 0 )
				{
					signals = Signals;
					groupNum = SignalProxy( signals[ 0 ] ).groupNum;
					intersectionNum = SignalProxy( signals[ 0 ] ).intersectionNum;
				}
				else
				{
					signals = [];
					groupNum = -1;
					intersectionNum = -1;
				}
			}
			else
			{
				signals = [];
				groupNum = -1;
				intersectionNum = -1;
			}
		}
	}
}