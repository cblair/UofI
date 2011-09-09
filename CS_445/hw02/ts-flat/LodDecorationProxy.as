package com.ts.aspect.model
{
	import com.ts.aspect.development.HistoryGraphProxy;
	
	import mx.collections.ArrayCollection;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class LodDecorationProxy
	{
		public var lodID:int;
		public var levelOfDetail:String
		public var historyGraphProxies:Array;
		public var notificationProxies:Array;
		public var host:Proxy
		
		public function LodDecorationProxy( LodID:int, LevelOfDetail:String, HistoryGraphProxies:Array, NotificationProxies:Array, Host:Proxy )
		{
			lodID 				= LodID;
			levelOfDetail		= LevelOfDetail;
			historyGraphProxies	= HistoryGraphProxies;
			notificationProxies = NotificationProxies;
			host				= Host;
		}
		
		public function get historyGraphDataProviders():ArrayCollection
		{
			return new ArrayCollection( historyGraphProxies );
		}
		
		public function mergeDecorationProxy( decorationProxy:LodDecorationProxy ):void
		{
			historyGraphProxies = historyGraphProxies.concat( decorationProxy.historyGraphProxies );
			notificationProxies = notificationProxies.concat( decorationProxy.notificationProxies );
		}
	}
}