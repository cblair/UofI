package com.ts.importing.configuration.controller
{
	import com.ts.data.enumerations.MetadataAttribute;
	import com.ts.data.factories.DataTypeFactory;
	import com.ts.data.model.vo.DataTypeVO;
	import com.ts.importing.configuration.model.vo.ExternalImportMetadataVO;
	import com.ts.importing.configuration.model.vo.ExternalImportStateVO;

	public class ExternalImportUnpackers
	{
		/**
		 * Injects ExternalImportState XML data into an ExternalImportStateVO
		 **/
		public static function getExtImportStateVOFromXML( stateXML:XML ):ExternalImportStateVO
		{
			return new ExternalImportStateVO(	String( stateXML.@stateName ),			String( stateXML.@dataName ),		String( stateXML.@longName ),
												String( stateXML.@shortName ),			String( stateXML.@longDescription ),String( stateXML.@shortDescription ),
												String( stateXML.@externalStateString ),String( stateXML.@stateString ),	int( stateXML.@stateNumber ),			
												String( stateXML.@toolTip ),			String( stateXML.@webLink )	);
		}
		
		/**
		 * Injects ExternalImportMap XML data into an ExternalImportMetadataVO
		 **/
		public static function getExtImportMetadataVOFromXML( metadataXML:XML ):ExternalImportMetadataVO
		{
			var dataTypeVO:DataTypeVO = DataTypeFactory.getDataType( metadataXML.dataType );
			
			return new ExternalImportMetadataVO( 	metadataXML.externalDataName,	metadataXML.externalFileExtension,	metadataXML.externalFileLocation,
												 	metadataXML.externalParamName,	metadataXML.dataName,				metadataXML.dataType,
													dataTypeVO.dataClass,			dataTypeVO.dataRange,				metadataXML.longName,
													metadataXML.shortName,			metadataXML.longDescription,		metadataXML.shortDescription,
													metadataXML.nullValue,			metadataXML.aspect,					metadataXML.toolTip,
													metadataXML.webLink );
		}
	}
}