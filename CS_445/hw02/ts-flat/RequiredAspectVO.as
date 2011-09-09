package com.ts.importing.configuration.model.vo
{
	public class RequiredAspectVO
	{
		private var _xml:XML;
		
		public function RequiredAspectVO( xml:XML )
		{
			_xml = xml;
		}
		
		public function get name():String
		{
			return _xml.dataName;
		}
		
		public function get longName():String
		{
			return _xml.longName;
		}
		
		public function get shortName():String
		{
			return _xml.shortName;
		}
		
		public function get dataType():String
		{
			return _xml.dataType;
		}
		
		public function get nullValue():String
		{
			return _xml.nullValue;
		}
		
		public function get xml():XML
		{
			return _xml;
		}
		
		public function toString():String
		{
			return _xml.toXMLString();
		}
	}
}