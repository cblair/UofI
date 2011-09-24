package com.ts.importing.configuration.controller
{
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class FormatImportDataCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var data:Array = notification.getBody() as Array;
		}
	}
}