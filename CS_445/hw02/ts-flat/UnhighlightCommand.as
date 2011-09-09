package com.ts.presentation.controller
{
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class UnhighlightCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var highlighedID:String = notification.getBody();
			var type:String = notification.getType();
		}
	}
}