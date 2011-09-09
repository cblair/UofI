package com.ts.load.controller
{
	import com.ts.data.model.DatabaseProxy;
	import com.ts.load.model.LoadProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class RegisterLoadCommand extends SimpleCommand
	{	
		override public function execute(notification:INotification):void
		{
			facade.registerCommand( LoadProxy.NOTE_CANCEL_LOAD, LoadCanceledCommand );
			facade.registerCommand( LoadProxy.NOTE_LOAD_COMPLETE, LoadCompleteCommand );
			var loadProxy:LoadProxy = facade.retrieveProxy( LoadProxy.NAME ) as LoadProxy;
			loadProxy.loadModule();
		}
	}
}