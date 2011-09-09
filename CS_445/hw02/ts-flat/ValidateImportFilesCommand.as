package com.ts.importing.configuration.controller
{
	import com.ts.importing.parsing.model.ImportParsingProxy;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	/**
	 * validates the import file directory to make sure all required asset files are present
	 * <p>( checks each file's extension against a list of required extensions )</p>
	 * <p> if invalid, ( file extensions are missing ), returns the user to import configuration mode, passing a list of invalid extensions.</p>
	 * <p> if valid, starts the parsing command, sending an ImportConfigurationVO within the body of the command.</p>
	 **/
	public class ValidateImportFilesCommand extends SimpleCommand
	{
		override public function execute( notification:INotification ):void
		{
			var importConfig:ImportConfigProxy = facade.retrieveProxy( ImportConfigProxy.NAME ) as ImportConfigProxy;
			if( importConfig.validImportFiles() )
			{
				sendNotification( ImportConfigProxy.NOTE_IMPORT_FILES_VALID, importConfig.importConfigurationVO );
			}
			else
			{
				sendNotification( ImportConfigProxy.NOTE_IMPORT_FILES_INVALID, importConfig.missingFileExtensions );
			}
		}
	}
}