package com.ts.aspect.model.vo
{
	public class DisplayVO
	{
		public var name:String;
		public var title:String;
		public var emphasisGroup:String;
		public var levelOfDetail:String;
		public var lodID:int;
		public var visible:Boolean;
		public var subdued:Boolean;
		public var emphasized:Boolean;
		public var lodPinned:Boolean;
		public var globalPinned:Boolean;
		
		public function DisplayVO( Name:String, Title:String, EmphasisGroup:String, LevelOfDetail:String, LodID:int, 
								   Visible:Boolean = true, LodPinned:Boolean = false, GlobalPinned:Boolean = false )
		{
			name 			= Name;
			title 			= Title;
			emphasisGroup 	= EmphasisGroup;
			levelOfDetail 	= LevelOfDetail;
			lodID 			= LodID;
			visible 		= Visible;
			lodPinned 		= LodPinned;
			globalPinned 	= GlobalPinned;
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "Display: ";
			
			string += "name = "				+ name 					+ ", ";
			string += "title = "			+ title 				+ ", ";
			string += "emphasisGroup = "	+ emphasisGroup			+ ", ";
			string += "levelOfDetail = "	+ levelOfDetail			+ ", ";
			string += "lodID = "			+ String( lodID ) 		+ ", ";
			string += "visible = "			+ String( visible )		+ ", ";
			string += "lodPinned = "		+ String( lodPinned ) 	+ ", ";
			string += "globalPinned = "		+ String( globalPinned )+ ", ";
			
			string += "\n";
			
			return string;
		}
	}
}