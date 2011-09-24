package com.ts.importing.parsing.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataConverter;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.vo.DataVO;
	import com.ts.importing.configuration.controller.ExternalImportConverter;
	import com.ts.importing.configuration.model.ExternalImportProxy;

	public class STRParsingPolicy extends AbstractParsingPolicy
	{
		public static const NAME:String = "STRParsingPolicy";
		
		private var _validArray:Array;
		private var _dataArray:Array;
		private var _dataVO:DataProxy;
		
		public function STRParsingPolicy(externalImportProxies:Array, rawData:String, dataRange:int, dataResolution:int )
		{
			super(externalImportProxies, rawData, NAME, dataRange, dataResolution );
			
			var dataArray:Array = formRows( rawData );
			_validArray = stripMetadata( dataArray );
			_dataArray = formRowColumns( _validArray );
		}
		
		override protected function validateWorkingImportProxy():Boolean
		{
			if( _dataArray[ 0 ].indexOf( _workingImportProxy.externalDataName ) == -1 )
			{
				_invalidImportProxies.push( _workingImportProxy );
				return false;
			}
			
			return true;
		}
		/*override public function validateImport():Boolean
		{
			var isValid:Boolean = true;
			for each( var importProxy:ExternalImportProxy in _externalImportProxies )
			{
				if( _validArray[0].indexOf( importProxy.externalDataName ) == -1 )
				{
					_invalidImportProxies.push( importProxy );
					//isValid = false;
					trace( importProxy.externalDataName + "is not valid in ." + importProxy.externalFileExtension )
				}
			}
			
			return isValid;
		}*/
		
		override protected function parseWorkingProxy():void
		{
			var index:int;
			var dataArray:Array = new Array();
			var levelOfDetail:String = LevelOfDetail.INTERSECTION;
			var lodID:int = 0;
			var valueList:Array;
			var compressedValueList:Array;
			var n:int = _dataArray.length;
			
			index = getDataIndexByDataName( _workingImportProxy.externalDataName );
			valueList = new Array();
			
			for( var i:int = 1; i < n; i++ )
			{
				valueList.push( _dataArray[ i ][ index ] );
			}
			
			//compressedValueList = DataConverter.compressExternalDataListToArray( valueList, valueList.length, _workingImportProxy.nullValue, 0 );
			
			var data:DataVO = ExternalImportConverter.createDataVO( valueList, _workingImportProxy, 0, valueList.length );
			
			_newDataProxies.push( ExternalImportConverter.createDataProxy( data, _workingImportProxy, levelOfDetail, lodID, 0, _workingImportProxy.dataName + String( lodID ) + "0" ) );
			
			//_newDataProxies.push( DataProxyFactory.getFromRuntimeAssets( valueList, _workingImportProxy.dataName, _workingImportProxy.dataType, _workingImportProxy.longName, _workingImportProxy.shortName,
			//														 _workingImportProxy.nullValue, levelOfDetail, lodID, 0, _workingImportProxy.aspect, _workingImportProxy.toolTip ) );
			trace( _newDataProxies[ _newDataProxies.length - 1 ].toString( false ) );
			super.parseWorkingProxy();
		}
/*		override public function parseImport():Array
		{
			var index:int;
			var dataProxies:Array = new Array();
			var levelOfDetail:String = LevelOfDetailE.INTERSECTION;
			var lodID:int = 0;
			var valueList:Array;
			var n:int = _dataArray.length;
			
			for each( var importProxy:ExternalImportProxy in _externalImportProxies )
			{
				index = getDataIndexByDataName( importProxy.externalDataName );
				valueList = new Array();
				
				for( var i:int = 1; i < n; i++ )
				{
					valueList.push( _dataArray[ i ][ index ] );
				}
				dataProxies.push( DataProxyFactory.getFromRuntimeAssets( valueList, importProxy.dataName, importProxy.dataType, importProxy.longName, importProxy.shortName,
																		 importProxy.nullValue, levelOfDetail, lodID, 0, importProxy.aspect, importProxy.toolTip ) );
				trace( dataProxies[ dataProxies.length - 1 ].toString() );
			}
			
			return dataProxies;
		}*/
		
		private function formRows( rawData:String ):Array
		{
			rawData = rawData.split("\r").join("");
			var newArray:Array = rawData.split("\n");
			return newArray;
		}
		
		private function stripMetadata( rows:Array ):Array
		{
			var stripped:Array = new Array();
			for each( var item:String in rows )
			{
				if( item.match(";") )
				{
					stripped.push( item );
				}
			}
			return stripped;
		}
		
		private function formRowColumns( stripped:Array ):Array
		{
			var rowColumns:Array = new Array();
			for each( var item:String in stripped )
			{
				item = item.split(" ").join("");
				var tempArray:Array = item.split(";");
				if( tempArray[ tempArray.length-1 ] == "" )
				{
					tempArray.pop();
				}
				rowColumns.push( tempArray );
			}
			return rowColumns;
		}
		
		private function getDataIndexByDataName( dataName:String ):int
		{
			return _dataArray[0].indexOf( dataName );
		}
	}
}