package com.ts.importing
{
	import com.ts.importing.configuration.controller.BrowseForImportCommand;
	import com.ts.importing.configuration.controller.ConfigureImportCommand;
	import com.ts.importing.configuration.controller.FormatImportDataCommand;
	import com.ts.importing.configuration.controller.ImportCancelCommand;
	import com.ts.importing.configuration.controller.ReturnToImportConfigCommand;
	import com.ts.importing.configuration.controller.SaveImportInstructionsCommand;
	import com.ts.importing.configuration.controller.ValidateImportFilesCommand;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.importing.parsing.controller.ParseImportCommand;
	import com.ts.importing.parsing.model.ImportParsingProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	/**
	 * Registers all the commands needed for import mode.
	 * <p>Called when Import Mode is entered</p>
	 */
	public class RegisterImportCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			facade.registerCommand( ImportConfigProxy.NOTE_BROWSE_FOR_IMPORT, BrowseForImportCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_IMPORT_BROWSE_COMPLETE, ValidateImportFilesCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_IMPORT_BROWSE_CANCELLED, ReturnToImportConfigCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_CONFIGURE_IMPORT, ConfigureImportCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_SAVE_IMPORT_INSTRUCTIONS, SaveImportInstructionsCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_IMPORT_FILES_VALID, ParseImportCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_CANCEL_IMPORT, ImportCancelCommand );
			
			sendNotification( ImportConfigProxy.NOTE_CONFIGURE_IMPORT );
		}
	}
}