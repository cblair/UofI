package com.ts.aspect.model.vo
{
	import com.ts.utilities.StringFormatters;

	public class DecorationVO
	{
		public var subdueAlpha:Number;
		public var subdueColor:uint;
		public var alpha:Number;
		public var color:uint;
		public var emphasisAlpha:Number;
		public var emphasisColor:uint;
		public var subdued:Boolean;
		public var emphasized:Boolean;
		public var tooltip:String;
		public var weblink:String;
		
		public function DecorationVO( SubdueColor:uint, SubdueAlpha:Number, Color:uint, Alpha:Number, EmphasisColor:uint, EmphasisAlpha:uint, 
									  Subdued:Boolean,	Emphasized:Boolean,	Tooltip:String, Weblink:String )
		{
			subdueAlpha 	= SubdueAlpha;
			subdueColor 	= SubdueColor;
			alpha 			= Alpha;
			color 			= Color;
			emphasisColor 	= EmphasisColor;
			emphasisAlpha 	= EmphasisAlpha;
			subdued			= Subdued;
			emphasized		= Emphasized;
			tooltip			= Tooltip;
			weblink			= Weblink;
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "Decoration: ";
			
			string += "subdueAlpha = "		+ String( subdueAlpha ) 						+ ", ";
			string += "subdueColor = "		+ StringFormatters.decimalToHex( subdueColor ) 	+ ", ";
			string += "alpha = "			+ String( alpha ) 								+ ", ";
			string += "color = "			+ StringFormatters.decimalToHex( color ) 		+ ", ";
			string += "emphasisAlpha = "	+ String( emphasisAlpha ) 						+ ", ";
			string += "emphasisColor = "	+ StringFormatters.decimalToHex( emphasisColor )+ ", ";
			string += "subdued = "			+ subdued										+ ", ";
			string += "emphasized = "		+ emphasized									+ ", ";
			string += "tooltip = "			+ tooltip										+ ", ";
			string += "weblink = "			+ weblink										+ ", ";
			
			string += "\n";
			
			return string;
		}
	}
}