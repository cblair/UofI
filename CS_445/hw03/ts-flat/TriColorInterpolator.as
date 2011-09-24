package com.ts.presentation.view.decorationAssets.interpolators
{
	public class TriColorInterpolator extends AbstractColorInterpolator
	{
		private var _min:Number;
		private var _mid:Number;
		private var _ttl:Number;
		private var _prt:Number;
		private var _prcnt:Number;
		
		private var _lR:uint;
		private var _lG:uint;
		private var _lB:uint;
		
		private var _mR:uint;
		private var _mG:uint;
		private var _mB:uint;
		
		private var _hR:uint;
		private var _hG:uint;
		private var _hB:uint;
		
		private var _r:uint;
		private var _g:uint;
		private var _b:uint;

		public function TriColorInterpolator( Min:Number, Mid:Number, Max:Number, LColor:uint, MColor:uint, HColor:uint, resolution:int = 1, defaultColor:uint = 0x000000, roundValue:Boolean = true )
		{
			super( defaultColor, roundValue, resolution );
			
			_ttl = ( Max - Min );
			_mid = ( Mid - Min ) / _ttl;
			_min = Min;
			
			_lR = ( LColor & 0xff0000 ) >> 16;
			_lG = ( LColor & 0x00ff00 ) >> 8;
			_lB = ( LColor & 0x0000ff );
			
			_mR = ( MColor & 0xff0000 ) >> 16;
			_mG = ( MColor & 0x00ff00 ) >> 8;
			_mB = ( MColor & 0x0000ff );
			
			_hR = ( HColor & 0xff0000 ) >> 16;
			_hG = ( HColor & 0x00ff00 ) >> 8;
			_hB = ( HColor & 0x0000ff );
		}
		
		override public function interpolateValueToColor( value:Number ):uint
		{
			value = _roundValue ? Math.round( value ) : value;
			value -= value % resolution;
			
			_prt 	= value - _min;
			_prcnt 	= _prt / _ttl;
			
			if( _prcnt <= _mid )
			{
				_prcnt = _prcnt / _mid;
				_r = ( _mR - _lR ) * _prcnt + _lR;
				_g = ( _mG - _lG ) * _prcnt + _lG;
				_b = ( _mB - _lB ) * _prcnt + _lB;
			}
			else
			{
				_prcnt = ( _prcnt - _mid ) / ( 1 - _mid );
				_r = ( _hR - _mR ) * _prcnt + _mR;
				_g = ( _hG - _mG ) * _prcnt + _mG;
				_b = ( _hB - _mB ) * _prcnt + _mB;
			}
			
			return _r << 16 | _g << 8 | _b;
		}
	}
}