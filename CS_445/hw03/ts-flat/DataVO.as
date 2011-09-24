package com.ts.data.model.vo
{
	public class DataVO
	{
		/**
		 * a virtual object containing a HashTable, HashIndex, and the number of values stored.
		 **/
		public var hashVO:HashVO;
		/**
		 * The maximum value in the data
		 **/
		public var maxValue:Number;
		/**
		 * the smallest value in the data
		 **/
		public var minValue:Number;
		/**
		 * the number of unique values in the data
		 **/
		public var numUniqueValues:int;
		
		public function DataVO( Hash:HashVO, NumUniqueValues:int, MaxValue:Number, MinValue:Number )
		{
			hashVO				= Hash;
			numUniqueValues 	= NumUniqueValues;
			maxValue			= MaxValue;
			minValue			= MinValue;
		}
		
		/**
		 * A hash table of values, allowing repeating data to be stored at a single location.
		 **/
		public function get hashTable():Object{ return hashVO.hashTable };
		/**
		 * The lookup array the hash function uses to get values from the table.
		 * <p> The hash index stores a number representing the range of indicies at which the values in the data set do not change.
		 * The index of the array itself is the key for the hash table.</p>
		 **/
		public function get hashIndex():Array{ return hashVO.hashIndex };
		/**
		 * The number of data values compressed in the hashTable
		 **/
		public function get numValues():int{ return hashVO.numValues };
		/**
		 * Compression Metric - The number of compression chunks in the hash table ( how many times the value changed
		 **/
		public function get chunkLength():int{ return hashIndex.length; }
		/**
		 * Compression Metric - How compact the data is. Worst case scenario ( 1 ) = all values in the data were unique. There were as many chuncks as values.
		 * Best case scenario, ( 0.0000... ) = all values were the same, and 1 value was stored for the entire data set.
		 **/
		public function get compressionFactor():Number{ return chunkLength / numValues; }
		/**
		 * Compression Metric - Returns the average number of values stored in each chunk
		 **/
		public function get valuesPerChunk():Number{ return numValues / chunkLength; }
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "String output for dataVO:";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ", ";
			
			string += leadingFormatString + "numValues = " 	+ String( numValues );
			string += formatString + "numUniqueValues = " 	+ String( numUniqueValues );
			string += formatString + "numChunks = "			+ String( chunkLength );
			string += formatString + "compressionFactor "	+ String( compressionFactor );
			string += formatString + "values/chunk = "		+ String( valuesPerChunk );
			string += formatString + "maxValue = " 			+ String( maxValue );
			string += formatString + "minValue = " 			+ String( minValue );
			
			string += "\n";
			
			string += hashVO.toString( asList, true );
			
			return string;
		}
	}
}