package com.ts.save.controller
{
	import com.ts.data.model.DatabaseProxy;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.save.model.SaveProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class RegisterSaveCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var database:DatabaseProxy = facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy;
			var system:SystemProxy = facade.retrieveProxy( SystemProxy.NAME ) as SystemProxy;
			var saveProxy:SaveProxy = facade.retrieveProxy( SaveProxy.NAME ) as SaveProxy;
			
			facade.registerCommand( SaveProxy.NOTE_CANCEL_SAVE, SaveCancelCommand );
			facade.registerCommand( SaveProxy.NOTE_SAVE_COMPLETE, SaveCompleteCommand );
			
			saveProxy.saveModule( database, system );
		}
	}
}