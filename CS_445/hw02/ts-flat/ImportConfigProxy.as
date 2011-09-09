package com.ts.importing.configuration.model
{
	
	import com.ts.importing.aspectClasses.AbstractAspectClass;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;
	import com.ts.importing.configuration.model.vo.ImportConfigVO;
	
	import flash.events.Event;
	import flash.filesystem.File;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	import com.ts.importing.aspectClasses.AspectFactory;

	public class ImportConfigProxy extends Proxy
	{
		
		public static const NAME:String = "ImportConfigurationProxy";
		
		/**
		 * Notification - Configure Import - requests the application to set up the configuration model
		 */
		public static const NOTE_CONFIGURE_IMPORT:String			= NAME + "/note/configureImport";
		
		
		public static const NOTE_IMPORT:String						= NAME + "/note/import";
		public static const NOTE_CANCEL_IMPORT:String				= NAME + "/note/cancelImport";
		/**
		 * Notification - Import Config Ready - alerts the application that configuration may commence
		 */
		public static const NOTE_IMPORT_CONFIG_READY:String			= NAME + "/note/importConfigurationReady";
		
		/**
		 * Notification - Browse For Import - requests the application to browse for the import assets folder
		 */
		public static const NOTE_BROWSE_FOR_IMPORT:String			= NAME + "/note/browseForImport";
		
		/**
		 * Notification - Import Browse Complete - alerts the application that the import browse is complete
		 */
		public static const NOTE_IMPORT_BROWSE_COMPLETE:String		= NAME + "/note/importBrowseComplete";
		
		/**
		 * Notification - Import Browse Complete - alerts the application that the import browse was cancelled
		 */
		public static const NOTE_IMPORT_BROWSE_CANCELLED:String		= NAME + "/note/importBrowseCancelled";
		
		/**
		 * Notification - Save Import Instructions - requests the application saves the import instructions to a file
		 */
		public static const NOTE_SAVE_IMPORT_INSTRUCTIONS:String	= NAME + "/note/saveImportInstructions";
		
		/**
		 * Notification - Import Files Valid - Alerts the application that the import files are valid ( all required ext are present ).
		 */
		public static const NOTE_IMPORT_FILES_VALID:String			= NAME + "/note/importFilesValid";
		
		/**
		 * Notification - Import Files Invalid - Alerts the application that the import files are not valid ( all required ext are not present ).
		 */
		public static const NOTE_IMPORT_FILES_INVALID:String		= NAME + "/note/importFilesInvalid";
		
		/**
		 * State - Importing Module - The import process, where the user imports all the external assets needed to build the model for the application
		 **/
		public static const STATE_IMPORTING_MODULE:String		= NAME + "/state/importingModule";
		
		public static const STATE_APP_READY_IMPORTING:String	= NAME + "/state/appReadyImporting";
		
		/**
		 * Action - Import - Changes the state of the application to the "Importing Module" state.
		 **/
		public static const ACTION_IMPORT:String				= NAME + "/action/import";
		public static const ACTION_IMPORT_ERRORED:String		= NAME + "/action/importErrored";
		public static const ACTION_IMPORT_COMPLETED:String		= NAME + "/action/importCompleted";
		public static const ACTION_IMPORT_CANCELLED:String		= NAME + "/action/importCancelled";
		public static const ACTION_UNIMPORT:String				= NAME + "/action/unimport";
		
		public static const NOTE_ENTER_IMPORT_MODE:String		= NAME + "/note/enterImportMode";
		public static const NOTE_EXIT_IMPORT_MODE:String		= NAME + "/note/exitImportMode";
		
		[Bindable] public var externalImportProxies:Array;
		public var aspectClass:AbstractAspectClass;
		private var _externalFileExts:Array;
		private var _missingFileExtensions:Array;
		
		private var _exportInstructions:String;
		
		//Browse vars
		private var _extension:String;
		private var _importFilePathLookup:Object;
		private var _externalXMLProxy:ExternalImportXMLProxy;
		
		/**
		 * returns an importConfigurationVO, containing the current importConfiguration data
		 **/
		public function get importConfigurationVO():ImportConfigVO
		{
			return new ImportConfigVO( externalImportProxies, _externalFileExts, _importFilePathLookup );
		}
		
		public function getImportProxiesByExt( extension:String ):Array 
		{
			var importProxies:Array = new Array();
			for each( var importProxy:ExternalImportProxy in externalImportProxies )
			{
				if( importProxy.externalFileExtension == extension )
				{
					importProxies.push( importProxy );
				}
			}
			return importProxies;
		}
		
		/**
		 * returns an array of missing file extensions if there are any.
		 * <p> returns null if all file exts. are present, or if called before an asset folder has been selected.</p>
		 */
		public function get missingFileExtensions():Array
		{
			return _missingFileExtensions;
		}
		
		public function ImportConfigProxy( externalXMLProxy:ExternalImportXMLProxy )
		{
			super( NAME );
			_externalXMLProxy = externalXMLProxy;
			setupImport( [] );
		}
		
		public function selectAspect( aspectName:String ):Array
		{
			aspectClass = AspectFactory.getAspectClass( aspectName );
			setupImport( aspectClass.derivationRequirements );
			return externalImportProxies;
		}
		
		private function setupImport( prereqList:Array ):void
		{
			var importList:Array = RequiredImportDataName.REQUIRED_IMPORT_NAMES.concat( prereqList );
			externalImportProxies = new Array();
			_externalFileExts = new Array();
			
			for each( var dataName:String in importList )
			{
				var tempXML:ExternalImportProxy = _externalXMLProxy.getImportProxyByName( dataName );
				var tempFileExt:String = _externalXMLProxy.getfileExtFromDataName( dataName );
				
				if( tempXML )
				{
					externalImportProxies.push( tempXML );
				}
				
				if( tempFileExt )
				{
					_externalFileExts.push( tempFileExt );
				}
			}
			_externalFileExts.map( removeDuplicate );
			externalImportProxies.sortOn( "externalFileExtension", Array.CASEINSENSITIVE );
		}
		
		public function openImport():void
		{
			for each( var importProxy:ExternalImportProxy in externalImportProxies )
			{
				facade.registerProxy( importProxy );
			}
		}
		
		private function closeImport():void
		{
			for each( var importProxy:ExternalImportProxy in externalImportProxies )
			{
				facade.removeProxy( importProxy.compoundName );
			}
		}
		
		private function removeDuplicate(entry:String, i:int, array:Array):void
		{
			while(array.indexOf(entry) != array.lastIndexOf(entry))
			{
				array.splice(array.lastIndexOf(entry), 1);
			}
		}
		
		/**
		 * Returns a richly formatted string of external import instructions reguarding details on where to expert all the required assets
		 */
		public function get exportInstructions():String
		{
			_exportInstructions = null;
			var tempStringArray:Array = new Array();
			
			if( externalImportProxies )
			{
				tempStringArray.push( "File Export List: \r\n" );
				tempStringArray.push( "Instructions: \r\n" );
				tempStringArray.push( "To export assets, open external and select Evaluation from the menu bar. \r\n\r\n" );
				
				var currentExt:String;
				for each( var externalData:ExternalImportProxy in externalImportProxies )
				{
					if( currentExt != externalData.externalFileExtension )
					{
						currentExt = externalData.externalFileExtension;
						tempStringArray.push( "\r\nexternal File Extension: ." + externalData.externalFileExtension + "\r\n" );
						tempStringArray.push( "external File Location: " + externalData.externalFileLocation + "\r\n" );
						tempStringArray.push( "external File Parameters: \r\n" );
					}
					tempStringArray.push( "\tParameter Name: " + externalData.externalParamName + "\r\n" );
				}
			}
			
			for(var i:int = 0; i < tempStringArray.length; i++)
			{
				if( _exportInstructions )
				{
					_exportInstructions = _exportInstructions + tempStringArray[i];
				}
				else
				{
					_exportInstructions = tempStringArray[i];
				}
			}
			
			return _exportInstructions;
		}
		
		/**
		 * Browse for folder containing external export data
		 * <p> Upon Completion, Extracts all external path strings from the folders required by the application, and validates their extensions </p>
		 * <p> Upon Cancel, returns to the Import Configuration mode </p>
		 */
		public function browseForFileInFolder():void
		{
			var file:File = File.desktopDirectory;
			file.browseForDirectory( "Select external Output Folder" );
			file.addEventListener( Event.SELECT, handleEvent );
			file.addEventListener( Event.CANCEL, handleEvent );
		}
		
		/**
		 * Validates the extracted asset external files for all the required file types.
		 * <p>Returns false if some are missing. A list of missing extensions can be accessed at "missingFileExtensions" </p>
		 */
		public function validImportFiles():Boolean
		{
			if( _importFilePathLookup )
			{
				var isValid:Boolean = true;
				_missingFileExtensions = new Array();
				for each( var extension:String in _externalFileExts )
				{
					if( !_importFilePathLookup[ extension ] )
					{
						_missingFileExtensions.push( extension );
						isValid = false;
					}
				}
				return isValid;
			}
			return false;
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case Event.SELECT:
					event.target.removeEventListener( Event.SELECT, handleEvent );
					event.target.removeEventListener( Event.CANCEL, handleEvent );
					getFilePathsFromDirectory( event.target as File );
					sendNotification( NOTE_IMPORT_BROWSE_COMPLETE, importConfigurationVO );
					break;
				
				case Event.CANCEL:
					event.target.removeEventListener( Event.SELECT, handleEvent );
					event.target.removeEventListener( Event.CANCEL, handleEvent );
					sendNotification( NOTE_IMPORT_BROWSE_CANCELLED );
					break;
				
				default:	break;	
			}
		}
		
		private function getFilePathsFromDirectory( directory:File ):void
		{
			_importFilePathLookup = new Object();
			var files:Array = directory.getDirectoryListing();
			for each( var extension:String in _externalFileExts )
			{
				for each( var file:File in files )
				{
					if( file.extension.toLowerCase() == extension.toLowerCase() )
					{
						if( !_importFilePathLookup[ extension ] )
						{
							_importFilePathLookup[ extension ] = new Array();
						}
						_importFilePathLookup[ extension ].push( file.nativePath );
					}
				}
			}
		}
	}
}