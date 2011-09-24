package com.ts.load.controller
{
	import com.ts.load.model.LoadProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class RemoveLoadCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			facade.removeCommand( LoadProxy.NOTE_CANCEL_LOAD );
			facade.removeCommand( LoadProxy.NOTE_LOAD_COMPLETE );
		}
	}
}