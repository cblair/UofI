package com.ts.save.controller
{
	import com.ts.save.model.SaveProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class RemoveSaveCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			facade.removeCommand( SaveProxy.NOTE_CANCEL_SAVE );
			facade.removeCommand( SaveProxy.NOTE_SAVE_COMPLETE );
		}
	}
}