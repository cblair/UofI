package com.ts.importing.configuration.model.vo
{
	import com.ts.data.enumerations.StateAttribute;
	import com.ts.data.enumerations.XMLElement;
	
	import flash.text.StaticText;

	public class ExternalImportStateVO
	{
		public var stateName:String;
		public var dataName:String;
		public var longName:String;
		public var shortName:String;
		public var longDescription:String;
		public var shortDescription:String;
		public var externalStateString:String;
		public var stateString:String;
		public var stateNumber:int;
		public var toolTip:String;
		public var webLink:String;
		
		public function ExternalImportStateVO(	StateName:String,			DataName:String,		LongName:String, 
												ShortName:String,			LongDescription:String,	ShortDescription:String,		
												ExternalStateString:String, StateString:String, 	StateNumber:int, 	 	 
												ToolTip:String,				WebLink:String		)
		{
			stateName 			= StateName;
			dataName			= DataName;
			longName 			= LongName;
			shortName 			= ShortName;
			longDescription		= LongDescription;
			shortDescription 	= ShortDescription;
			externalStateString = ExternalStateString;
			stateString			= StateString;
			stateNumber			= StateNumber;
			toolTip				= ToolTip;
			webLink				= WebLink;
		}
		
		public function clone():ExternalImportStateVO
		{
			return new ExternalImportStateVO( 	stateName, 				dataName, 				longName, 
												shortName, 				longDescription,		shortDescription,
												externalStateString, 	stateString,  			stateNumber, 	
												toolTip, 				webLink	);
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "State Metadata for " + longName + ":";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ",\t";
			
			string += leadingFormatString + "stateName = " 		+ stateName;
			string += formatString + "dataName = " 				+ dataName;
			string += formatString + "longName = " 				+ longName;
			string += formatString + "shortName = " 			+ shortName;
			string += formatString + "longDescription = " 		+ longDescription;
			string += formatString + "shortDescription = " 		+ shortDescription;
			string += formatString + "externalStateString = " 	+ externalStateString;
			string += formatString + "stateString = " 			+ stateString;
			string += formatString + "stateNumber = " 			+ String( stateNumber );
			string += formatString + "toolTip = " 				+ toolTip;
			string += formatString + "webLink = " 				+ webLink;
			
			return string;
		}
	}
}