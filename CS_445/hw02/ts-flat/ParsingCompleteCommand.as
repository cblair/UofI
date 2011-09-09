package com.ts.importing.parsing.controller
{
	import com.ts.importing.parsing.model.ImportParsingProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	public class ParsingCompleteCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var parseProxy:ImportParsingProxy = facade.retrieveProxy( ImportParsingProxy.NAME ) as ImportParsingProxy;
			
			//** WAIT TO UPDATE DISPLAY **//
			sendNotification( ApplicationFacade.NOTE_WAIT_FOR_DISPLAY, parseProxy.parseNextImport );
		}
	}
}