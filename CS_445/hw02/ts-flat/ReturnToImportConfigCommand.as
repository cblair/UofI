package com.ts.importing.configuration.controller
{
	import com.ts.importing.configuration.model.ImportConfigProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	/**
	 * Returns the user to the import configuration mode
	 */
	public class ReturnToImportConfigCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			facade.removeCommand( ImportConfigProxy.NOTE_IMPORT_BROWSE_COMPLETE );
			facade.removeCommand( ImportConfigProxy.NOTE_IMPORT_BROWSE_CANCELLED );
			facade.registerCommand( ImportConfigProxy.NOTE_BROWSE_FOR_IMPORT, BrowseForImportCommand );
		}
	}
}