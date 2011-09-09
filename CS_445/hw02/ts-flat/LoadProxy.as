package com.ts.load.model
{
	import com.fManagement.fileLoader.FileEvent;
	import com.fManagement.fileLoader.FileLoader;
	import com.fManagement.fileTypes.FileTypes;
	import com.fManagement.utilities.FileBrowseEvent;
	import com.fManagement.utilities.FileBrowser;
	import com.ts.data.enumerations.DataRange;
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.enumerations.MetadataAttribute;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.model.DatabaseProxy;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.load.controller.LoadUnpackers;
	import com.ts.load.controller.SystemUnpackers;
	import com.ts.load.xml.SystemElement;
	import com.ts.presentation.model.map.SystemProxy;
	
	import flash.events.Event;
	import flash.filesystem.File;
	import flash.filesystem.FileMode;
	import flash.filesystem.FileStream;
	import flash.net.FileFilter;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	import spark.components.DataGroup;
	
	public class LoadProxy extends Proxy
	{
		public static const NAME:String						= "LoadProxy";
		
		public static const NOTE_LOAD:String				= NAME + "/note/load";
		public static const NOTE_CANCEL_LOAD:String			= NAME + "/note/cancelLoad";
		public static const NOTE_LOAD_COMPLETE:String		= NAME + "/note/loadComplete";
		public static const NOTE_ENTER_LOAD:String			= NAME + "/note/enterLoad";
		public static const NOTE_EXIT_LOAD:String			= NAME + "/note/exitLoad";
		
		public static const ACTION_LOAD:String				= NAME + "/action/load";
		public static const ACTION_CANCEL_LOAD:String		= NAME + "/action/cancelLoad";
		
		public static const STATE_LOADING:String			= NAME + "/state/loading";
		public static const STATE_APP_READY_LOADING:String	= NAME + "/state/appReadyLoading";

		private const STATUS_LOADING:String					= "Loading File";
		private const STATUS_UNPACKING:String				= "Unpacking File";
		private const STATUS_LOAD_COMPLETE:String			= "";
		private const STATUS_LOAD_FAILED:String				= "";
		
		private const LOAD_PROGRESS_PORTION:int = 30;
		private const UNPACK_PROGRESS_PORTION:int = 70;
		
		[Bindable] public var loadStatus:String = "";
		[Bindable] public var loadProgress:int = 0;
		[Bindable] public var loadTarget:String = "";
		
		public function LoadProxy()
		{
			super( NAME );
		}
		
		public function loadModule():void
		{
			var fileBrowser:FileBrowser = new FileBrowser();
			
			fileBrowser.addEventListener( FileBrowseEvent.FILE_SELECTED, handleEvent );
			fileBrowser.addEventListener( FileBrowseEvent.BROWSE_CANCELED, handleEvent );
			fileBrowser.browseForFile( File.desktopDirectory.nativePath, new FileFilter( "XML Files", "*.xml" ), "Open XML Module" );
		}
		
		private function handleEvent( event:Event ):void
		{
			switch( event.type )
			{
				case FileEvent.FILE_OPEN:
					event.target.removeEventListener( FileEvent.FILE_OPEN, handleEvent );
					event.target.removeEventListener( FileEvent.FILE_OPEN_FAILURE, handleEvent );
					event.target.removeEventListener( FileEvent.LOAD_PROGRESS_UPDATE, handleEvent );
					
					loadStatus = STATUS_LOAD_COMPLETE;
					unpackXML( FileEvent( event ).file.fileData as XML  );
					break;
				
				case FileEvent.LOAD_PROGRESS_UPDATE:
					loadProgress = FileEvent( event ).progress * LOAD_PROGRESS_PORTION;
					break;
				
				case FileEvent.FILE_OPEN_FAILURE:
					event.target.removeEventListener( FileEvent.FILE_OPEN, handleEvent );
					event.target.removeEventListener( FileEvent.FILE_OPEN_FAILURE, handleEvent );
					event.target.removeEventListener( FileEvent.LOAD_PROGRESS_UPDATE, handleEvent );
					
					closeLoader();
					loadStatus = STATUS_LOAD_FAILED;
					
					break;
				
				case FileBrowseEvent.FILE_SELECTED:
					event.target.removeEventListener( FileBrowseEvent.FILE_SELECTED, handleEvent );
					event.target.removeEventListener( FileBrowseEvent.BROWSE_CANCELED, handleEvent );
					
					loadFile( FileBrowseEvent( event ).path );
					break;
				
				case FileBrowseEvent.BROWSE_CANCELED:
					event.target.removeEventListener( FileBrowseEvent.FILE_SELECTED, handleEvent );
					event.target.removeEventListener( FileBrowseEvent.BROWSE_CANCELED, handleEvent );
					sendNotification( NOTE_CANCEL_LOAD );
					break;
				
				default:
					break;
			}
		}
		
		private function loadFile( fileURL:String ):void
		{
			loadStatus = STATUS_LOADING;
			
			var fileLoader:FileLoader = new FileLoader();
			fileLoader.addEventListener( FileEvent.FILE_OPEN, handleEvent );
			fileLoader.addEventListener( FileEvent.FILE_OPEN_FAILURE, handleEvent );
			fileLoader.addEventListener( FileEvent.LOAD_PROGRESS_UPDATE, handleEvent );
			fileLoader.loadFile( fileURL, FileTypes.XML );
		}
		
		private function unpackXML( rawXML:XML ):void
		{
			loadStatus 		= STATUS_UNPACKING;
			loadProgress 	= LOAD_PROGRESS_PORTION;
			
			var progressIncrement:Number 	= UNPACK_PROGRESS_PORTION / getModelLength( rawXML );
			var database:DatabaseProxy 		= facade.retrieveProxy( DatabaseProxy.NAME ) as DatabaseProxy;
			
			for each( var dataProxyXML:XML in rawXML[ XMLElement.MODEL ].children() )
			{
				var dataProxy:DataProxy 	= LoadUnpackers.unpackDataProxy( dataProxyXML );
				database.indexDataProxy( dataProxy );
				
				trace( dataProxy.toString( false ) );
				
				loadProgress += progressIncrement;
			}
			
			var system:SystemProxy = SystemUnpackers.unpackSystemProxy( rawXML[ SystemElement.SYSTEM ][0], database.dataIndex );
			
			loadProgress = LOAD_PROGRESS_PORTION + UNPACK_PROGRESS_PORTION;
			sendNotification( SystemProxy.NOTE_FINALIZE_APPLICATION, system );
			closeLoader();
		}
		
		private function getModelLength( rawXML:XML ):int
		{
			var modelLength:int = 0;
			
			for each( var data:XML in rawXML.model.children() )
			{
				modelLength++;
			}
			
			return modelLength;
		}
		
		private function closeLoader():void
		{
			loadTarget = "";
			loadProgress = 0;
			loadStatus = "";
		}
	}
}