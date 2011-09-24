package com.ts.importing.configuration.model.vo
{

	public class TemplateXMLVO
	{
		private var _xml:XML;
		private var _templates:Array;
		
		public function TemplateXMLVO( xml:XML )
		{
			_xml = xml;
			
			_templates = new Array();
			
			for each( var item:XML in _xml.template )
			{
				_templates.push( new TemplateVO( item ) );
			}
		}
		
		public function templateLookup( templateName:String ):TemplateVO
		{
			if( _xml )
			{
				for each( var item:XML in _xml.template )
				{
					if( item.name == templateName )
					{
						return new TemplateVO( item );
					}
				}
			}
			
			return null;
		}
		
		public function get templates():Array
		{
			return _templates;
		}
		
		public function toString():String
		{
			return _xml.toXMLString();
		}
	}
}