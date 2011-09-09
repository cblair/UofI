package com.ts.core.controller
{
	
	import com.ts.importing.configuration.controller.ImportCommand;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.load.controller.LoadCommand;
	import com.ts.load.model.LoadProxy;
	import com.ts.save.controller.SaveCommand;
	import com.ts.save.model.SaveProxy;
	
	import org.puremvc.as3.patterns.command.MacroCommand;
	
	public class RegisterCommands extends MacroCommand
	{
		public function RegisterCommands()
		{
			super();
		}
		
		override protected function initializeMacroCommand():void
		{
			facade.registerCommand( ImportConfigProxy.NOTE_IMPORT, ImportCommand );
			facade.registerCommand( SaveProxy.NOTE_SAVE, SaveCommand );
			facade.registerCommand( LoadProxy.NOTE_LOAD, LoadCommand );
		}
	}
}