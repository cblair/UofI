package com.ts.importing.parsing.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.vo.DataVO;
	import com.ts.importing.configuration.controller.ExternalImportConverter;
	
	import mx.utils.StringUtil;

	public class MERParsingPolicy extends AbstractParsingPolicy
	{
		public static const NAME:String = "MERParsingPolicy";
		
		private var _dataCPs:Array;
		private var _rowColumnData:Array;
		
		private var _validArray:Array;
		private var _dataArray:Array;
		private var _dataCPArray:Array;
		private var _dataVO:DataProxy;
		
		public function MERParsingPolicy(externalImportProxies:Array, rawData:String, dataRange:int, dataResolution:int )
		{
			super(externalImportProxies, rawData, NAME, dataRange, dataResolution );
			
			stripMetadata( rawData );
			_dataCPArray = formDataCPRowColumns( _dataCPs );
			_dataArray = formRowColumns( _rowColumnData );
		}
		
		override protected function validateWorkingImportProxy():Boolean
		{
			if( _dataArray[ 0 ].indexOf( _workingImportProxy.externalDataName ) == -1 && _dataCPArray.indexOf( _workingImportProxy.externalDataName ) == -1 )
			{
				_invalidImportProxies.push( _workingImportProxy );
				return false;
			}
			
			return true;
		}
		override protected function parseWorkingProxy():void
		{
			var index:int;
			var levelOfDetail:String = LevelOfDetail.INTERSECTION;
			var lodID:int = 0;
			var valueList:Array;
			var compressedValueList:Array;
			var n:int = _dataArray.length;
			var m:int = _dataCPArray.length;
			var i:int;
			
			valueList = new Array();
			if( _dataArray[0].indexOf( _workingImportProxy.externalDataName ) != -1 )
			{
				index = getDataIndexByDataName( _workingImportProxy.externalDataName );
				
				for( i = 1; i < n; i++ )
				{
					valueList.push( _dataArray[ i ][ index ] );
				}
			}
			else if( _dataCPArray[0].indexOf( _workingImportProxy.externalDataName ) != -1 )
			{
				index = getCPDataIndexByDataName( _workingImportProxy.externalDataName );
				
				for( i = 1; i < m; i++ )
				{
					valueList.push( _dataCPArray[ i ][ index ] );
				}
			}
			
			var data:DataVO = ExternalImportConverter.createDataVO( valueList, _workingImportProxy, 0, valueList.length );
			
			_newDataProxies.push( ExternalImportConverter.createDataProxy( data, _workingImportProxy, levelOfDetail, lodID, 0, _workingImportProxy.dataName + String( lodID ) + "0" ) );
																
			trace( _newDataProxies[ _newDataProxies.length - 1 ].toString( false ) );
			super.parseWorkingProxy();
		}
		
		private function stripMetadata( rawData:String ):void
		{
			var usableArray:Array = rawData.split("\n\r\nData Collection Point");
			
			var dataCPString:String = "Data Collection Point" + usableArray[1].split("\n\r\n")[0];
			var rowColumnString:String = usableArray[1].split("\n\r\n")[1];
			
			_dataCPs = dataCPString.split("\r\n");
			_rowColumnData = rowColumnString.split("\r\n");
			if( StringUtil.trim( _rowColumnData[_rowColumnData.length - 1] ) == "" )
			{
				_rowColumnData.pop();
			}
			
		}
		
		private function formDataCPRowColumns( dataCPs:Array ):Array
		{
			var dirtyDataCPArray:Array = new Array();
			var cleanDataCPArray:Array = [ [ "Data Collection Point", "Link", "Lane", "at", "Length" ] ];
			
			var currentDataCP:String;
			var cleanDataCP:String;
			
			var firstSplitCharacter:String = "";
			var splitCharacter:String = ";";
			
			
			for each( var dataCP:String in dataCPs )
			{
				currentDataCP = dataCP;
				for each( var dataCPHeader:String in cleanDataCPArray[ 0 ] )
				{
					if( dataCPHeader == cleanDataCPArray[ 0 ][ 0 ] )
					{
						currentDataCP = currentDataCP.replace( dataCPHeader, firstSplitCharacter );
					}
					else
					{
						currentDataCP = currentDataCP.replace( dataCPHeader, splitCharacter );
					}
				}
				dirtyDataCPArray.push( currentDataCP );
			}
			
			for each( var dirtyDataCP:String in dirtyDataCPArray )
			{
				dirtyDataCP = dirtyDataCP.replace("m.","m");
				cleanDataCP = StringUtil.restrict( dirtyDataCP, "0-9;." );
				cleanDataCPArray.push( cleanDataCP.split(";") );
			}
			
			return cleanDataCPArray;
		}
		
		private function formRowColumns( stripped:Array ):Array
		{
			var rowColumns:Array = new Array();
			var i:int;
			var n:int;
			for each( var item:String in stripped )
			{
				item = item.replace( "Data C.P.", "DataC.P." ); //Don't want to split this one by space
				
				var tempArray:Array = item.split(" ");
				var tempArray2:Array = new Array();
				
				n = tempArray.length;
				for( i = 0; i < n; i++ )
				{
					if( tempArray[ i ] != " " && tempArray[ i ] != "" )
					{
						if( tempArray[ i ] == "DataC.P." )
						{
							tempArray[ i ] = "Data C.P.";
						}
						tempArray2.push( tempArray[ i ] );
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
		
		private function getCPDataIndexByDataName( dataName:String ):int
		{
			return _dataCPArray[0].indexOf( dataName );
		}
	}
}