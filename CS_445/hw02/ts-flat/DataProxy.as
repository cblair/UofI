package com.ts.data.model
{
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.MetadataVO;
	
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class DataProxy extends Proxy
	{
		public static const NAME:String = "dataProxy";
		
		[Bindable] public var currentValue:Number;
		[Bindable] public var currentRange:Array;
		[Bindable] public var metadata:MetadataVO;
		
		protected var _lastLength:int;
		protected var _lastIndex:int;
		protected var _maxIndex:int;
		protected var _hashTable:Object;
		protected var _changeTable:Object;
		protected var _hashIndex:Array;
		protected var _numValues:int;
		
		private var _data:DataVO;
		private var _compoundName:String;
		
		public function DataProxy( Metadata:MetadataVO, Data:DataVO )
		{
			metadata 	= Metadata;
			_data		= Data;
			_hashTable 	= Data.hashTable; //-- Stores the numeric values of all data types
			_hashIndex 	= Data.hashIndex; //-- Stores a compressed lookup table for the hash
			_maxIndex	= Data.hashIndex.length - 1;
			_numValues 	= Data.numValues;

			_lastLength 	= 0;
			_lastIndex 		= 0;
			_compoundName 	= NAME + dataName + lodID;
			
			_changeTable = makeChangeTable( _hashIndex, _hashTable );
			
			super( _compoundName );
		}
		
		public function get compoundName()		:String{ return _compoundName; }
		public function get dataType()			:String{ return metadata.dataType; }
		public function get dataClass()			:String{ return metadata.dataClass; }
		public function get dataRange()			:String{ return metadata.dataRange; }
		public function get dataName()			:String{ return metadata.dataName; }
		public function get longName()			:String{ return metadata.longName; }
		public function get shortName()			:String{ return metadata.shortName; }
		public function get longDescription()	:String{ return metadata.longDescription; }
		public function get shortDescription()	:String{ return metadata.shortDescription; }
		public function get levelOfDetail()		:String{ return metadata.levelOfDetail; }
		public function get lodID()				:int   { return metadata.lodID; }
		public function get intersection()		:int   { return metadata.intersection; }
		public function get uniqueID()			:String{ return metadata.uniqueID; }
		public function get groupID()			:String{ return String( metadata.lodID ) + "-" + String( metadata.intersection ) }; 
		public function get aspect()			:String{ return metadata.aspect; }
		public function get toolTip()			:String{ return metadata.toolTip; }
		public function get wewbLink()			:String{ return metadata.webLink; }
		public function get dataVO()			:DataVO{ return _data; }
		public function get minValue()			:Number{ return _data.minValue; }
		public function get maxValue()			:Number{ return _data.maxValue; }
		public function get numValues()			:int   { return _numValues; }
		public function get numUniqueValues()	:int   { return _data.numUniqueValues; }
		public function get compressionFactor()	:Number{ return _data.compressionFactor; }
		
		/**
		 * Gets the indexes at which a value is stored in the data ( using a hash function )
		 * 
		 * @param value The value to search for in the data.
		 * @param onRepeat Flag to mark all occurances of value, or just the first in each repeating series
		 * 
		 * @return An array, if onRepeat = true, an exhaustive list of all the indicies where the value is stored. If onRepeat = false,
		 * a list of indicies where the data changes to the value. If the value is not in the dataset, an empty array is returned.
		 **/
		public function indexesOf( value:Number, onRepeat:Boolean = false ):Array
		{
			var indexes:Array = new Array();
			
			if( onRepeat )
			{
				
			}
			else
			{
				for each( var index:int in dataVO.hashIndex )
				{
				}
			}
			return null;
		}
		
		public function changeIndexOf( value:Number ):Array
		{
			var indexes:Array = new Array();
			var valueFound:Boolean = false;
			var n:int = dataVO.hashIndex.length;
			for( var i:int = 0; i < n; i++ )
			{
				if( dataVO.hashTable[ i ] == value && valueFound == false )
				{
					valueFound = true;
					indexes.push( dataVO.hashIndex[ i ] )
				}
				else if( valueFound == true )
				{
					valueFound = false;
					indexes.push( dataVO.hashIndex[ i ] - 1 )
				}
			}
			
			//-- last values are the value we are looking for
			if( valueFound == true )
			{
				indexes.push( dataVO.numValues - 1 );
			}
			return indexes;
		}
		
		/**
		 * Gets the numeric value stored at the lookup index ( using a hash function )
		 * 
		 * @param index The lookup index.
		 * 
		 * @return the numeric value stored at that index.
		 **/
		public function getValueAt( index:int ):Number
		{
			//return inBounds( index ) ? getHashValueAt( index ) : NaN;
			return getHashValueAt( index );
		}
		
		/**
		 * Sets the Numeric [Bindable] "currentValue" to a new value. While this does not return a value, all components bound
		 * to the currentValue will be updated. Allows for implicit updating.
		 * 
		 * @param index The lookup index;
		 **/
		public function setValueTo( index:int ):void
		{
			currentValue = getHashValueAt( index );
			//currentValue = inBounds( index ) ? getHashValueAt( index ) : NaN;
			//trace( uniqueID + " = " + currentValue );
		}
		
		public function setChangeValueTo( index:int ):void
		{
			currentValue = _changeTable[ index ];
		}
		
		/**
		 * INCOMPLETE
		 **/
		public function setRangeTo( startIndex:int, rangeLength:int ):void
		{
			//-- OVERRIDE ME
		}
		
		/**
		 * Uncompresses the Hash Table into an exaustive array of values. This is good when heavy use of the data is required, as 1 to 1 lookup
		 * is always faster than using a hash function.
		 * 
		 * @return An array containing an uncompressed list of every value of this DataProxy
		 **/
		public function get uncompressedData():Array
		{
			return DataConverter.extractListFromHash( _data.hashVO );
		}
		
		/**
		 * Utility Function - converts the DataProxy into an output string.
		 * 
		 * @param asList Flag, if true, the string is rendered in column form, if false, it is rendered in row form.
		 * 
		 * @return a string representation of the DataProxy.
		 **/
		public function toString( asList:Boolean = true ):String
		{
			var string:String = longName + "(" + dataName + ") Data Proxy:";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ", ";
			
			string += "\n";
			string += metadata.toString( asList );
			string += dataVO.toString( asList );
			
			return string;
		}
		
		/**
		 * Hash Function, returns a value from the hash table for the index. Optimizes lookup for sequential indexing ( which is characteristic of uninterrupted iteration )
		 * 
		 * @param index The index at which the value would be stored if it were in an exhaustive list
		 * 
		 * @return the numeric value. If the data type is discrete, returns the unique number for the state ( i.e. 0, 1, 2, 3...etc )
		 **/
		protected function getHashValueAt( index:int ):Number
		{
			//if( inBounds( index ) )
			//{
				if( index >= _hashIndex[ _lastIndex ] )
				{
					while( index >= _hashIndex[ _lastIndex + 1 ] && _lastIndex < _maxIndex )
					{
						_lastIndex++;
					}
				}
				else
				{
					while( index < _hashIndex[ _lastIndex ] )
					{
						_lastIndex--;
					}
				}
				
				return Number( _hashTable[ _lastIndex ] );
			//}
			//return NaN;
		}
		
		/**
		 * Hash Function, returns the key where the indexed value is stored in the hash table. 
		 * Optimizes lookup for sequential indexing ( which is characteristic of uninterrupted iteration )
		 * 
		 * @param index The index at which the value would be stored if it were in an exhaustive list
		 * 
		 * @return the integer key where the values is stored.
		 **/
		protected function getHashKeyFor( index:int ):int
		{
			if( inBounds( index ) )
			{
				var key:int = _lastIndex;
				
				if( index >= _hashIndex[ key ] )
				{
					while( index > _hashIndex[ key ] )
					{
						key++;
					}
				}
				else
				{
					while( index < _hashIndex[ key ] )
					{
						key--;
					}
				}
				
				return key;
			}
			return NaN;
		}
		
		/**
		 * INCOMPLETE
		 **/
		protected function getHashValuesAt( startIndex:int, endIndex:int, toNumber:Boolean = false ):Array
		{
			return null;
		}
		
		/**
		 * Tests an index to see if it is in bounds. Optimizes sequential index validating
		 * 
		 * @param index The lookup index
		 * 
		 * @return true if in range, false if not.
		 **/
		protected function inBounds( index:int ):Boolean
		{
			if( index > _numValues ) //-- Check for forward range error first, try to end test with 1 comparison
			{
				return false;
			}
			
			return index < 0 ? false : true;
		}
		
		/**
		 * Tests an index to see if it is the next sequential index
		 * 
		 * @param index The lookup index.
		 * 
		 * @return true if next index ( last index + 1 ), false if not.
		 **/
		protected function isNextValue( index:int ):Boolean
		{
			return( index == _lastIndex + 1 );
		}
		
		private function makeChangeTable( hashIndex:Array, hashTable:Object ):Object
		{
			var changeTable:Object = new Object();
			var n:int = hashIndex.length;
			for( var i:int = 0; i < n; i++ )
			{
				changeTable[ hashIndex[ i ] ] = hashTable[ i ];
			}
			
			return changeTable;
		}
	}
}