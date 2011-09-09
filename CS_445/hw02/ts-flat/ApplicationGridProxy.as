package com.ts.presentation.model.grid
{
	import org.puremvc.as3.patterns.proxy.Proxy;

	public class ApplicationGridProxy extends Proxy
	{
		public static const NAME:String	= "applicationGridProxy";
		
		private var _historySpan:int 			= 10;
		private var _historySeconds:Number		= 10;
		private var _currentSpan:Number			= 0;
		private var _currentSecond:Number 		= 0;
		private var _totalSeconds:Number 		= 1;
		
		private var _pxPerSecond:int 			= 10;

		private var _currentPxPos:int			= 0;
		private var _timeSpan:int				= 0;
		private var _elapsedSpan:int			= 0;
		private var _totalSpan:int				= 0;
		private var _futureSpan:int				= 0;
		
		private var _setHistorySpan:Boolean = true;
		[Bindable]
		public function get historySpan():int{ return _historySpan; }
		public function set historySpan( value:int ):void
		{
			if( _setHistorySpan )
			{
				_historySpan = value;
				
				_setHistorySpan = false;
				historySeconds = value / _pxPerSecond;
				_setHistorySpan = true;
			}
		}
		
		private var _setHistorySeconds:Boolean = true;
		[Bindable]
		public function get historySeconds():Number{ return _historySeconds; }
		public function set historySeconds( value:Number ):void
		{
			if( _setHistorySeconds )
			{
				_historySeconds = value;
				
				_setHistorySeconds = false;
				historySpan = value * _pxPerSecond;
				_setHistorySeconds = true;
			}
		}
		
		private var _setCurrentSpan:Boolean = true;
		[Bindable]
		public function get currentSpan():Number{ return _currentSpan; }
		public function set currentSpan( value:Number ):void
		{
			if( _setCurrentSpan )
			{
				_currentSpan = value;
				
				_setCurrentSpan = false;
				currentSecond = value / _pxPerSecond;
				_setCurrentSpan = true;
			}
		}
		
		private var _setCurrentSecond:Boolean = true;
		[Bindable]
		public function get currentSecond():Number{ return _currentSecond; }
		public function set currentSecond( value:Number ):void
		{
			if( _setCurrentSecond )
			{
				_currentSecond = value;
				
				_setCurrentSecond = false;
				currentSpan = value * _pxPerSecond;
				_setCurrentSecond = true;
			}
		}
		
		private var _setPxPerSecond:Boolean = true;
		[Bindable]
		public function get pxPerSecond():int{ return _pxPerSecond; }
		public function set pxPerSecond( value:int ):void
		{
			if( _setPxPerSecond )
			{
				_pxPerSecond = value;
				
				_setPxPerSecond = false;
				_setHistorySeconds = false;
				_setCurrentSecond = false;
				historySpan = Math.ceil( historySeconds * value );
				currentSpan = Math.ceil( currentSecond * value );
				_setPxPerSecond = true;
				_setHistorySeconds = true;
				_setCurrentSecond = true;
			}
		}
		
		[Bindable]
		public function get totalSeconds():Number{ return _totalSeconds; }
		public function set totalSeconds( value:Number ):void
		{
			_totalSeconds = value;
		}
		
		public function ApplicationGridProxy()
		{
			super( NAME );
		}
	}
}