package com.ts.importing.configuration.model
{
	import com.fManagement.fileLoader.FileEvent;
	import com.fManagement.fileLoader.FileLoader;
	import com.fManagement.fileLoader.IFile;
	import com.fManagement.fileTypes.FileTypes;
	import com.ts.startup.EntityProxy;
	
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	
	import org.puremvc.as3.utilities.startupmanager.interfaces.IStartupProxy;

	public class ExternalImportXMLProxy extends EntityProxy implements IStartupProxy
	{
		public static const NAME:String 						= "sourceXMLProxy";
		public static const SRNAME:String						= "sourceSRProxy";
		
		public static const NOTE_SOURCE_XML_LOADING:String		= NAME + "/note/sourceXMLLoading";
		public static const NOTE_SOURCE_XML_LOADED:String		= NAME + "/note/sourceXMLLoaded";
		public static const NOTE_SOURCE_XML_FAILED:String		= NAME + "/note/sourceXMLFailed";
		
		private var _importXML:XML;
		private var _importHash:Object;
		private var _importLookup:Array;
		private var _loadFailed:Boolean = false;
		
		public function ExternalImportXMLProxy()
		{
			super( NAME );
		}
		
		public function load():void
		{
			sendNotification( NOTE_SOURCE_XML_LOADING );
			
			var loader:FileLoader = new FileLoader();
			loader.addEventListener( FileEvent.FILE_OPEN, handleEvent );
			loader.addEventListener( FileEvent.FILE_OPEN_FAILURE, handleEvent );
			loader.addEventListener( FileEvent.LOAD_QUEUE_COMPLETE, handleEvent );
			
			loader.loadFile( "com/ts/importing/assets/ExternalImportMap.xml", FileTypes.XML );
			loader.loadFile( "com/ts/importing/assets/ExternalImportStates.xml", FileTypes.XML );
		}
		
		public function getImportProxyByName( dataName:String ):ExternalImportProxy
		{
			if( _importLookup )
			{
				return _importHash[ dataName ];
			}
			
			return null;
		}
		
		public function getfileExtFromDataName( dataName:String ):String
		{
			
			if( _importLookup )
			{
				return _importHash[ dataName ].externalFileExtension;
			}
			
			return null;
		}
		
		public function get externalDataNames():Array
		{
			return _importLookup;
		}
		
		private function handleEvent( event:FileEvent ):void
		{
			switch( event.type )
			{
				case FileEvent.FILE_OPEN:
					if( event.file.fileName == "ExternalImportMap.xml" )
					{
						unpackImportMap( event.file );
					}
					else
					{
						unpackImportStates( event.file );
					}
					break;
				
				case FileEvent.FILE_OPEN_FAILURE:
					trace( event.file.fileName + " load failed" );
					_loadFailed = true;
					break;
				
				case FileEvent.LOAD_QUEUE_COMPLETE:
					event.target.removeEventListener( FileEvent.FILE_OPEN, handleEvent );
					event.target.removeEventListener( FileEvent.FILE_OPEN_FAILURE, handleEvent );
					event.target.removeEventListener( FileEvent.LOAD_QUEUE_COMPLETE, handleEvent );
					if( _loadFailed )
					{
						sendLoadedNotification( NOTE_SOURCE_XML_FAILED, NAME, SRNAME );
					}
					else
					{
						sendLoadedNotification( NOTE_SOURCE_XML_LOADED, NAME, SRNAME );
					}
					break;
				
				default:
					break;
			}
		}
		
		private function unpackImportMap( file:IFile ):void
		{
			_importXML = file.fileData as XML;	
		}
		
		private function unpackImportStates( file:IFile ):void
		{
			_importHash = new Object();
			_importLookup = new Array();
			
			var importStateXML:XML = file.fileData as XML;
			
			for each( var metaXML:XML in _importXML.dataDefinition )
			{
				var dataName:String = metaXML.dataName;
				if( metaXML.dataType == "state" )
				{
					var stateXMLList:Array = new Array();
					for each( var stateXML:XML in importStateXML.children() )
					{
						if( dataName == String( stateXML.@dataName ) )
						{
							stateXMLList.push( stateXML );
						}
						
					}
					_importHash[ dataName ] = new ExternalImportProxy( metaXML, stateXMLList );	
				}
				else
				{
					_importHash[ dataName ] = new ExternalImportProxy( metaXML );	
				}
				
				_importLookup.push( dataName );
				trace( _importHash[ dataName ].toString( false ) );
			}
		}
	}
}