package com.ts.importing.configuration.model.vo
{

	public class RequiredAspectXMLVO
	{
		private var _xml:XML;
		private var _coreDataVOs:Array;
		
		public function RequiredAspectXMLVO( xml:XML )
		{
			_xml = xml;
			
			_coreDataVOs = new Array();
			
			for each( var item:XML in _xml.data )
			{
				_coreDataVOs.push( new RequiredAspectVO( item ) );
			}
		}
		
		public function coreDataLookup( dataName:String ):RequiredAspectVO
		{
			if( _xml )
			{
				for each( var item:XML in _xml.data )
				{
					if( item.name == dataName )
					{
						return new RequiredAspectVO( item );
					}
				}
			}
			
			return null;
		}
		
		public function get coreDataVOs():Array
		{
			return _coreDataVOs
		}
		
		public function toString():String
		{
			return _xml.toXMLString();
		}
	}
}