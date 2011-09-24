package com.ts.importing.configuration.controller
{
	import com.ts.importing.configuration.model.ImportConfigProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import org.puremvc.as3.utilities.statemachine.StateMachine;
	
	public class ImportCancelCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			sendNotification( StateMachine.ACTION, null, ImportConfigProxy.ACTION_IMPORT_CANCELLED );
		}
	}
}