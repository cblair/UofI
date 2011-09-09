package com.ts.presentation.controller
{
	import com.ts.presentation.model.map.MapProxy;
	import com.ts.presentation.model.map.TrafficProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class UpdatePhaseSelectionCommand extends SimpleCommand
	{
		override public function execute( notification:INotification ):void
		{
			var selectedPhases:Array 	= notification.getBody() as Array;
			var sender:String			= notification.getType();
			
			if( sender == MapProxy.NAME )
			{
				var trafficProxy:TrafficProxy = facade.retrieveProxy( TrafficProxy.NAME ) as TrafficProxy;
				trafficProxy.selectedPhases = selectedPhases;
			}
		}
	}
}