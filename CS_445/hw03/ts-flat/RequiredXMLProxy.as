package com.ts.importing.configuration.model
{
	import com.ts.startup.EntityProxy;
	
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	
	import org.puremvc.as3.utilities.startupmanager.interfaces.IStartupProxy;
	import com.ts.importing.configuration.model.vo.RequiredAspectXMLVO;

	public class RequiredXMLProxy extends EntityProxy implements IStartupProxy
	{
		public static const NAME:String 							= "coreXMLProxy";
		public static const SRNAME:String							= "coreSRProxy";
		
		public static const NOTE_CORE_XML_LOADING:String		= NAME + "/note/coreXMLLoading";
		public static const NOTE_CORE_XML_LOADED:String			= NAME + "/note/coreXMLLoaded";
		public static const NOTE_CORE_XML_FAILED:String			= NAME + "/note/coreXMLFailed";
		
		private var _coreXML:XML;
		
		public function RequiredXMLProxy()
		{
			super( NAME );
		}
		
		public function get coreXMLVO():RequiredAspectXMLVO
		{
			if( _coreXML )
			{
				return new RequiredAspectXMLVO( _coreXML );
			}
			return null;
		}
		
		public function load():void
		{
			sendNotification( NOTE_CORE_XML_LOADING );
			
			var request:URLRequest = new URLRequest( "com/ts/importing/assets/aspects/RequiredAspect.xml" );
			var loader:URLLoader = new URLLoader();
			
			loader.addEventListener( IOErrorEvent.IO_ERROR, eventHandler );
			loader.addEventListener( Event.COMPLETE, eventHandler );
			
			loader.load( request );
		}
		
		private function eventHandler( event:Event ):void
		{
			event.target.removeEventListener( IOErrorEvent.IO_ERROR, eventHandler );
			event.target.removeEventListener( Event.COMPLETE, eventHandler );
			switch( event.type )
			{
				case Event.COMPLETE:
					unpackLoader( event.target as URLLoader );
					break;
				
				case IOErrorEvent.IO_ERROR:
					sendLoadedNotification( NOTE_CORE_XML_FAILED, NAME, SRNAME );
					break;
			}
		}
		
		private function unpackLoader( loader:URLLoader ):void
		{
			_coreXML = new XML( loader.data );
			sendLoadedNotification( NOTE_CORE_XML_LOADED, NAME, SRNAME );
		}
	}
}