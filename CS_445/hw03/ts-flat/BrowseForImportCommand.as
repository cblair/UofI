package com.ts.importing.configuration.controller
{
	import com.ts.importing.configuration.model.ImportConfigProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	/**
	 * Opens up a user dialog to browse for an import folder
	 * <p>If a folder is selected, the files are sent to be validated</p>
	 */
	public class BrowseForImportCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			var importPrepProxy:ImportConfigProxy = facade.retrieveProxy( ImportConfigProxy.NAME ) as ImportConfigProxy;
			importPrepProxy.browseForFileInFolder();
		}
	}
}