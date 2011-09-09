package com.ts.importing.parsing.model
{
	import com.ts.data.enumerations.LevelOfDetail;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.model.vo.DataVO;
	import com.ts.importing.configuration.controller.ExternalImportConverter;
	import com.ts.importing.configuration.model.enumerations.RequiredImportDataName;

	public class FZPParsingPolicy extends AbstractParsingPolicy
	{
		public static const NAME:String = "FZPParsingPolicy";
		
		private var _validArray:Array;
		private var _dataArray:Array;
		private var _vehicleArray:Array;
		private var _startTimeOffset:int;
		
		
		public function FZPParsingPolicy(externalImportProxies:Array, rawData:String, dataRange:int, dataResolution:int )
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
		
		private var _vehicleIndexLookup:Array;
		private var _timeArray:Array;
		override protected function initializeParser():void
		{
			_vehicleIndexLookup = buildVehicleIndex();
			_timeArray = buildTimeArray();
		}
		
		//-- Builds 2D array; one row per vehicle, each row is an array of the indexes for that vehicle
		private var _vehicleNumIndex:int;
		private var _numberOfVehicles:int;
		private var _simTimeIndex:int;
		private function buildVehicleIndex():Array
		{
			var i:int;
			var n:int = _dataArray.length;
			
			var currentVehicle:int;
			var vehicleIndex:Array = new Array();
			
			_simTimeIndex = _dataArray[ 0 ].indexOf( "t" );
			_startTimeOffset = _dataArray[ 1 ][ _simTimeIndex ] * _dataResolution;
			_vehicleNumIndex = _dataArray[ 0 ].indexOf( "VehNr" );
			
			for( i = 0; i < n; i++ )
			{
				currentVehicle = int( _dataArray[ i ][ _vehicleNumIndex ] );
				if( !vehicleIndex[ currentVehicle ] )
				{
					vehicleIndex[ currentVehicle ] = new Array();
				}
				vehicleIndex[ currentVehicle ].push( i );
			}
			
			_numberOfVehicles = vehicleIndex.length - 1;
			return vehicleIndex;
		}
		
		//-- Builds array of unique time values, used for finding startIndex in the parser
		private function buildTimeArray():Array
		{
			var i:int;
			var n:int = _dataArray.length;
			
			var currentTime:String;
			var timeIndex:int = _dataArray[ 0 ].indexOf( "t" );
			var lastTime:String = "";
			
			var timeArray:Array = new Array();
			
			for( i = 1; i < n; i++ )
			{
				//-- Building time array
				currentTime =  _dataArray[ i ][ timeIndex ];
				if( currentTime != lastTime )
				{
					lastTime = currentTime;
					timeArray.push( currentTime );
				}
			}
			return timeArray;
		}
		
		private var _parseInitialized:Boolean = false;
		private var _firstInstanceFound:Boolean;
		override protected function parseWorkingProxy():void
		{
			var dataIndex:int;
			var startIndex:int = 0;
			var valueList:Array;
			var compressedValueList:Array;
			var levelOfDetail:String = LevelOfDetail.VEHICLE;
			var lodID:int;
			
			dataIndex = _dataArray[0].indexOf( _workingImportProxy.externalDataName );
			
			var vehicleIndex:Array;
			var index:int;
			for each( vehicleIndex in _vehicleIndexLookup )
			{
				valueList = new Array();
				compressedValueList = new Array();
				
				if( _vehicleIndexLookup.indexOf( vehicleIndex ) != 0 ) //-- First value in _vehicleIndex is empty to keep index the same as vehicle ID Number
				{
					lodID = _vehicleIndexLookup.indexOf( vehicleIndex );
					if( _workingImportProxy.dataName == RequiredImportDataName.FZP_VEH_RWORLD_Y || _workingImportProxy.dataName == RequiredImportDataName.FZP_VEH_WORLD_Y )
					{
						for each( index in vehicleIndex )
						{
							valueList.push( String( Number( _dataArray[ index ][ dataIndex ] ) * -1 ) );
						}
					}
					else
					{
						for each( index in vehicleIndex )
						{
							valueList.push( _dataArray[ index ][ dataIndex ] );
						}
					}
					
					var data:DataVO = ExternalImportConverter.createDataVO( valueList, _workingImportProxy, _timeArray.indexOf( _dataArray[ vehicleIndex[0] ][ _simTimeIndex ] ) + _startTimeOffset, _timeArray.length + _startTimeOffset );
					
					_newDataProxies.push( ExternalImportConverter.createDataProxy( data, _workingImportProxy, levelOfDetail, lodID, 0, _workingImportProxy.dataName + String( lodID ) + "0" ) );
						
					trace( _newDataProxies[ _newDataProxies.length - 1 ].toString( false ) );
				}
			}
			
			super.parseWorkingProxy();
		}
		
		private var _compressedArray:Array;
		private function compressData( uncompressed:Array ):String
		{
			_compressedArray = new Array();
			
			var i:int;
			var j:int = uncompressed.length;
			var runningValue:String = uncompressed[0];
			var numConsecutiveValues:int = 0;
			var fullString:String = "";
			var newString:String = "";
			for( i = 0; i < j; i++ )
			{
				if( uncompressed[ i ] == runningValue )
				{
					numConsecutiveValues++;
				}
				else
				{
					if( numConsecutiveValues > 1 )
					{
						newString = runningValue + XMLElement.COMPRESS_TOKEN + String( numConsecutiveValues ) + XMLElement.DATA_DELIMITER;
					}
					else
					{
						newString = runningValue + XMLElement.DATA_DELIMITER;
					}
					fullString = fullString + newString;
					runningValue = uncompressed[ i ];
					numConsecutiveValues = 0;
				}
			}
			
			return fullString;
		}
		
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
			
			//-- Eliminate extreme link numbers
			var i:int;
			var j:int;
			var linkIndex:int = rowColumns[0].indexOf("Link");
			var parseLinkToInt:int;
			var parsePastLinkToInt:int;
			
			return rowColumns;
		}
		
		private function getDataIndexByDataName( dataName:String ):int
		{
			return _dataArray[0].indexOf( dataName );
		}
	}
}