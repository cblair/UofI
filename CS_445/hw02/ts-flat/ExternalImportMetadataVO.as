package com.ts.importing.configuration.model.vo
{
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.enumerations.MetadataAttribute;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.factories.DataTypeFactory;

	public class ExternalImportMetadataVO
	{
		public var externalDataName:String;
		public var externalFileExtension:String;
		public var externalFileLocation:String;
		public var externalParamName:String;
		public var dataName:String;
		public var dataType:String;
		public var dataClass:String;
		public var dataRange:String;
		public var longName:String;
		public var shortName:String;
		public var longDescription:String;
		public var shortDescription:String;
		public var nullValue:String;
		public var toolTip:String;
		public var webLink:String;
		public var aspect:String;
		
		
		public function ExternalImportMetadataVO( 	ExternalDataName:String,	ExternalFileExtension:String,	ExternalFileLocation:String, 
													ExternalParamName:String, 	DataName:String, 				DataType:String, 
													DataClass:String, 			DataRange:String, 				LongName:String, 
													ShortName:String, 			LongDescription:String,			ShortDescription:String,
													NullValue:String, 			Aspect:String, 					ToolTip:String,		
													WebLink:String	)
		{
			externalDataName		= ExternalDataName;
			externalFileExtension	= ExternalFileExtension;
			externalFileLocation	= ExternalFileLocation;
			externalParamName		= ExternalParamName;
			dataName 				= DataName;
			dataType 				= DataType;
			dataClass				= DataClass;
			dataRange				= DataRange;
			longName 				= LongName;
			shortName 				= ShortName;
			longDescription			= LongDescription;
			shortDescription		= ShortDescription;
			nullValue 				= NullValue ? NullValue : "";
			aspect					= Aspect;
			toolTip					= ToolTip;
			webLink					= WebLink;
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "Metadata for " + longName + ":";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ", ";
			
			string += leadingFormatString + "externalDataName = " 	+ externalDataName;
			string += formatString + "externalFileExtension = "		+ externalFileExtension;
			string += formatString + "externalFileLocation = "		+ externalFileLocation;
			string += formatString + "externalParamName = " 		+ externalParamName;
			string += formatString + "dataName = " 					+ dataName;
			string += formatString + "dataType = " 					+ dataType;
			string += formatString + "dataClass = " 				+ dataClass;
			string += formatString + "dataRange = " 				+ dataRange;
			string += formatString + "longName = " 					+ longName;
			string += formatString + "shortName = " 				+ shortName;
			string += formatString + "longDescription = " 			+ longDescription;
			string += formatString + "shortDescription = " 			+ shortDescription;
			string += formatString + "nullValue = " 				+ nullValue;
			string += formatString + "aspect = "					+ aspect;
			string += formatString + "toolTip = " 					+ toolTip;
			string += formatString + "webLink = " 					+ webLink;
			
			return string;
		}
		
		public function clone():ExternalImportMetadataVO
		{
			return new ExternalImportMetadataVO( 	externalDataName, 	externalFileExtension, 	externalFileLocation, 
													externalParamName, 	dataName, 				dataType, 
													dataClass, 			dataRange, 				longName, 
													shortName, 			longDescription,		shortDescription,
													nullValue, 			aspect, 				toolTip,
													webLink	);
		}
	}
}