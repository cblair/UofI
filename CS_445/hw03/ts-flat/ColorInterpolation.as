package com.ts.utilities.colorRanges
{
	import flash.net.LocalConnection;
	
	import mx.rpc.events.HeaderEvent;

	public class ColorInterpolation
	{
		public static const CONSTANT:String			= "constant";
		public static const RANGE:String			= "range";
		public static const MID_RANGE:String		= "midRange";
		public static const DISCRETE_RANGE:String	= "discreteRange";
		public static const COLOR_MAP:String		= "colorMap";
		
		public static const DEF_MIN:Number		= 0;
		public static const DEF_MAX:Number		= 1;
		public static const DEF_MID:Number		= .5;
		
		public static const DEF_COLOR:uint		= 0x777777;
		
		private static var _ttl:Number;
		private static var _prt:Number;
		private static var _prcnt:Number;
		
		private static var _lA:uint;
		private static var _lR:uint;
		private static var _lG:uint;
		private static var _lB:uint;
		
		private static var _mA:uint;
		private static var _mR:uint;
		private static var _mG:uint;
		private static var _mB:uint;
		
		private static var _hA:uint;
		private static var _hR:uint;
		private static var _hG:uint;
		private static var _hB:uint;
		
		private static var _a:uint;
		private static var _r:uint;
		private static var _g:uint;
		private static var _b:uint;
		

		
		public static function interpolate3ColorARGB( Value:Number, Min:Number, Mid:Number, Max:Number, LColor:uint, MColor:uint, HColor:uint, LAlpha:Number, MAlpha:Number, HAlpha:Number ):uint
		{
			_ttl 	= Max - Min;
			_prt 	= Value - Min;
			_prcnt 	= _prt / _ttl;
			
			Mid 	= Mid * _ttl;
			
			_lA = LAlpha * 255;
			_lR = ( LColor & 0xff0000 ) >> 16;
			_lG = ( LColor & 0x00ff00 ) >> 8;
			_lB = ( LColor & 0x0000ff );
			
			_mA = MAlpha * 255;
			_mR = ( MColor & 0xff0000 ) >> 16;
			_mG = ( MColor & 0x00ff00 ) >> 8;
			_mB = ( MColor & 0x0000ff );
			
			_hA = HAlpha * 255;
			_hR = ( HColor & 0xff0000 ) >> 16;
			_hG = ( HColor & 0x00ff00 ) >> 8;
			_hB = ( HColor & 0x0000ff );
			
			if( _prcnt <= Mid )
			{
				_prcnt = _prcnt / Mid;
				_a = ( _mA - _lA ) * _prcnt + _lA;
				_r = ( _mR - _lR ) * _prcnt + _lR;
				_g = ( _mG - _lG ) * _prcnt + _lG;
				_b = ( _mB - _lB ) * _prcnt + _lB;
			}
			else
			{
				_prcnt = ( _prcnt - Mid ) / ( 1 - Mid );
				_a = ( _hA - _mA ) * _prcnt + _mA;
				_r = ( _hR - _mR ) * _prcnt + _mR;
				_g = ( _hG - _mG ) * _prcnt + _mG;
				_b = ( _hB - _mB ) * _prcnt + _mB;
			}
			
			return _a << 32 | _r << 16 | _g << 8 | _b;
		}
		
		public static function interpolate3Alphas( Value:Number, Min:Number, Mid:Number, Max:Number, LAlpha:Number, MAlpha:Number, HAlpha:Number ):uint
		{
			_ttl 	= Max - Min;
			_prt 	= Value - Min;
			_prcnt 	= _prt / _ttl;
			
			Mid 	= Mid * _ttl;
			
			_lA = LAlpha;
			_mA = MAlpha;
			_hA = HAlpha;
			
			if( _prcnt <= Mid )
			{
				_prcnt = _prcnt / Mid;
				return ( _mA - _lA ) * _prcnt + _lA;
			}
			else
			{
				_prcnt = ( _prcnt - Mid ) / ( 1 - Mid );
				return ( _hA - _mA ) * _prcnt + _mA;
			}
		}
		
		public static function interpolate3ColorRGB( Min:Number, Value:Number, Max:Number, LColor:uint, MColor:uint, HColor:uint, Mid:Number = .5 ):uint
		{
			_ttl 	= Max - Min;
			_prt 	= Value - Min;
			_prcnt 	= _prt / _ttl;
			
			Mid 	= Mid * _ttl;
			
			_lR = ( LColor & 0xff0000 ) >> 16;
			_lG = ( LColor & 0x00ff00 ) >> 8;
			_lB = ( LColor & 0x0000ff );
			
			_mR = ( MColor & 0xff0000 ) >> 16;
			_mG = ( MColor & 0x00ff00 ) >> 8;
			_mB = ( MColor & 0x0000ff );
			
			_hR = ( HColor & 0xff0000 ) >> 16;
			_hG = ( HColor & 0x00ff00 ) >> 8;
			_hB = ( HColor & 0x0000ff );

			if( _prcnt <= Mid )
			{
				_prcnt = _prcnt / Mid;
				_r = ( _mR - _lR ) * _prcnt + _lR;
				_g = ( _mG - _lG ) * _prcnt + _lG;
				_b = ( _mB - _lB ) * _prcnt + _lB;
			}
			else
			{
				_prcnt = ( _prcnt - Mid ) / ( 1 - Mid );
				_r = ( _hR - _mR ) * _prcnt + _mR;
				_g = ( _hG - _mG ) * _prcnt + _mG;
				_b = ( _hB - _mB ) * _prcnt + _mB;
			}
			
			return _r << 16 | _g << 8 | _b;
		}
		
		public static function interpolate2ColorARBG( Min:Number, Value:Number, Max:Number, LColor:uint, HColor:uint, LAlpha:Number, HAlpha:Number ):uint
		{
			_ttl 	= Max - Min;
			_prt 	= Value - Min;
			_prcnt 	= _prt / _ttl;
			
			_lA 	= LAlpha * 255;
			_lR		= LColor >> 16 & 0xFF;
			_lG 	= LColor >> 8 & 0xFF;
			_lB 	= LColor & 0xFF;
			
			_hA		= HAlpha * 255;
			_hR 	= HColor >> 16 & 0xFF;
			_hG 	= HColor >> 8 & 0xFF;
			_hB 	= HColor & 0xFF;
			
			_a = ( _hA - _lA ) * _prcnt + _lA;
			_r = ( _hR - _lR ) * _prcnt + _lR;
			_g = ( _hG - _lG ) * _prcnt + _lG;
			_b = ( _hB - _lB ) * _prcnt + _lB;
			
			return _a << 32 | _r << 16 | _g << 8 | _b;
		}
		
		public static function interpolate2ColorRBG( Min:Number, Value:Number, Max:Number, LColor:uint, HColor:uint ):uint
		{
			_ttl 	= Max - Min;
			_prt 	= Value - Min;
			_prcnt 	= _prt / _ttl;
			
			_lR		= LColor >> 16 & 0xFF;
			_lG 	= LColor >> 8 & 0xFF;
			_lB 	= LColor & 0xFF;

			_hR 	= HColor >> 16 & 0xFF;
			_hG 	= HColor >> 8 & 0xFF;
			_hB 	= HColor & 0xFF;
	
			_r = ( _hR - _lR ) * _prcnt + _lR;
			_g = ( _hG - _lG ) * _prcnt + _lG;
			_b = ( _hB - _lB ) * _prcnt + _lB;
			
			return _r << 16 | _g << 8 | _b;
		}
		
		public static function interpolate2Alphas( Min:Number, Value:Number, Max:Number, LAlpha:Number, HAlpha:Number ):Number
		{
			_ttl 	= Max - Min;
			_prt 	= Value - Min;
			_prcnt 	= _prt / _ttl;
			
			return ( HAlpha - LAlpha ) * _prcnt + LAlpha;
		}
	}
}