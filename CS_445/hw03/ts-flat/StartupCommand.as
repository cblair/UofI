package com.ts.core.controller
{
	import org.puremvc.as3.patterns.command.MacroCommand;
	
	public class StartupCommand extends MacroCommand
	{		
		public function StartupCommand()
		{
			super();
		}
		
		override protected function initializeMacroCommand():void
		{
			addSubCommand( PrepModelCommand );
			addSubCommand( PrepViewCommand );
			addSubCommand( RegisterCommands );
			addSubCommand( InjectFSMCommand );
		}
	}
}