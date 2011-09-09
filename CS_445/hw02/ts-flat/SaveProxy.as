package com.ts.save.model
{
	import com.fManagement.fileLoader.IFile;
	import com.fManagement.fileSaver.FileSaveEvent;
	import com.fManagement.fileSaver.FileSaver;
	import com.fManagement.fileTypes.XMLFile;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.model.DatabaseProxy;
	import com.ts.data.model.DataProxy;
	import com.ts.load.xml.SystemElement;
	import com.ts.presentation.model.map.SystemProxy;
	import com.ts.save.controller.SavePackers;
	import com.ts.save.controller.SystemPackers;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class SaveProxy extends Proxy
	{
		public static const NAME:String					= "SaveProxy";
		
		public static const STATE_SAVING:String			= NAME + "state/saving/";
		
		public static const ACTION_SAVE:String			= NAME + "action/save";
		public static const ACTION_CANCEL_SAVE:String	= NAME + "action/cancelSave";
		public static const ACTION_SAVE_COMPLETE:String = NAME + "action/saveComplete";
		
		public static const NOTE_SAVE:String			= NAME + "/note/save";
		public static const NOTE_CANCEL_SAVE:String		= NAME + "/note/cancelSave";
		public static const NOTE_SAVE_COMPLETE:String	= NAME + "/note/saveComplete";
		public static const NOTE_ENTER_SAVE:String		= NAME + "/note/enterSave";
		public static const NOTE_EXIT_SAVE:String		= NAME + "/note/exitSave";
		
		//public static const NOTE_SAVE_FAILED:String		= NAME + "/note/saveFailed";
		
		private const STATUS_PACKING_MODEL:String		= "Packing Model";
		private const STATUS_PACKING_DISPLAY:String		= "Packing Display";
		private const STATUS_SAVING_MODULE:String		= "Saving Module";
		private const STATUS_SAVE_COMPLETE:String		= "";
		private const STATUS_SAVE_FAILED:String			= "";
		private const STATUS_SAVE_CANCELED:String		= "";
		
		[Bindable] public var saveStatus:String = "";
		[Bindable] public var saveTarget:String = "";
		
		public function SaveProxy()
		{
			super( NAME );
		}
		
		public function saveModule( database:DatabaseProxy, system:SystemProxy ):void
		{
			var fileSaver:FileSaver = new FileSaver();

			var saveFile:IFile = new XMLFile( "", "", packModule( database, system ) );
			
			fileSaver.addEventListener( FileSaveEvent.FILE_SAVE_FAILURE, handleEvent );
			fileSaver.addEventListener( FileSaveEvent.FILE_SAVE_SUCCESS, handleEvent );
			fileSaver.addEventListener( FileSaveEvent.FILE_SAVE_CANCELED, handleEvent );
			fileSaver.saveFile( saveFile );
			
			saveStatus = STATUS_SAVING_MODULE;
		}
		
		private function packModule( database:DatabaseProxy, system:SystemProxy ):XML
		{
			var saveXML:XML = new XML("<application></application>");
			
			saveXML = packSystem( system, saveXML );
			saveXML = packModel( database, saveXML );
			
			saveTarget = "";
			
			return saveXML;
		}
		
		private function packSystem( system:SystemProxy, saveXML:XML ):XML
		{
			saveStatus = STATUS_PACKING_DISPLAY;
			var stateWrapper:XML = new XML( "<" + SystemElement.SYSTEM + "/>" );
			
			stateWrapper = SystemPackers.packSystem( system, stateWrapper );
			
			saveXML.appendChild( stateWrapper );
			
			return saveXML;
		}
		
		private function packModel( database:DatabaseProxy, saveXML:XML ):XML
		{
			saveStatus = STATUS_PACKING_MODEL;
			
			var modelWrapper:XML = new XML( "<" + XMLElement.MODEL + "/>" );
			var modelXMLList:XMLList = new XMLList();
			
			for each( var dataProxy:DataProxy in database.dataProxies )
			{
				saveTarget = dataProxy.dataName;
				modelXMLList += SavePackers.packDataProxy( dataProxy );
			}
			
			modelWrapper.appendChild( modelXMLList );
			
			saveXML.appendChild( modelWrapper );
			
			return saveXML;
		}
		
		private function handleEvent( event:Event ):void
		{
			event.target.removeEventListener( FileSaveEvent.FILE_SAVE_FAILURE, handleEvent );
			event.target.removeEventListener( FileSaveEvent.FILE_SAVE_SUCCESS, handleEvent );
			event.target.removeEventListener( FileSaveEvent.FILE_SAVE_CANCELED, handleEvent );
			switch( event.type )
			{
				case FileSaveEvent.FILE_SAVE_SUCCESS:
					saveStatus = STATUS_SAVE_COMPLETE;
					sendNotification( NOTE_SAVE_COMPLETE );
					break;
				
				case FileSaveEvent.FILE_SAVE_FAILURE:
					saveStatus = STATUS_SAVE_FAILED;
					sendNotification( NOTE_CANCEL_SAVE );
					break;
				
				case FileSaveEvent.FILE_SAVE_CANCELED:
					saveStatus = STATUS_SAVE_CANCELED;
					sendNotification( NOTE_CANCEL_SAVE );
					break;
				
				default:
					break;
			}
		}
	}
}