package com.ts.aspect.model.graph.plot
{
	import com.ts.aspect.model.DecorationEntry;
	import com.ts.aspect.model.graph.plot.vo.PlotDecorationVO;
	import com.ts.utilities.colorRanges.MidRangeInterpolator;
	
	public class MidRangePlotDecorator extends PlotDecorator
	{
		private var _maxEntry:DecorationEntry;
		private var _midEntry:DecorationEntry;
		private var _minEntry:DecorationEntry;
		
		private var _min:Number;
		private var _mid:Number;
		private var _max:Number;
		
		private var _lArgb:Array;
		private var _mArgb:Array;
		private var _hArgb:Array;
		private var _newArgb:Array
		
		private var _ttl:Number;
		private var _prt:Number;
		private var _prcnt:Number;
		private var _midPrcnt:Number;
		
		public function MidRangePlotDecorator( Entries:Array )
		{
			_maxEntry = Entries[ 0 ];
			_midEntry = Entries[ 1 ];
			_minEntry = Entries[ 2 ];
			
			_min = minEntry.value;
			_mid = midEntry.value;
			_max = maxEntry.value;
			
			_ttl = _max - _min;
			_midPrcnt = _mid / _ttl;
			
			_lArgb = new Array();
			_mArgb = new Array();
			_hArgb = new Array();
			_newArgb = new Array();
			
			super( Entries );
		}
		
		override protected function refactorDecorator():void
		{
			_lArgb[ 0 ] = _minEntry.currentAlpha;
			_lArgb[ 1 ] = ( _minEntry.currentColor & 0xFF0000 ) >> 16;
			_lArgb[ 2 ] = ( _minEntry.currentColor & 0x00FF00 ) >> 8;
			_lArgb[ 3 ] = ( _minEntry.currentColor & 0x0000FF );
			
			_mArgb[ 0 ] = _midEntry.currentAlpha;
			_mArgb[ 1 ] = ( _midEntry.currentColor & 0xFF0000 ) >> 16;
			_mArgb[ 2 ] = ( _midEntry.currentColor & 0x00FF00 ) >> 8;
			_mArgb[ 3 ] = ( _midEntry.currentColor & 0x0000FF );
			
			_hArgb[ 0 ] = _maxEntry.currentAlpha;
			_hArgb[ 1 ] = ( _maxEntry.currentColor & 0xFF0000 ) >> 16;
			_hArgb[ 2 ] = ( _maxEntry.currentColor & 0x00FF00 ) >> 8;
			_hArgb[ 3 ] = ( _maxEntry.currentColor & 0x0000FF );
		}
		
		override public function getDecoration( value:Number ):PlotDecorationVO
		{
			value = Math.max( _min, Math.min( _max, value ) );
			
			_prt 	= value - _min;
			_prcnt 	= _prt / _ttl;
			
			if( _prcnt <= _midPrcnt )
			{
				_prcnt = _prcnt / _midPrcnt;
				
				_newArgb[ 0 ] = ( _mArgb[ 0 ] - _lArgb[ 0 ] ) * _prcnt + _lArgb[ 0 ];
				_newArgb[ 1 ] = ( _mArgb[ 1 ] - _lArgb[ 1 ] ) * _prcnt + _lArgb[ 1 ];
				_newArgb[ 2 ] = ( _mArgb[ 2 ] - _lArgb[ 2 ] ) * _prcnt + _lArgb[ 2 ];
				_newArgb[ 3 ] = ( _mArgb[ 3 ] - _lArgb[ 3 ] ) * _prcnt + _lArgb[ 3 ];
				
				if( value == _mid )
				{
					return new PlotDecorationVO( ( _newArgb[ 1 ] << 16 | _newArgb[ 2 ] << 8 | _newArgb[ 3 ] ), _newArgb[ 0 ], _midEntry.tooltip, _midEntry.weblink );
				}
				return new PlotDecorationVO( ( _newArgb[ 1 ] << 16 | _newArgb[ 2 ] << 8 | _newArgb[ 3 ] ), _newArgb[ 0 ], _minEntry.tooltip, _minEntry.weblink );
			}
			else
			{
				_prcnt = ( _prcnt - _midPrcnt ) / ( 1 - _midPrcnt );
				
				_newArgb[ 0 ] = ( _hArgb[ 0 ] - _mArgb[ 0 ] ) * _prcnt + _mArgb[ 0 ];
				_newArgb[ 1 ] = ( _hArgb[ 1 ] - _mArgb[ 1 ] ) * _prcnt + _mArgb[ 1 ];
				_newArgb[ 2 ] = ( _hArgb[ 2 ] - _mArgb[ 2 ] ) * _prcnt + _mArgb[ 2 ];
				_newArgb[ 3 ] = ( _hArgb[ 3 ] - _mArgb[ 3 ] ) * _prcnt + _mArgb[ 3 ];
				
				if( value == _max )
				{
					return new PlotDecorationVO( ( _newArgb[ 1 ] << 16 | _newArgb[ 2 ] << 8 | _newArgb[ 3 ] ), _newArgb[ 0 ], _maxEntry.tooltip, _maxEntry.weblink );
				}
				return new PlotDecorationVO( ( _newArgb[ 1 ] << 16 | _newArgb[ 2 ] << 8 | _newArgb[ 3 ] ), _newArgb[ 0 ], _midEntry.tooltip, _midEntry.weblink );
			}
		}
	}
}