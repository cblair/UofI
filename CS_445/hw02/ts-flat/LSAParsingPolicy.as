package com.ts.importing.parsing.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataConverter;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.vo.DataVO;
	import com.ts.importing.configuration.controller.ExternalImportConverter;
	import com.ts.importing.configuration.model.ExternalImportProxy;
	
	import mx.utils.StringUtil;

	public class LSAParsingPolicy extends AbstractParsingPolicy
	{
		public static const NAME:String = "LSAParsingPolicy";
		
		private var _validArray:Array;
		private var _dataArray:Array;
		private var _dataVO:DataProxy;
		
		private var _phaseCount:int;
		
		public function LSAParsingPolicy(externalImportProxies:Array, rawData:String, dataRange:int, dataResolution:int )
		{
			super( externalImportProxies, rawData, NAME, dataRange, dataResolution );
			
			var dataArray:Array = formRows( rawData );
			_validArray = stripUnneeded( dataArray );
			_dataArray = formRowColumns( _validArray );
		}
		
		public function get numberOfPhases():int
		{
			var row:Array = _dataArray[0];
			var index:int = row.indexOf("SGroup");
			_phaseCount = 0;
			for each( row in _dataArray )
			{
				var phaseNumber:int = parseInt(row[index+1]);
				if( phaseNumber > _phaseCount )
				{
					_phaseCount = phaseNumber;
				}
			}
			return _phaseCount;
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
				if( !_validArray[0].match( importProxy.externalDataName ) )
				{
					_invalidImportProxies.push( importProxy );
					isValid = false;
					//trace( importProxy.externalDataName + " is not valid in ." + importProxy.externalFileExtension )
				}
			}
			
			return isValid;
		}*/
		
		override protected function parseWorkingProxy():void
		{
			var index:int;
			var levelOfDetail:String = LevelOfDetail.INTERSECTION;
			var lodID:int = 0;
			var valueList:Array;
			var compressedValueList:Array;
			var n:int = _dataArray.length;
			
			index = getDataIndexByDataName( _workingImportProxy.externalDataName );
			valueList = new Array();
			
			for( var i:int = 1; i < n; i++ )
			{
				valueList.push( _dataArray[ i ][ index + 1 ] );
			}
			
			var data:DataVO = ExternalImportConverter.createDataVO( valueList, _workingImportProxy, 0, valueList.length );
			
			_newDataProxies.push( ExternalImportConverter.createDataProxy( data, _workingImportProxy, levelOfDetail, lodID, 0, _workingImportProxy.dataName + String( lodID ) + "0" ) );
			
			trace( _newDataProxies[ _newDataProxies.length - 1 ].toString( false ) );
			super.parseWorkingProxy();
		}
		
		private function formRows( rawData:String ):Array
		{
			rawData = rawData.split("\r").join("");
			var newArray:Array = rawData.split("\n");
			return newArray;
		}
		
		private function stripUnneeded( rows:Array ):Array
		{
			var rowsToShift:int = 8;
			for( var i:int = 0; i <  rowsToShift; i++ )
			{
				rows.shift();
			}
			
			var fixedRows:Array = new Array();
			for each( var item:String in rows )
			{
				if( !item.match(";") && item != "" )
				{
					fixedRows.push( item );
				}
			}
			
			return fixedRows;
		}
		
		private function formRowColumns( stripped:Array ):Array
		{
			var rowColumns:Array = new Array();
			for each( var item:String in stripped )
			{
				var tempArray:Array = item.split(" ");
				var tempArray2:Array = new Array();
				for each( var item2:String in tempArray )
				{
					item2 = item2.split(" ").join("");
					var isInexternal:Boolean = false;
					for each( var importProxyVO:ExternalImportProxy in _externalImportProxies )
					{
						if( item2 == importProxyVO.externalDataName )
						{
							isInexternal = true; 											//Check if item in string matches externalDataName
						}
					}
					var stringRestriction:String = StringUtil.restrict( item2, "0-9" ); // - Check to see if item2 is a number
					var lastAddedNumberValue:String = " ";								// - Set to space so that it is not empty
					if( tempArray2.length > 0 )											// - If the last value added to tempArray2 was a number, don't add this one
					{
						lastAddedNumberValue = StringUtil.restrict( tempArray2[ tempArray2.length - 1 ], "0-9" );
					}
					if( item2 != "" )													//Make sure item isn't empty
					{
						if( isInexternal == true )							
						{
							tempArray2.push( item2 );									//If it's a externalDataName, pass it in
						}
						else
						{
							if( stringRestriction != "" && lastAddedNumberValue == "" && tempArray2.length > 0  )
							{
								tempArray2.push( item2 );								//If not, make sure it's a number and isn't the Lane number, and is'nt the first number that comes before SGroup
							}
						}
					}
				}
				rowColumns.push( tempArray2 );
			}
			return rowColumns;
		}
		
		private function getDataIndexByDataName( dataName:String ):int
		{
			return _dataArray[0].indexOf( dataName );
		}
	}
}