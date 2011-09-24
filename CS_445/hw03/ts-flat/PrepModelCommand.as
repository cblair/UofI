package com.ts.core.controller
{
	import com.ts.data.model.DatabaseProxy;
	import com.ts.importing.RegisterImportCommand;
	import com.ts.importing.RemoveImportCommand;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	import com.ts.load.controller.RegisterLoadCommand;
	import com.ts.load.controller.RemoveLoadCommand;
	import com.ts.load.model.LoadProxy;
	import com.ts.presentation.controller.RegisterPresentationCommand;
	import com.ts.presentation.controller.RemovePresentationCommand;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.presentation.view.PresentationMediator;
	import com.ts.save.controller.RegisterSaveCommand;
	import com.ts.save.controller.RemoveSaveCommand;
	import com.ts.save.model.SaveProxy;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	import com.ts.presentation.controller.FinalizeApplicationCommand;
	
	//-- A simple command that prepares the model for use.
	//-- Executed BEFORE the viewPrep command ( before the view is prepared )
	public class PrepModelCommand extends SimpleCommand
	{
		public function PrepModelCommand()
		{
			super();
		}
		
		override public function execute( note:INotification ):void
		{
			facade.registerProxy( new DatabaseProxy() );
			facade.registerProxy( new SaveProxy() );
			facade.registerProxy( new LoadProxy() );
			
			facade.registerCommand( PresentationMediator.NOTE_ENTER_PRESENTATION_MODE, RegisterPresentationCommand );
			facade.registerCommand( PresentationMediator.NOTE_EXIT_PRESENTATION_MODE, RemovePresentationCommand );
			facade.registerCommand( LoadProxy.NOTE_ENTER_LOAD, RegisterLoadCommand );
			facade.registerCommand( LoadProxy.NOTE_EXIT_LOAD, RemoveLoadCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_ENTER_IMPORT_MODE, RegisterImportCommand );
			facade.registerCommand( ImportConfigProxy.NOTE_EXIT_IMPORT_MODE, RemoveImportCommand );
			facade.registerCommand( SaveProxy.NOTE_ENTER_SAVE, RegisterSaveCommand );
			facade.registerCommand( SaveProxy.NOTE_EXIT_SAVE, RemoveSaveCommand );
			
			facade.registerCommand( SystemProxy.NOTE_FINALIZE_APPLICATION, FinalizeApplicationCommand );
			
		}
	}
}