package com.ts.data.model
{
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.HashVO;
	import com.ts.data.model.vo.NullStateVO;
	
	import mx.charts.AreaChart;

	public class DataConverter
	{
		public static function dataListToDataVO( dataList:Array, startIndex:int, numValues:int ):DataVO
		{
			var currentValue:Number = NaN;
			var lastValue:Number 	= NaN;
			
			var hashTable:Object 	= new Object();
			var hashIndex:Array 	= new Array();
			var uniqueValues:Array	= new Array();
			var maxValue:Number		= getFirstNonNullValue( dataList );
			var minValue:Number 	= maxValue;
			
			var workingValue:Number;
			var runningSum:int		= 0;
			
			//-- if the data starts at a later index than 0, pad the front with NaN values.
			if( startIndex > 0 )
			{
				hashIndex.push( 0 );
				hashTable[ 0 ] 	= NaN;
				runningSum 		= startIndex;
			}
			
			var n:int = dataList.length;
			for( var i:int = 0; i < n; i++ )
			{
				currentValue = dataList[ i ];
				if( currentValue != lastValue )
				{
					if( !( isNaN( currentValue ) && isNaN( lastValue ) ) || i == 0 )
					{
						workingValue = currentValue;
						
						hashIndex.push( runningSum );
						hashTable[ hashIndex.length - 1 ] = workingValue;
						lastValue = currentValue;
						
						if( !isNaN( workingValue ) )
						{
							maxValue = Math.max( maxValue, workingValue );
							minValue = Math.min( minValue, workingValue );	
						}
						if( !isNaN( currentValue )&& uniqueValues.indexOf( currentValue ) == -1 ) uniqueValues.push( currentValue );
					}
				}
				
				runningSum++;
			}
			
			if( runningSum < numValues )
			{
				hashIndex.push( runningSum );
				hashTable[ hashIndex.length - 1 ] = NaN;
			}
			
			return new DataVO( 	new HashVO( hashTable, hashIndex, numValues ),
								uniqueValues.length,
								maxValue,
								minValue );
		}
		
		private static function getFirstNonNullValue( data:Array ):Number
		{
			var n:int = data.length;
			for( var i:int = 0; i < n; i ++ )
			{
				if( !isNaN( data[ i ] ) ) break; //-- Find first non-NaN value for initializing max and min values
			}
			return data[ i ];
		}
		
		public static function compressExternalStringListToArray( dataList:Array, length:int, nullValue:String = "null", startIndex:int = 0 ):Array
		{
			var index:String;
			var currentValue:String = XMLElement.COMPRESS_TOKEN;
			var lastValue:String = XMLElement.COMPRESS_TOKEN;
			var workingValue:String;
			var repeatCounter:int = 0;
			var compressToken:String = XMLElement.COMPRESS_TOKEN;
			var nullString:String = XMLElement.NULL;
			var valueList:Array = new Array();
			
			if( startIndex > 0 )
			{
				valueList.push( nullString + compressToken + String( startIndex ) );
			}
			for each( index in dataList )
			{
				currentValue = index;
				if( currentValue != lastValue )
				{
					if( currentValue == nullValue )
					{
						workingValue = nullString;
					}
					else
					{
						workingValue = lastValue;
					}
					
					if( repeatCounter > 1 )
					{
						valueList.push( workingValue + compressToken + String( repeatCounter ) );
						repeatCounter = 1;
					}
					else if( repeatCounter == 0)
					{
						repeatCounter = 1;
					}
					else
					{
						valueList.push( String( workingValue ) );
					}
					
					lastValue = currentValue;
				}
				else
				{
					repeatCounter++;
				}
			}
			
			var lengthDiff:int = length - ( startIndex + dataList.length - 1 );
			if( lengthDiff > 0 )
			{
				valueList.push( nullString + compressToken + String( lengthDiff ) );
			}
			return valueList;
		}
		
		public static function compressStringListToArray( dataList:Array, length:int ):Array
		{
			var index:String;
			var currentValue:String = XMLElement.COMPRESS_TOKEN;
			var lastValue:String = XMLElement.COMPRESS_TOKEN;
			var repeatCounter:int = 0;
			var compressToken:String = XMLElement.COMPRESS_TOKEN;
			var nullString:String = XMLElement.NULL;
			var valueList:Array = new Array();
			
			for each( index in dataList )
			{
				currentValue = index;
				if( currentValue != lastValue )
				{
					if( repeatCounter > 1 )
					{
						valueList.push( lastValue + compressToken + String( repeatCounter ) );
						repeatCounter = 1;
					}
					else if( repeatCounter == 0)
					{
						repeatCounter = 1;
					}
					else
					{
						valueList.push( lastValue );
					}
					
					lastValue = currentValue;
				}
				else
				{
					repeatCounter++;
				}
			}
			
			return valueList;
		}
		
		public static function compressNumericListToArray( dataList:Array, length:int ):Array
		{
			var index:Number;
			var currentValue:Number = NaN;
			var lastValue:Number = NaN;
			var repeatCounter:int = 0;
			var compressToken:String = XMLElement.COMPRESS_TOKEN;
			var nullString:String = XMLElement.NULL;
			var valueList:Array = new Array();
			
			for each( index in dataList )
			{
				currentValue = index;
				if( currentValue != lastValue )
				{
					if( repeatCounter > 1 )
					{
						valueList.push( String( lastValue ) + compressToken + String( repeatCounter ) );
						repeatCounter = 1;
					}
					else if( repeatCounter == 0)
					{
						repeatCounter = 1;
					}
					else
					{
						valueList.push( String( lastValue ) );
					}
					
					lastValue = currentValue;
				}
				else
				{
					repeatCounter++;
				}
			}
			
			return valueList;
		}
		
		public static function compressListToString( dataList:Array ):String
		{
			var index:String;
			var currentValue:String = XMLElement.COMPRESS_TOKEN;
			var lastValue:String = XMLElement.COMPRESS_TOKEN;
			var repeatCounter:int = 0;
			var compressToken:String = XMLElement.COMPRESS_TOKEN;
			var valueString:String = "";
			
			for each( index in dataList )
			{
				if( currentValue != lastValue )
				{
					if( repeatCounter > 1 )
					{
						valueString += String( lastValue ) + compressToken + String( repeatCounter );
						repeatCounter = 1;
					}
					else if( repeatCounter == 0)
					{
						repeatCounter = 1;
					}
					else
					{
						valueString += String( lastValue );
					}
					
					lastValue = currentValue;
				}
				else
				{
					repeatCounter++;
				}
			}
			
			return valueString;
		}
		
		public static function uncompressStringToValues( compressedString:String ):Array
		{
			var compressedValueList:Array = compressedString.split( XMLElement.DATA_DELIMITER );
			var i:int;
			var n:int = compressedValueList.length;
			var j:int;
			var decompressed:Array = new Array();
			var compressedValue:Array;
			var compressToken:String = XMLElement.COMPRESS_TOKEN;
			var value:Number;
			var nullString:String = XMLElement.NULL;
			var repeats:int;
			
			for( i = 0; i < n; i++ )
			{ 
				compressedValue = compressedValueList[ i ].split( compressToken );
				//-- Set and cast these values outside the repeats loop to prevent needless calculations
				value 	= compressedValue[ 0 ] == nullString ? NaN : Number( compressedValue[ 0 ] ); 	//-- replace null string with NaN
				repeats = compressedValue[ 1 ] ? int( compressedValue[ 1 ] ) : 1; 						//-- Will repeat 1 or more times

				for( j = 0; j < repeats; j++ )
				{
					decompressed.push( value );
				}
			}
			
			return decompressed;
		}
		
		/**
		 * Converts a compressed data string ( usually from a load ) to a HashVO.
		 * 
		 * @param compressedData The compressed String containing data to be converted.
		 * 
		 * @return A loaded HashVO containing a HashTable, HashIndex, and the number of values in the data.
		 **/
		public static function convertStringToHash( compressedData:String ):HashVO
		{
			var compressedValueList:Array = compressedData.split( XMLElement.DATA_DELIMITER );
			var i:int;
			var n:int = compressedValueList.length;
			var hashTable:Object = new Object();
			var hashIndex:Array = new Array();
			var compressedValue:Array;
			var compressToken:String = XMLElement.COMPRESS_TOKEN;
			var value:Number;
			var nullString:String = XMLElement.NULL;
			var repeats:int;
			var runningSum:int = 0;
			
			for( i = 0; i < n; i++ )
			{ 
				compressedValue = compressedValueList[ i ].split( compressToken );
				//-- Set and cast these values outside the repeats loop to prevent needless calculations
				value 	= compressedValue[ 0 ] == nullString ? NaN : Number( compressedValue[ 0 ] ); 	//-- replace null string with NaN
				repeats = compressedValue[ 1 ] ? int( compressedValue[ 1 ] ) : 1; 						//-- Will repeat 1 or more times
				
				hashIndex.push( runningSum );
				hashTable[ i ] = value;
				runningSum += repeats;
			}
			
			return new HashVO( hashTable, hashIndex, runningSum );
		}
		
		/**
		 * Converts a HashVO to it's save string equivalent.
		 * 
		 * @param hashVO The HashVO object to be converted.
		 * 
		 * @return A save string in a compressed format: [value][repeatToken(x)][#repeats][valueDelimiter(;)] or [value][valueDelimter(;)]
		 * ( depending if a value repeats or not )
		 **/
		public static function convertHashToString( hashVO:HashVO ):String
		{
			var delimiter:String = XMLElement.DATA_DELIMITER;
			var repeatToken:String = XMLElement.COMPRESS_TOKEN;
			var hashTable:Object = hashVO.hashTable;
			var hashIndex:Array	= hashVO.hashIndex;
			var numValues:int = hashVO.numValues;
			var lastIndex:int = 0;
			var repeats:int = 1;
			var runningSum:int = 0;
			var string:String = "";
			var n:int = hashIndex.length;
			
			//-- i needs to start at 1, not 0, to get a valid repeats value. i @ 0 will always return 0 in the hash Index.
			//-- this will leave the last compressed value to be added manually in the end.
			for( var i:int = 1; i < n; i++ )
			{
				repeats = hashIndex[ i ] - lastIndex; //-- calculate the number of times this value repeats;
				runningSum += repeats 	//-- will need this for the final value;
				if( repeats > 1 ) 		//-- repeating values. compress the value
				{
					string += String( hashTable[ i - 1 ] ) + repeatToken + repeats + delimiter;
				}
				else 					//-- single value, add the value as is, since it cannot be compressed
				{
					string += String( hashTable[ i - 1 ] ) + delimiter;
				}
				
				lastIndex = hashIndex[ i ];
			}
			
			//-- Manually add the last value. have to use the numValues value instead, because the final range is not stored in the hashIndex.
			repeats = numValues - runningSum 	//-- calculate the number of times this final value repeats;
			if( repeats > 1 ) 					//-- repeating values. compress the value. Dont add delimiter, because its the final value.
			{
				string += String( hashTable[ i - 1 ] ) + repeatToken + repeats;
			}
			else 								//-- single value, add the value as is, since it cannot be compressed. Dont add delimiter, because its the final value.
			{
				string += String( hashTable[ i - 1 ] );
			}
			
			return string;
		}
		
		/**
		 * Extracts all the values from a HashVO onto a list
		 * 
		 * @param hashVO The HashVO containing the compressed data values
		 * 
		 * @return An exaustive, uncompressed list of values
		 **/
		public static function extractListFromHash( hashVO:HashVO ):Array
		{
			var hashTable:Object = hashVO.hashTable;
			var hashIndex:Array	= hashVO.hashIndex;
			var numValues:int = hashVO.numValues;
			var workingIndex:int = 0;
			var workingLength:int = hashIndex.length - 1; //-- Need to subtract 1 because we are using this variable to test if the NEXT index exists
			var array:Array = new Array();
			
			for( var i:int = 0; i < numValues; i++ )
			{
				if( workingIndex < workingLength )
				{
					if( i < hashIndex[ workingIndex + 1 ] )
					{
						array.push( hashTable[ workingIndex ] );
					}
					else
					{
						workingIndex++;
						array.push( hashTable[ workingIndex ] );
					}
				}
				else
				{
					//-- this else is to handle the case of the last working index, in wich workingIndex + 1 is out of bounds.
					array.push( hashTable[ workingIndex ] ) //-- workingIndex = the last non null index in the hashIndex.
				}
			}
			return array;
		}
	}
}