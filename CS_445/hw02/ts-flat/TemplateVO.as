package com.ts.importing.configuration.model.vo
{

	public class TemplateVO
	{
		private var _xml:XML;
		private var _aspects:Array;
		
		public function TemplateVO( xml:XML )
		{
			_xml = xml;
			_aspects = new Array();
			for each( var item:XML in _xml )
			{
				for( var i:int = 0; i < item.*.length(); i++ )
				{
					_aspects.push( new TemplateAspectVO( item.children()[ i ] as XML, name ) );
				}	
			}
		}
		
		public function get name():String
		{
			return _xml.@name;
		}
		
		public function get longName():String
		{
			return _xml.@longName;
		}
		
		public function get shortName():String
		{
			return _xml.@shortName;
		}
		
		public function get description():String
		{
			return _xml.@description;
		}
		
		public function get xml():XML
		{
			return _xml;
		}
		
		public function get aspects():Array
		{
			return _aspects;
		}
		
		public function toString():String
		{
			return _xml.toXMLString();
		}
	}
}