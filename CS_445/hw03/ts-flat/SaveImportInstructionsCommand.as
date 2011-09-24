package com.ts.importing.configuration.controller
{
	import com.fManagement.fileLoader.IFile;
	import com.fManagement.fileSaver.FileSaveEvent;
	import com.fManagement.fileSaver.FileSaver;
	import com.fManagement.fileTypes.TXTFile;
	import com.ts.importing.configuration.model.ImportConfigProxy;
	
	import flash.events.Event;
	import flash.sampler.NewObjectSample;
	
	import org.puremvc.as3.interfaces.INotification;
	import org.puremvc.as3.patterns.command.SimpleCommand;
	
	/**
	 * Saves a string of external import instructions to a .txt file
	 * <p>browses for a file to save the external import instructions</p>
	 */
	public class SaveImportInstructionsCommand extends SimpleCommand
	{
		private const STATUS_SAVE_COMPLETE:String		= "";
		private const STATUS_SAVE_FAILED:String			= "";
		
		[Bindable] public var saveStatus:String = "";
		[Bindable] public var saveTarget:String = "";
		
		override public function execute(notification:INotification):void
		{
			//-- SAVE THE INSTRUCTIONS TO A STRING
			var importConfig:ImportConfigProxy = facade.retrieveProxy( ImportConfigProxy.NAME ) as ImportConfigProxy;
			
			
			var fileSaver:FileSaver = new FileSaver();
			var saveFile:IFile = new TXTFile("", "", importConfig.exportInstructions );
			
			fileSaver.addEventListener( FileSaveEvent.FILE_SAVE_FAILURE, handleEvent );
			fileSaver.addEventListener( FileSaveEvent.FILE_SAVE_SUCCESS, handleEvent );
			fileSaver.saveFile( saveFile );
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case FileSaveEvent.FILE_SAVE_SUCCESS:
					event.target.removeEventListener( FileSaveEvent.FILE_SAVE_FAILURE, handleEvent );
					event.target.removeEventListener( FileSaveEvent.FILE_SAVE_SUCCESS, handleEvent );
					saveStatus = STATUS_SAVE_COMPLETE;
					break;
				
				case FileSaveEvent.FILE_SAVE_FAILURE:
					event.target.removeEventListener( FileSaveEvent.FILE_SAVE_FAILURE, handleEvent );
					event.target.removeEventListener( FileSaveEvent.FILE_SAVE_SUCCESS, handleEvent );
					saveStatus = STATUS_SAVE_FAILED;
					break;
				
				default:
					break;
			}
		}
	}
}