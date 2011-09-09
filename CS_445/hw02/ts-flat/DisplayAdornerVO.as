package com.ts.aspect
{
	import mx.collections.ArrayCollection;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class DisplayAdornerVO
	{
		public var hGraphData:ArrayCollection;
		public var notificationData:Array;
		public var host:Proxy;
		
		public function DisplayAdornerVO( HGraphData:ArrayCollection, NotificationData:Array, Host:Proxy )
		{
			hGraphData 			= HGraphData;
			notificationData 	= NotificationData;
			host 				= Host;
		}
	}
}