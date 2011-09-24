package com.ts.data.model.vo
{
	import com.ts.data.enumerations.DataClass;
	import com.ts.data.enumerations.MetadataAttribute;
	import com.ts.data.enumerations.XMLElement;
	import com.ts.data.factories.DataTypeFactory;

	public class MetadataVO
	{
		public var dataName:String;
		public var dataType:String;
		public var dataClass:String;
		public var dataRange:String;
		public var longName:String;
		public var shortName:String;
		public var longDescription:String;
		public var shortDescription:String;
		public var levelOfDetail:String;
		public var lodID:int;
		public var intersection:int;
		public var uniqueID:String;
		public var aspect:String;
		public var toolTip:String;
		public var webLink:String;
		
		
		public function MetadataVO(	DataName:String, 		DataType:String, 		DataClass:String, 
									DataRange:String, 		LongName:String, 		ShortName:String,
									LongDescription:String,	ShortDescription:String,LevelOfDetail:String, 	
									LodID:int,			 	Intersection:int,		UniqueID:String,		
									Aspect:String,			ToolTip:String,	WebLink:String )
		{
			dataName 		= DataName;
			dataType 		= DataType;
			dataClass		= DataClass;
			dataRange		= DataRange;
			longName 		= LongName;
			shortName 		= ShortName;
			longDescription = LongDescription;
			shortDescription= ShortDescription;
			levelOfDetail	= LevelOfDetail;
			lodID			= LodID;
			intersection	= Intersection;
			uniqueID		= UniqueID;
			aspect			= Aspect;
			toolTip			= ToolTip;
			webLink			= WebLink;
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "Metadata for " + longName + ":";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ", ";
			
			string += leadingFormatString + "dataName = " 	+ dataName;
			string += formatString + "dataType = " 			+ dataType;
			string += formatString + "dataClass = " 		+ dataClass;
			string += formatString + "dataRange = " 		+ dataRange;
			string += formatString + "longName = " 			+ longName;
			string += formatString + "shortName = " 		+ shortName;
			string += formatString + "longDescroption = " 	+ longDescription;
			string += formatString + "shortDescription = "  + shortDescription;
			string += formatString + "levelOfDetail = " 	+ levelOfDetail;
			string += formatString + "lodID = " 			+ String( lodID );
			string += formatString + "intersection = " 		+ String( intersection );
			string += formatString + "uniqueID = " 			+ uniqueID;
			string += formatString + "aspect = "			+ aspect;
			string += formatString + "toolTip = " 			+ toolTip;
			string += formatString + "webLink = "			+ webLink;
			
			string += "\n";
			
			return string;
		}
		
		public function clone():MetadataVO
		{
			return new MetadataVO( 	dataName, 			dataType, 			dataClass, 
									dataRange, 			longName, 			shortName,
									longDescription,	shortDescription, 	levelOfDetail, 		
									lodID,				intersection,		uniqueID,			
									aspect, 			toolTip, 			webLink );
		}
	}
}