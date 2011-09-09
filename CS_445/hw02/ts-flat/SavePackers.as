package com.ts.save.controller
{
	import com.ts.data.enumerations.DataAttribute;
	import com.ts.data.enumerations.MetadataAttribute;
	import com.ts.data.enumerations.StateAttribute;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.model.DataConverter;
	import com.ts.data.model.DataProxy;
	import com.ts.data.model.DiscreteDataProxy;
	import com.ts.data.model.vo.DataVO;
	import com.ts.data.model.vo.MetadataVO;
	import com.ts.data.model.vo.StateVO;
	import com.ts.data.model.vo.StatesVO;

	public class SavePackers
	{
		public static function packDataProxy( dataProxy:DataProxy ):XML
		{
			var dataProxyXML:XML = new XML( "<" + XMLElement.DATA_PROXY + "/>" );
			
			dataProxyXML.appendChild( packMetadataVO( dataProxy.metadata ) );
			dataProxyXML.appendChild( packDataVO( dataProxy.dataVO ) );
			
			if( dataProxy is DiscreteDataProxy )
			{
				dataProxyXML.appendChild( packStatesVO( DiscreteDataProxy( dataProxy ).states ) );
			}
			
			return dataProxyXML;
		}
		
		/**
		 * Packs a "DataVO" ( a hash table and hash index ) into an XMLList element )
		 **/
		public static function packDataVO( dataVO:DataVO ):XMLList
		{
			var openTag:String = "<" + XMLElement.DATA + ">";
			var closeTag:String = "</" + XMLElement.DATA + ">";
			var dataString:String = DataConverter.convertHashToString( dataVO.hashVO );
			var dataXML:XMLList = new XMLList( openTag + dataString + closeTag );
			
			dataXML.@[ DataAttribute.NUM_VALUES ]			= dataVO.numValues;
			dataXML.@[ DataAttribute.NUM_UNIQUE_VALUES ]	= dataVO.numUniqueValues;
			dataXML.@[ DataAttribute.MAX_VALUE ]			= dataVO.maxValue;
			dataXML.@[ DataAttribute.MIN_VALUE ]			= dataVO.minValue;
			
			return dataXML;
		}
		
		/**
		 * Packs a MetadataVO into an XML element
		 **/
		public static function packMetadataVO( metadataVO:MetadataVO ):XML
		{
			var metadataXML:XML										= new XML( "<" + XMLElement.METADATA + "/>" );
			metadataXML.@[ MetadataAttribute.DATA_NAME ]			= metadataVO.dataName;
			metadataXML.@[ MetadataAttribute.DATA_TYPE ]			= metadataVO.dataType;
			metadataXML.@[ MetadataAttribute.DATA_CLASS ]			= metadataVO.dataClass;
			metadataXML.@[ MetadataAttribute.DATA_RANGE ]			= metadataVO.dataRange;
			metadataXML.@[ MetadataAttribute.LONG_NAME ]			= metadataVO.longName;
			metadataXML.@[ MetadataAttribute.SHORT_NAME ]			= metadataVO.shortName;
			metadataXML.@[ MetadataAttribute.LONG_DESCRIPTION ]		= metadataVO.longDescription;
			metadataXML.@[ MetadataAttribute.SHORT_DESCRIPTION ]	= metadataVO.shortDescription;
			metadataXML.@[ MetadataAttribute.LEVEL_OF_DETAIL ]		= metadataVO.levelOfDetail;
			metadataXML.@[ MetadataAttribute.LOD_ID ]				= metadataVO.lodID;
			metadataXML.@[ MetadataAttribute.INTERSECTION ]			= metadataVO.intersection;
			metadataXML.@[ MetadataAttribute.UNIQUE_ID ]			= metadataVO.uniqueID;
			metadataXML.@[ MetadataAttribute.ASPECT ]				= metadataVO.aspect;
			metadataXML.@[ MetadataAttribute.TOOL_TIP ]				= metadataVO.toolTip;
			metadataXML.@[ MetadataAttribute.WEB_LINK ]				= metadataVO.webLink;
			
			return metadataXML;
		}
		
		/**
		 * Packs an entire "StatesVO" ( a smart collection of StateVO's ) into an XML List element )
		 **/
		public static function packStatesVO( statesVO:StatesVO ):XMLList
		{
			var openTag:String 		= "<" + XMLElement.STATES + ">";
			var dataString:String 	= "";
			var closeTag:String 	= "</" + XMLElement.STATES + ">";
			
			for each( var stateVO:StateVO in statesVO.states )
			{
				dataString += packStateVO( stateVO ).toXMLString(); 
			}
			
			return new XMLList( openTag + dataString + closeTag );
		}
		
		/**
		 * Packs a StateVO into an XML element
		 **/
		public static function packStateVO( stateVO:StateVO ):XML
		{
			var stateXML:XML								= new XML( "<" + XMLElement.STATE + "/>" );
			stateXML.@[ StateAttribute.STATE_NAME ]			= stateVO.stateName;
			stateXML.@[ StateAttribute.LONG_NAME ]			= stateVO.longName;
			stateXML.@[ StateAttribute.SHORT_NAME ]			= stateVO.shortName;
			stateXML.@[ StateAttribute.LONG_DESCRIPTION ]	= stateVO.longDescription;
			stateXML.@[ StateAttribute.SHORT_DESCRIPTION ]	= stateVO.shortDescription;
			stateXML.@[ StateAttribute.STATE_STRING ]		= stateVO.stateString;
			stateXML.@[ StateAttribute.STATE_NUMBER ]		= stateVO.stateNumber;
			stateXML.@[ StateAttribute.TOOL_TIP ]			= stateVO.toolTip;
			stateXML.@[ StateAttribute.WEB_LINK ]			= stateVO.webLink;
			
			return stateXML;
		}
	}
}