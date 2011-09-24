package com.ts.importing.parsing.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.enumerations.MetadataAttribute;
	import com.ts.data.model.DataConverter;
	import com.ts.data.model.vo.DataVO;
	import com.ts.importing.configuration.controller.ExternalImportConverter;
	import com.ts.importing.configuration.model.ExternalImportProxy;
	import com.ts.importing.configuration.model.enumerations.ExternalImportDataName;
	
	import flash.sampler.NewObjectSample;
	
	import mx.utils.StringUtil;

	public class LDPParsingPolicy extends AbstractParsingPolicy
	{
		
		public static const NAME:String = "LDPParsingPolicy";
		
		//Pre-parse variables
		private var _dataIndices:Array;
		private var _workingHeader:Array;
		private var _workingData:Array;
		private var _workingDataSet:Array;
		private var _dataArray:Array;
		private var _controllerID:String;
		private var _splitMap:Array;
		private var _isVAP:Boolean;
		private var _isRR:Boolean;
		
		public function LDPParsingPolicy( externalImportProxies:Array, rawData:String, dataRange:int, dataResolution:int )
		{
			super(externalImportProxies, rawData, NAME, dataRange, dataResolution );
			
			_isVAP = false;
			if( rawData.match( "Program file: vap" ) )
			{
				_isVAP = true;
			}
			
			_workingHeader = parseHeader( _rawData );
			_workingData = parseData( _rawData );
			_workingDataSet = joinDataset( _workingHeader, _workingData );
		}

		override protected function validateWorkingImportProxy():Boolean
		{
			for( var i:int = 0; i < _workingDataSet[0].length; i++ )
			{
				if( _workingDataSet[0][i].match( _workingImportProxy.externalDataName ) )
				{
					return true;
				}
			}
			return false
		}
		
		override protected function parseWorkingProxy():void
		{
			trace( "Parse Import Started" );
			var index:int;
			var checkHeadersArray:Array = new Array();
			var storeHeader:String;
			var phaseNumber:String;
			var levelOfDetail:String = LevelOfDetail.PHASE;
			var lodID:int;
			var headerIsUsed:Boolean = false;
			var valueList:Array;
			var compressedValueList:Array;
			var n:int;
			
			for each( var dataHeader:String in _workingDataSet[0] )
			{
				storeHeader 	= dataHeader;
				dataHeader 		= StringUtil.restrict(dataHeader, "^0-9");
				phaseNumber 	= StringUtil.restrict( storeHeader, "0-9" );
				
				headerIsUsed = false;
				for each( var usedHeader:String in checkHeadersArray )
				{
					if( usedHeader == storeHeader )
					{
						headerIsUsed = true;
					}
				}
				if( _workingImportProxy.externalDataName.split(" ").join("").match( dataHeader.split(" ").join("") ) && headerIsUsed == false )
				{
					checkHeadersArray.push( storeHeader );
					index = getDataIndexByDataName( dataHeader + phaseNumber );
					lodID 			= phaseNumber == "" ? 0 							: int( phaseNumber );
					levelOfDetail 	= phaseNumber == "" ? LevelOfDetail.INTERSECTION 	: LevelOfDetail.PHASE;
					
					valueList = new Array();
					n = _workingDataSet.length;
					for( var i:int = 1; i < n; i++ )
					{
						valueList.push( _workingDataSet[ i ][ index ] );
					}
					
					var data:DataVO = ExternalImportConverter.createDataVO( valueList, _workingImportProxy, 0, valueList.length );
					
					if( _isVAP )
					{
						//-- need to restore this when finished.
						var tempDataName:String = _workingImportProxy.dataName;
						
						_workingImportProxy.metadata.dataName = "vap" + _workingImportProxy.dataName;
						if( _workingImportProxy.dataName == ExternalImportDataName.LDP_VAP_SIG_DISPLAY )
						{
							if( valueList.indexOf("=") != -1 )
							{
								_workingImportProxy.metadata.dataName = ExternalImportDataName.LDP_GATE_SIGNAL;
							}
						}
						_newDataProxies.push( ExternalImportConverter.createDataProxy( data, _workingImportProxy, levelOfDetail, lodID, int( _controllerID ), _workingImportProxy.dataName + String( lodID ) +_controllerID ) );
						_workingImportProxy.metadata.dataName = tempDataName;
					}
					else
					{
						_newDataProxies.push( ExternalImportConverter.createDataProxy( data, _workingImportProxy, levelOfDetail, lodID, int( _controllerID ), _workingImportProxy.dataName + String( lodID ) +_controllerID ) );
					}
					trace( _newDataProxies[ _newDataProxies.length - 1 ].toString( false ) );
				}
			}
			super.parseWorkingProxy();
		}
		
		private function getDataIndexByDataName( dataName:String ):int
		{
			for( var i:int = 0; i < _workingDataSet[0].length; i++ )
			{
				if( _workingDataSet[0][i].match( dataName ) )
				{
					return i;
				}
			}
			return -1;
		}
		
		private function parseHeader( rawData:String ):Array
		{
			var masterArray:Array = new Array();
			
			var toSplitArray:Array = stripMetadata( rawData.split( "\n\r\n" ) );
						
			_dataIndices = new Array();
			_dataIndices = splitHeaders( toSplitArray[0] );
			
			var headerArray:Array = toSplitArray[0].split( "\n" );
			_dataArray = toSplitArray[1].split( "\n" );
			
			for each( var item:String in headerArray )
			{
				var tempArray:Array = item.split( "" );
				masterArray.push( tempArray );
			}
			
			masterArray = rotate2DArray( masterArray );
			var cleanArray:Array = cleanArray( masterArray );
			
			return cleanArray;	
		}
		
		private function stripMetadata( array:Array ):Array
		{
			var removeHeaderLines:int = 3;
			var tempArray:Array;
			for( var i:int = 0; i < array.length; i++ )
			{
				if( array[ i ].match("SC ") )
				{
					tempArray = array[ i ].split(";");
					tempArray = tempArray[0].split(" ");
					_controllerID = tempArray[ tempArray.length - 1 ];
				}
			}
			for( var k:int = 0; k < removeHeaderLines; k++ )
			{
				array.shift();
			}
			
			return array;
		}
		
		private function splitHeaders( string:String ):Array
		{
			var tempIndexArray:Array = string.split( "\n" );
			var indexString:String = tempIndexArray[ tempIndexArray.length - 1 ];
			tempIndexArray = indexString.split( "" );
			for ( var i:int = 0; i < tempIndexArray.length; i++ )
			{
				if ( tempIndexArray[i] == "\r" )
				{
					tempIndexArray[i] = " ";
				}
			}
			
			return tempIndexArray;
			
		}
		
		private function cleanArray( array:Array ):Array
		{
			for( var i:int = 0; i < array.length; i++ )
			{
				for( var j:int = 0; j < array[i].length; j++ )
				{
					if( array[i][j] == " " )
					{
						array[i][j] = "";
					}
					else
					{
						break;
					}
				}
				array[i] = array[i].join("");
			}
			var cleanArray:Array = new Array();
			for( i = 0; i < array.length; i++ )
			{
				if( !array[i].match("\r") && array[i] != "" )
				{
					cleanArray.push( array[i] );
				}
			}
			
			return cleanArray;
		}
		
		private function rotate2DArray(array:Array):Array {
			var d1:int = array.length;
			var d2:int = array[0].length;
			var r:Array = new Array(d2);
			for(var i:int = 0; i < d2; i++){
				var t:Array = new Array(d1);
				for(var j:int = 0; j < d1; j++){
					t[j] = array[j][i];
				}
				r[i] = t;
			}
			return r;
		}
		
		private function parseData( rawData:String ):Array
		{
			
			//-- 1. get all the starting index positions of data.
			var indexPositionList:Array = new Array();
			for ( var i:int = 0; i < _dataIndices.length; i++ )
			{
				if( _dataIndices[ i ] != " " )
				{
					indexPositionList.push( i + 1 );
				}
			}
			
			//-- 2. chop up the data into columns to the right of the index positons
			var dataTable:Array = new Array();
			var values:Array;
			var value:String;
			var n:int = indexPositionList.length;
			var begin:int;
			var end:int;
			for each( var row:String in _dataArray )
			{
				values = new Array();
				begin = 0;
				end = 0;
				for( i = 0; i < n; i++ )
				{
					end = indexPositionList[ i ];
					value = StringUtil.trim( row.slice( begin, end ) );
					values.push( value == "" ? " " : value );
					begin = end;
				}
				dataTable.push( values );
			}
			
			//-- check to see if the last row is white space. pop it if it is.
			var lastRow:Array = dataTable[ dataTable.length - 1 ];
			var isWhitespace:Boolean = true;
			for each( var char:String in lastRow )
			{
				if( char != " " && char != "\n" && char != "\r" )
				{
					isWhitespace = false;
					break;
				}
			}
			
			if( isWhitespace )
			{
				dataTable.pop();
			}
			
			if( _isVAP )
			{
				n = 10;
				var legacyDataTable:Array = dataTable;
				dataTable = new Array();
				for each( var dataRow:Array in legacyDataTable )
				{
					for( i = 0; i < n; i++ )
					{
						dataTable.push( dataRow );
					}
				}
			}
			
			return dataTable;
			
			var cleanDataArray:Array = new Array();
			for each( var iItem:String in _dataArray )
			{
				var tempArray1:Array = iItem.split("");
				var tempArray2:Array = new Array();
				var tinyArray:Array = new Array();
				var tinyString:String;
				var jItem:String;
				for ( i = 0; i < tempArray1.length; i++ )
				{
					if( tempArray1[i] != " " || _dataIndices[i] != " " ) //test if both arrays have nothing there
					{
						tinyArray.push( tempArray1[i] );//Push value to little array to make single values
						tinyString = "";				//Reset Tiny String
					}
					else
					{
						tinyString = tinyArray.join("");//If the next character was empty, join the tinyArray
						tinyArray = new Array();		//Reset tinyArray
					}
					if( tinyString != "" && tinyString != "\r" )
					{
						tempArray2.push( tinyString );	//If the joined item isn't empty or a line break, push it
					}
				}
				if( tempArray2.length > 0 )
				{
					cleanDataArray.push( tempArray2 );
				}
			}
			
			
			return cleanDataArray;
		}
		
		private function joinDataset( workingHeader:Array, workingData:Array ):Array
		{
			workingData.unshift( workingHeader );
			_workingDataSet = workingData;
			return _workingDataSet;
		}
	}
}