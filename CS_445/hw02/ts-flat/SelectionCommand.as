package com.ts.presentation.controller
{
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class SelectionCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var selectedIDs:Array = notification.getBody() as Array;
			var type:String = notification.getType();
		}
	}
}