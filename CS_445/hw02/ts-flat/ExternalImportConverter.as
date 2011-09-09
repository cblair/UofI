package com.ts.importing.configuration.controller
{
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.model.ConstantDataProxy;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.DiscreteDataProxy;
	import com.ts.data.model.DynamicDataProxy;
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.HashVO;
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.data.model.vo.NullStateVO;
	import com.ts.data.model.vo.StateVO;
	import com.ts.data.model.vo.StatesVO;
	import com.ts.importing.configuration.model.ExternalImportProxy;
	import com.ts.importing.configuration.model.vo.ExternalImportMetadataVO;
	import com.ts.importing.configuration.model.vo.ExternalImportStateVO;
	import com.ts.importing.configuration.model.vo.ExternalImportStatesVO;

	/**
	 * Aids in data normalization during the import process. Externally imported data needs to be
	 * reformatted, and analyzed to make it native to the application. This process only takes place
	 * during import mode, where the user is importing a new model to analyze in the system. The model
	 * can be subsequently saved in this converted form, and loaded as native data.
	 **/
	public class ExternalImportConverter
	{
		/**
		 * Creates a DataVO from external assets. Extra analysis must occur with external conversions since metadata ( such as "Max Value" and
		 * "NumUniqueValues" ) dosen't exist at this point. 
		 * 
		 * @param dataList The list of raw data values, fresh from the import parser. This list will always be
		 * in string form, and must be converted to Numeric values.
		 * 
		 * @param importData The ExternalImportProxy that houses the data. This provides key metadata about the
		 * build. If the data's class is "discrete" then the string state values must be converted to their numeric
		 * equivalents, described in the proxy.
		 * 
		 * @return A normalized, compressed DataVO. This is the only data format accepted by the DataProxy
		 **/
		public static function createDataVO( dataList:Array, importData:ExternalImportProxy, startIndex:int, numValues:int ):DataVO
		{
			switch( importData.metadata.dataClass )
			{
				case DataClass.CONSTANT:
					return getConstantDataVO( dataList, importData.nullValue, startIndex, numValues );
				
				case DataClass.DISCRETE:
					return getDiscreteDataVO( dataList, importData.states, importData.nullValue, startIndex, numValues );
					
				case DataClass.DYNAMIC:
					return getDynamicDataVO( dataList, importData.nullValue, startIndex, numValues );
					
				default:
					return null;
			}
		}
		
		private static function getDiscreteDataVO( dataList:Array, states:ExternalImportStatesVO, nullValue:String, startIndex:int, numValues:int ):DataVO
		{
			var currentValue:String = XMLElement.COMPRESS_TOKEN;
			var lastValue:String 	= XMLElement.COMPRESS_TOKEN;
			
			var hashTable:Object 	= new Object();
			var hashIndex:Array 	= new Array();
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
					
					hashIndex.push( runningSum );
					hashTable[ hashIndex.length - 1 ] = currentValue == nullValue ? NaN : states.getNumByExternalString( currentValue ); //-- replace all state strings with numbers
					lastValue = currentValue;
				}
				
				runningSum++;
			}
			
			if( runningSum < numValues )
			{
				hashIndex.push( runningSum );
				hashTable[ hashIndex.length - 1 ] = NaN;
			}
			
			var stateArray:Array = states.states.sortOn( "stateNumber" );
			
			return new DataVO( 	new HashVO( hashTable, hashIndex, numValues ),
								states.numUniqueValues,
								stateArray[ stateArray.length - 1 ].stateNumber,
								stateArray[ 0 ].stateNumber );
		}
		
		private static function getDynamicDataVO( dataList:Array, nullValue:String, startIndex:int, numValues:int ):DataVO
		{
			return getConstantDataVO( dataList, nullValue, startIndex, numValues );
		}
		
		private static function getConstantDataVO( dataList:Array, nullValue:String, startIndex:int, numValues:int ):DataVO
		{
			var currentValue:String = XMLElement.COMPRESS_TOKEN;
			var lastValue:String 	= XMLElement.COMPRESS_TOKEN;
			
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
					workingValue = Number( currentValue );
					
					hashIndex.push( runningSum );
					hashTable[ hashIndex.length - 1 ] = currentValue == nullValue ? NaN : workingValue; //-- replace all external null characters with NaN,
					lastValue = currentValue;
					
					if( !isNaN( workingValue ) )
					{
						maxValue = Math.max( maxValue, workingValue );
						minValue = Math.min( minValue, workingValue );
					}
					
					if( currentValue != nullValue && uniqueValues.indexOf( currentValue ) == -1 ) uniqueValues.push( currentValue );
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
		
		public static function createDataProxy( data:DataVO, importData:ExternalImportProxy, levelOfDetail:String, lodID:int, intersection:int, uniqueID:String ):DataProxy
		{
			var metadata:MetadataVO = new MetadataVO( 	importData.dataName, 		importData.dataType, 			importData.dataClass, 
														importData.dataRange, 		importData.longName, 			importData.shortName,
														importData.longDescription, importData.shortDescription, 	levelOfDetail,
														lodID,						intersection,					uniqueID,						
														importData.aspect, 			importData.toolTip,			importData.webLink );
			
			switch( importData.metadata.dataClass )
			{
				case DataClass.CONSTANT:
					return new ConstantDataProxy( metadata, data );
					
				case DataClass.DISCRETE:
					return new DiscreteDataProxy( metadata, data, getNormalStates( importData.states ) );
					
				case DataClass.DYNAMIC:
					return new DynamicDataProxy( metadata, data );
					
				default:
					return null;
			}
		}
		
		private static function getNormalStates( externalStates:ExternalImportStatesVO ):StatesVO
		{
			var newStates:Array = new Array();
			
			for each( var exState:ExternalImportStateVO in externalStates.states )
			{
				newStates.push( new StateVO(	exState.stateName,		exState.longName,			exState.shortName,
												exState.longDescription,exState.shortDescription, 	exState.stateString,	
												exState.stateNumber, 	exState.toolTip,			exState.webLink ) );
				
			}
			
			return new StatesVO( newStates );
		}
	}
}