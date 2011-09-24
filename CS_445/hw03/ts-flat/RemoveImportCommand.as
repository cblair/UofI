package com.ts.importing
{
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.importing.parsing.model.ImportParsingProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	/**
	 * Removes all the commands needed for import mode.
	 * <p>Called when Import Mode is exited</p>
	 */
	public class RemoveImportCommand extends SimpleCommand
	{
		override public function execute(notification:INotification):void
		{
			facade.removeCommand( ImportConfigProxy.NOTE_BROWSE_FOR_IMPORT );
			facade.removeCommand( ImportConfigProxy.NOTE_IMPORT_BROWSE_COMPLETE );
			facade.removeCommand( ImportConfigProxy.NOTE_IMPORT_BROWSE_CANCELLED );
			facade.removeCommand( ImportConfigProxy.NOTE_CONFIGURE_IMPORT );
			facade.removeCommand( ImportConfigProxy.NOTE_SAVE_IMPORT_INSTRUCTIONS );
			facade.removeCommand( ImportConfigProxy.NOTE_IMPORT_FILES_VALID );
			facade.removeCommand( ImportConfigProxy.NOTE_CANCEL_IMPORT );
		}
	}
}