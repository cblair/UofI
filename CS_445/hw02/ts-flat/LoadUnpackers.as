package com.ts.load.controller
{
	import com.ts.data.enumerations.DataAttribute;
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.enumerations.MetadataAttribute;
	import com.ts.data.enumerations.StateAttribute;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.factories.DataTypeFactory;
	import com.ts.data.model.ConstantDataProxy;
	import com.ts.data.model.DataConverter;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.DiscreteDataProxy;
	import com.ts.data.model.DynamicDataProxy;
	import com.ts.data.model.vo.DataTypeVO;
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.HashVO;
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.data.model.vo.StateVO;
	import com.ts.data.model.vo.StatesVO;
	
	/**
	 * Unpacking utility, used to load previously saved XML Application Modules into memory
	 **/ 
	public class LoadUnpackers
	{
		/**
		 * Unpacks an entire XML dataProxy element and injects it into a working dataProxy.
		 * <p>( Coordinates the execution of all other unpacking functions. )</p>
		 * 
		 * @param dataProxyXML The Parent XML element of a dataProxy.
		 * 
		 * @return A DataProxy, which handles the complete model of a dataset.
		 **/ 
		public static function unpackDataProxy( dataProxyXML:XML ):DataProxy
		{
			var metadata:MetadataVO = unpackMetadataVO( dataProxyXML[ XMLElement.METADATA ][ 0 ] );
			var data:DataVO 		= unpackDataVO( dataProxyXML[ XMLElement.DATA ][ 0 ] );
			
			switch( metadata.dataClass )
			{
				case DataClass.CONSTANT:
					return new ConstantDataProxy( metadata, data );
				
				case DataClass.DISCRETE:
					return new DiscreteDataProxy( metadata, data, unpackStatesVO( dataProxyXML[ XMLElement.STATES ] ) );
				
				case DataClass.DYNAMIC:
					return new DynamicDataProxy( metadata, data );
				
				default:
					return null;
			}
		}
		
		/**
		 * Unpacks an XML data element into a DataVO object
		 * 
		 * @param dataXML The XML that contains the compressed data string.
		 * 
		 * @return the DataVO object that will house the data as a hash table, along with metadata information
		 **/
		public static function unpackDataVO( dataXML:XML ):DataVO
		{
			//-- dataXML[ 0 ] is the only xml value in the element. this gets the string value stored there
			return new DataVO( 	DataConverter.convertStringToHash( dataXML[ 0 ] ),
								int   ( dataXML.@[ DataAttribute.NUM_UNIQUE_VALUES ] ),
								Number( dataXML.@[ DataAttribute.MAX_VALUE ] ),
								Number( dataXML.@[ DataAttribute.MIN_VALUE ] )	);
		}
		
		/**
		 * Unpacks Metadata XML data into a MetadataVO
		 * 
		 * @param metadataXML The XML element with metadata information about the data proxy
		 * 
		 * @return A MetadataVO which stores all the human-centric information about the data, things like name and description
		 **/
		public static function unpackMetadataVO( metadataXML:XML ):MetadataVO
		{
			var dataTypeVO:DataTypeVO = DataTypeFactory.getDataType( String( metadataXML.@[ MetadataAttribute.DATA_TYPE ] ) );
			
			return new MetadataVO( 	String( metadataXML.@[ MetadataAttribute.DATA_NAME ] ),
									String( metadataXML.@[ MetadataAttribute.DATA_TYPE ] ),
									String( metadataXML.@[ MetadataAttribute.DATA_CLASS ] ),
									String( metadataXML.@[ MetadataAttribute.DATA_RANGE ] ),
									String( metadataXML.@[ MetadataAttribute.LONG_NAME ] ),
									String( metadataXML.@[ MetadataAttribute.SHORT_NAME ] ),
									String( metadataXML.@[ MetadataAttribute.LONG_DESCRIPTION ] ),
									String( metadataXML.@[ MetadataAttribute.SHORT_DESCRIPTION ] ),
									String( metadataXML.@[ MetadataAttribute.LEVEL_OF_DETAIL ] ),
									int   ( metadataXML.@[ MetadataAttribute.LOD_ID ] ),
									int   ( metadataXML.@[ MetadataAttribute.INTERSECTION ] ),
									String( metadataXML.@[ MetadataAttribute.UNIQUE_ID ] ),
									String( metadataXML.@[ MetadataAttribute.ASPECT ] ),
									String( metadataXML.@[ MetadataAttribute.TOOL_TIP ] ),
									String( metadataXML.@[ MetadataAttribute.WEB_LINK ] )	);
		}
		
		/**
		 * Unpacks a "StatesVO" XML List into a StatesVO with child StateVOs
		 * 
		 * @param statesXML The XML element containing 1 or more state elements.
		 * 
		 * @return The StatesVO ( plural ) acts as a wrapper and property accessor for the given states of a discrete data set.
		 **/
		public static function unpackStatesVO( statesXML:XMLList ):StatesVO
		{
			var stateVOs:Array = new Array();
			for each( var stateXML:XML in statesXML.children() )
			{
				stateVOs.push( unpackStateVO( stateXML ) );
			}
			
			return new StatesVO( stateVOs );
		}
		
		/**
		 * Unpacks State XML data into a StateVO
		 * 
		 * @param stateXML The XML element containg state metadata attributes
		 * 
		 * @return StateVO's maintain all the human-centric data concerning the states in a discrete data set.
		 **/
		public static function unpackStateVO( stateXML:XML ):StateVO
		{
			return new StateVO( String( stateXML.@[ StateAttribute.STATE_NAME ] ),
								String( stateXML.@[ StateAttribute.LONG_NAME ] ),
								String( stateXML.@[ StateAttribute.SHORT_NAME ] ),
								String( stateXML.@[ StateAttribute.LONG_DESCRIPTION ] ),
								String( stateXML.@[ StateAttribute.SHORT_DESCRIPTION ] ),
								String( stateXML.@[ StateAttribute.STATE_STRING ] ),
								Number( stateXML.@[ StateAttribute.STATE_NUMBER ] ),
								String( stateXML.@[ StateAttribute.TOOL_TIP ] ),
								String( stateXML.@[ StateAttribute.WEB_LINK ] )	);
		}
	}
}