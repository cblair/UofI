package com.ts.importing.configuration.model
{
	import com.ts.startup.EntityProxy;
	
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	
	import org.puremvc.as3.utilities.startupmanager.interfaces.IStartupProxy;
	import com.ts.importing.configuration.model.vo.TemplateXMLVO;

	public class TemplateXMLProxy extends EntityProxy implements IStartupProxy
	{
		public static const NAME:String 							= "templateXMLProxy";
		public static const SRNAME:String							= "templateSRProxy";
		
		public static const NOTE_TEMPLATE_XML_LOADING:String		= NAME + "/note/templateXMLLoading";
		public static const NOTE_TEMPLATE_XML_LOADED:String			= NAME + "/note/templateXMLLoaded";
		public static const NOTE_TEMPLATE_XML_FAILED:String			= NAME + "/note/templateXMLFailed";
		
		private var _templateXML:XML;
		
		public function TemplateXMLProxy()
		{
			super( NAME );
		}
		
		public function get templateXMLVO():TemplateXMLVO
		{
			if( _templateXML )
			{
				return new TemplateXMLVO( _templateXML );
			}
			return null;
		}
		
		public function load():void
		{
			sendNotification( NOTE_TEMPLATE_XML_LOADING );
			
			var request:URLRequest = new URLRequest( "com/ts/importing/assets/TemplateXML.xml" );
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
					sendLoadedNotification( NOTE_TEMPLATE_XML_FAILED, NAME, SRNAME );
					break;
			}
		}
		
		private function unpackLoader( loader:URLLoader ):void
		{
			_templateXML = new XML( loader.data );
			sendLoadedNotification( NOTE_TEMPLATE_XML_LOADED, NAME, SRNAME );
		}
	}
}