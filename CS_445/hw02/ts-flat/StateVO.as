package com.ts.data.model.vo
{
	import com.ts.data.enumerations.StateAttribute;
	import com.ts.data.enumerations.XMLElement;
	
	import flash.text.StaticText;

	public class StateVO
	{
		public var stateName:String;
		public var longName:String;
		public var shortName:String;
		public var longDescription:String;
		public var shortDescription:String;
		public var stateString:String;
		public var stateNumber:Number;
		public var toolTip:String;
		public var webLink:String;
		
		public function StateVO( 	StateName:String, 		LongName:String, 		ShortName:String, 
									LongDescription:String, ShortDescription:String,StateString:String, 		
									StateNumber:Number, 	ToolTip:String, 		WebLink:String )
		{
			stateName 			= StateName;
			longName 			= LongName;
			shortName 			= ShortName;
			longDescription		= LongDescription;
			shortDescription 	= ShortDescription;
			stateString			= StateString;
			stateNumber			= StateNumber;
			toolTip				= ToolTip;
			webLink				= WebLink;
		}
		
		public function clone():StateVO
		{
			return new StateVO( stateName, 			longName, 			shortName,
								shortDescription, 	longDescription,	stateString,		
								stateNumber,		toolTip,			webLink );
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "State Metadata for " + longName + ":\t";
			var leadingFormatString:String = asList ? "\n " : " ";
			var formatString:String = asList ? "\n " : ",\t";
			
			string += leadingFormatString + "stateName = " 	+ stateName;
			string += formatString + "longName = " 			+ longName;
			string += formatString + "shortName = " 		+ shortName;
			string += formatString + "longDescription = " 	+ longDescription;
			string += formatString + "shortDescription = " 	+ shortDescription;
			string += formatString + "stateString = " 		+ stateString;
			string += formatString + "stateNumber = " 		+ String( stateNumber );
			string += formatString + "toolTip = "			+ toolTip;
			string += formatString + "webLink = " 			+ webLink;
			
			string += "\n";
			
			return string;
		}
	}
}