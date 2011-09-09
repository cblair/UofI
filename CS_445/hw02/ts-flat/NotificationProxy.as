package com.ts.presentation.model.notification
{
	import com.ts.aspect.controller.DecorationInterpolator;
	import com.ts.data.model.DiscreteDataProxy;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class NotificationProxy extends Proxy
	{
		public static const NAME:String		= "notificationProxy";
		
		public var dataSource:DiscreteDataProxy;
		public var isState:Boolean;
		public var plotProxy:NotificationPlotProxy;
		
		public function NotificationProxy( DataSource:DiscreteDataProxy, IsState:Boolean, PlotProxy:NotificationPlotProxy )
		{
			super( NAME );
			dataSource = DataSource;
			isState = IsState;
			plotProxy = PlotProxy;
		}
	}
}