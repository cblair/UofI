package com.ts.data.model
{
	import org.puremvc.as3.patterns.proxy.Proxy;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class DatabaseProxy extends Proxy
	{
		public static const NAME:String					= "DatabaseProxy";
		public static const NOTE_MODEL_READY:String		= NAME + "/note/modelReady";
		
		public var dataIndex:DataIndex
		
		public function DatabaseProxy()
		{
			super( NAME );
			dataIndex = new DataIndex([]);
		}
		
		public function indexDataProxy( dataProxy:DataProxy ):void
		{
			dataIndex.indexDataProxy( dataProxy );
		}
		
		public function get dataProxies():Array{ return dataIndex.dataProxies; } 
	}
}