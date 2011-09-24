package com.ts.utilities.colorRanges
{
	import mx.graphics.GradientEntry;
	import mx.graphics.GradientStroke;
	import mx.graphics.IFill;
	import mx.graphics.IStroke;
	import mx.graphics.LinearGradient;
	import mx.graphics.LinearGradientStroke;
	import mx.graphics.SolidColor;
	import mx.graphics.SolidColorStroke;

	public class MidRangeInterpolator extends AbstractColorInterpolator
	{
		public var iFunction = ColorInterpolation.interpolate3ColorARGB;
		public var lColor:uint;
		public var mColor:uint;
		public var hColor:uint;
		public var lAlpha:Number;
		public var mAlpha:Number;
		public var hAlpha:Number;
		public var minVal:Number;
		public var maxVal:Number;
		public var midPrcnt:Number;
		public var gradient:LinearGradient;
		public var gradientStroke:LinearGradientStroke;
		
		public function MidRangeInterpolator( LColor:uint, 		MColor:uint, 	HColor:uint, 
											  LAlpha:Number, 	MAlpha:Number, 	HAlpha:Number, 
											  MinVal:Number, 	MaxVal:Number, 	MidPrcnt:Number )
		{
			lColor 		= LColor;
			hColor 		= HColor;
			mColor 		= MColor;
			lAlpha		= LAlpha;
			mAlpha		= MAlpha;
			hAlpha		= HAlpha;
			minVal 		= MinVal;
			maxVal 		= MaxVal;
			midPrcnt 	= MidPrcnt;
			
			gradient = new LinearGradient();
			gradient.angle = 90;
			gradient.entries = [ new GradientEntry( lColor, 0, lAlpha ),
								 new GradientEntry( mColor, midPrcnt, mAlpha ),
								 new GradientEntry( hColor, 1, hAlpha ) ];
			
			gradientStroke = new GradientStroke();
			gradientStroke.angle = 90;
			gradientStroke.entries = [ new GradientEntry( lColor, 0, lAlpha ),
									   new GradientEntry( mColor, midPrcnt, mAlpha ),
									   new GradientEntry( hColor, 1, hAlpha ) ];
		}
		
		override public function getColor( value:Number ):uint
		{
			return iFunction( minVal, value, maxVal, lColor, mColor, hColor, lAlpha, mAlpha, hAlpha, midPrcnt );
		}
		
		override public function getHFill( value:Number ):IFill
		{
			return new SolidColor( getColor( value ) );
		}
		
		override public function getVFill( value:Number ):IFill
		{
			return gradient;
		}
		
		override public function getHStroke( value:Number ):IStroke
		{
			return new SolidColorStroke( getColor( value ) );
		}
		
		override public function getVStroke( value:Number ):IStroke
		{
			return gradientStroke;
		}
		
		override public function toString( asList:Boolean=true ):String
		{
			return "";
		}
	}
}