package com.ts.importing.configuration.model
{
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.importing.configuration.controller.ExternalImportUnpackers;
	import com.ts.importing.configuration.model.vo.ExternalImportMetadataVO;
	import com.ts.importing.configuration.model.vo.ExternalImportStatesVO;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class ExternalImportProxy extends Proxy
	{
		public static const NAME:String = "externalImportProxy";
		
		private var _metadata:ExternalImportMetadataVO;
		private var _states:ExternalImportStatesVO;
		private var _importProgress:Number;
		
		[Bindable]
		public var isValid:Boolean;
		
		[Bindable]
		public var statusMessage:String;
		
		public function ExternalImportProxy( metadataXML:XML, stateXMLList:Array = null )
		{
			_metadata = ExternalImportUnpackers.getExtImportMetadataVOFromXML( metadataXML );
			
			if( stateXMLList )
			{
				var stateList:Array = new Array();
				for each( var stateXML:XML in stateXMLList )
				{
					stateList.push( ExternalImportUnpackers.getExtImportStateVOFromXML( stateXML ) );
					_states = new ExternalImportStatesVO( stateList );
				}
			}
			
			super( compoundName );
			
			_importProgress = 0;
			isValid = true;
			statusMessage = "";
		}
		
		[Bindable]
		public function get importProgress()				:Number{ return _importProgress; }
		public function set importProgress( value:Number )	:void{ _importProgress = value; }
		
		public function get compoundName()			:String{ return NAME + dataName; }
		
		public function get metadata()				:ExternalImportMetadataVO{ return _metadata; }
		public function get states()				:ExternalImportStatesVO{ return _states; }
		
		public function get externalDataName()		:String{ return _metadata.externalDataName; }
		public function get externalFileExtension()	:String{ return _metadata.externalFileExtension; }
		public function get externalFileLocation()	:String{ return _metadata.externalFileLocation; }
		public function get externalParamName()		:String{ return _metadata.externalParamName; }
		public function get dataName()				:String{ return _metadata.dataName; }
		public function get dataType()				:String{ return _metadata.dataType; }
		public function get dataClass()				:String{ return _metadata.dataClass; }
		public function get dataRange()				:String{ return _metadata.dataRange; }
		public function get longName()				:String{ return _metadata.longName; }
		public function get shortName()				:String{ return _metadata.shortName; }
		public function get longDescription()		:String{ return _metadata.longDescription; }
		public function get shortDescription()		:String{ return _metadata.shortDescription; }
		public function get nullValue()				:String{ return _metadata.nullValue; }
		public function get aspect()				:String{ return _metadata.aspect; }
		public function get toolTip()				:String{ return _metadata.toolTip; }
		public function get webLink()				:String{ return _metadata.webLink; }
		
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = dataName + " external import proxy:\n";
			string += _metadata.toString( asList );
			if( _states )
			{
				string += _states.toString( asList );
			}
			
			return string;
		}
	}
}