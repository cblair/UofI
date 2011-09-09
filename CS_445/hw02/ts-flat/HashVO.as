package com.ts.data.model.vo
{
	import com.ts.data.model.DataConverter;

	/**
	 * A hash table that allows a compressed version of the data set to be stored in memory, and saved to file. It uses 2 key components, the HashTable and HashIndex.
	 * The HashTable holds values at keys equaling the "INDEX" of the HashIndex array, not the values of the array itself. The actual values of the HashIndex array hold
	 * the indexes in the data where the value changed.
	 * <p> Example: the data, A, A, A, A, A, B, C, C, C, C, D... would generate a hashIndex Arrray of [ 0 ] = 0, [ 1 ] = 5, [ 2 ] = 6, [ 3 ] = 10, ...
	 * and a hashTable of { 0:A, 1:B, 2:C, 3:D }</p>
	 * <p> Data values for records 0-4 are stored at Hash key 0, 5 at 1, 6-9 at 2, and 10-... at 3. the lookup key is found by comparing the record lookup index agains the hashIndex.</p>
	 **/
	public class HashVO
	{
		/**
		 * A hash table of values, allowing repeating data to be stored at a single location.
		 **/
		public var hashTable:Object;
		/**
		 * The lookup array the hash function uses to get values from the table.
		 * <p> The hash index stores a number representing the range of indicies at which the values in the data set do not change.
		 * The index of the array itself is the key for the hash table.</p>
		 **/
		public var hashIndex:Array;
		/**
		 * The number of data values compressed in the hashTable
		 **/
		public var numValues:int;
		
		public function HashVO( HashTable:Object, HashIndex:Array, NumValues:int )
		{
			hashTable = HashTable;
			hashIndex = HashIndex;
			numValues = NumValues;
		}
		
		public function toString( asList:Boolean = true, completeList:Boolean = false ):String
		{
			var string:String = "Data Values:";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ",";
			
			string += "\n";
			
			if( completeList )
			{
				string += DataConverter.convertHashToString( this );
			}
			else
			{
				if( hashIndex[ 0 ] )
				{
					string += String( hashTable[ 0 ] );
				}
				if( hashIndex[ 1 ] )
				{
					string += "x" + String( hashIndex[ 1 ] ) + ", " + String( hashTable[ 1 ] );
				}
				if( hashIndex[ 2 ] )
				{
					string += "x" + String( hashIndex[ 2 ] - hashIndex[ 1 ] );
				}
				
				string += ", ...";
			}
			
			string += "\n";
			
			return string;
		}
	}
}