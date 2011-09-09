package com.ts.importing.configuration.model.vo
{
	public class TemplateAspectVO
	{
		private var _xml:XML;
		private var _required:Boolean;
		private var _templateName:String;
		
		public function TemplateAspectVO( xml:XML, TemplateName:String )
		{
			_xml = xml;
			_templateName = TemplateName;
			_required = false;
			var required:String = String( _xml.@required );
			if( required.search( "t" ) != -1 || required.search( "T" ) != -1 ) _required = true;
		}
		
		public function get name():String
		{
			return _templateName + "/" + _xml.@name;
		}
		
		public function get longName():String
		{
			return _xml.@longName;
		}
		
		public function get fileName():String
		{
			return _xml.@fileName;
		}
		
		public function get isRequired():Boolean
		{
 			return _required;
		}
		
		public function get aspectClass():String
		{
			return _xml.@aspectClass;
		}
		
		public function get shortName():String
		{
			return ""//_xml.@shortName;
		}
		
		public function get description():String
		{
			return _xml.@description;
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