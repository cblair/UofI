package com.ts.presentation.model.grid
{
	import com.ts.global.assets.AppAlphaAssets;
	import com.ts.global.assets.ColorAssets;
	import com.ts.presentation.model.PlaybackProxy;
	import com.ts.utilities.StringFormatters;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	
	import mx.binding.utils.BindingUtils;
	import mx.collections.ArrayCollection;
	import mx.geom.RoundedRectangle;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class GridProxy extends Proxy
	{
		public static const NAME:String	= "gridProxy";
		
		public static const UPDATE_PX_PER_SECOND:String 	= "updatePxPerSecond";
		public var eventDispatcher:EventDispatcher;
		
		[Bindable] public var hLines:ArrayCollection;
		[Bindable] public var timeLine:ArrayCollection;
		[Bindable] public var hOffset:int		= 0;
		[Bindable] public var hScrollP:int		= 0;
		[Bindable] public var worldScrollP:int	= 0;
		
		private var _historySpan:int 			= 1;
		private var _historySeconds:Number		= 1;
		private var _currentSpan:int			= 0;
		private var _currentSecond:Number 		= 0;
		private var _timeSpan:int				= 0;
		private var _totalSeconds:Number 		= 1;
		private var _viewportWidth:Number		= 1;
		private var _viewportHeight:Number		= 1;
		private var _hLineWeight:int 			= 1;
		private var _hLineColor:uint 			= ColorAssets.MAIN_EMPHASIS;
		private var _hLineAlpha:Number 			= AppAlphaAssets.GRID_LINE;
		private var _hRuleAlpha:Number			= AppAlphaAssets.GRID_RULE;
		
		private var _pxPerSecond:Number			= 10;
		private var _secondsPerLine:int 		= 1;
		private var _linesPerRule:int 			= 5;
		private var _hasRuleLines:Boolean		= true;
		
		private var _pxPerLine:int				= 10;
		private var _pxPerRule:int				= 50;
		private var _currentPxPos:int			= 0;
		
		private var _invalidHLines:Boolean = false;
		private var _invalidHOffset:Boolean = false;
		
		private var _setViewportWidth:Boolean = true;
		[Bindable]
		public function get viewportWidth():int{ return _viewportWidth; }
		public function set viewportWidth( value:int ):void
		{
			if( _setViewportWidth )
			{
				_viewportWidth = value;
				
				_setViewportWidth = false;
				invalidateHLines();
				invalidateTimeLine();
				invalidateHOffset();
				invalidateHScrollPos();
				_setViewportWidth = true;
			}
		}
		
		private var _setViewportHeight:Boolean = true;
		[Bindable]
		public function get viewportHeight():int{ return _viewportHeight; }
		public function set viewportHeight( value:int ):void
		{
			if( _setViewportHeight )
			{
				_viewportHeight = value;
				
				_setViewportHeight = false;
				invalidateHOffset();
				invalidateHLines();
				_setViewportHeight = true;
			}
		}
		
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
				updateCurrentPxPos();
				invalidateHOffset();
				invalidateHScrollPos();
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
		public function get currentSpan():int{ return _currentSpan; }
		public function set currentSpan( value:int ):void
		{
			if( _setCurrentSpan )
			{
				_currentSpan = value;
				//trace( "currentSpan = " + value );
				
				_setCurrentSpan = false;
				currentSecond = value / _pxPerSecond;
				updateCurrentPxPos();
				invalidateHScrollPos();
				invalidateHOffset();
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
		public function get pxPerSecond():Number{ return _pxPerSecond; }
		public function set pxPerSecond( value:Number ):void
		{
			if( _setPxPerSecond )
			{
				_pxPerSecond = value;
				_pxPerLine = value * _secondsPerLine;
				_pxPerRule = _pxPerLine * _linesPerRule;
				
				_setPxPerSecond = false;
				_setHistorySeconds = false;
				_setCurrentSecond = false;
				_setTotalSeconds = false;
				historySpan = historySeconds * value;
				currentSpan = currentSecond * value;
				timeSpan 	= totalSeconds  * value;
				invalidateHLines();
				_setPxPerSecond = true;
				_setHistorySeconds = true;
				_setCurrentSecond = true;
				_setTotalSeconds = true;
				
				eventDispatcher.dispatchEvent( new Event( UPDATE_PX_PER_SECOND ) );
			}
		}
		
		private var _setSecondsPerLine:Boolean = true;
		[Bindable]
		public function get secondsPerLine():int{ return _secondsPerLine; }
		public function set secondsPerLine( value:int ):void
		{
			if( _setSecondsPerLine )
			{
				_secondsPerLine = value;
				_pxPerLine = value * _pxPerSecond;
				_pxPerRule = _pxPerLine * _linesPerRule;
				
				_setSecondsPerLine = false;
				invalidateHOffset();
				invalidateHScrollPos();
				invalidateHLines();
				invalidateTimeLine();
				_setSecondsPerLine = true;
			}
		}
		
		private var _setLinesPerRule:Boolean = true;
		[Bindable]
		public function get linesPerRule():int{ return _linesPerRule; }
		public function set linesPerRule( value:int ):void
		{
			if( _setLinesPerRule )
			{
				_linesPerRule = Math.max( value, 0 );
				
				_setLinesPerRule = false;
				if( value == 0 ) hasRuleLines = false;
				else hasRuleLines = true;
				_setLinesPerRule = true;
			}
		}
		
		private var _setHasRuleLines:Boolean = true;
		[Bindable]
		public function get hasRuleLines():Boolean{ return _hasRuleLines; }
		public function set hasRuleLines( value:Boolean ):void
		{
			if( _setHasRuleLines )
			{
				_hasRuleLines = value;
				
				_setHasRuleLines = false;
				if( _linesPerRule == 0 )
				{
					linesPerRule = 1;
				}
				invalidateHOffset();
				invalidateHScrollPos();
				invalidateHLines();
				invalidateTimeLine();
				_setHasRuleLines = true;
			}
		}	
		
		private var _setTimeSpan:Boolean = true;
		[Bindable]
		public function get timeSpan():int{ return _timeSpan; }
		public function set timeSpan( value:int ):void
		{
			if( _setTimeSpan )
			{
				_timeSpan = value;
				
				_setTimeSpan = false;
				totalSeconds = value / _pxPerSecond;
				invalidateTimeLine();
				invalidateHScrollPos();
				_setTimeSpan = true;
			}
		}
		
		private var _setTotalSeconds:Boolean = true;
		[Bindable]
		public function get totalSeconds():Number{ return _totalSeconds; }
		public function set totalSeconds( value:Number ):void
		{
			if( _setTotalSeconds )
			{
				_totalSeconds = value;
				
				_setTotalSeconds = false;
				timeSpan = value * _pxPerSecond;
				_setTotalSeconds = true;
			}
		}
		
		
		
		public function GridProxy( name:String = "" )
		{
			super( NAME + name );
			historySpan = 1;
			currentSpan = 1;
			eventDispatcher = new EventDispatcher();
		}
		
		public function styleHLines( lineWeight:int, lineColor:uint, lineAlpha:Number, ruleAlpha:Number ):void
		{
			_hLineWeight 	= lineWeight;
			_hLineColor 	= lineColor;
			_hLineAlpha 	= lineAlpha;
			_hRuleAlpha 	= ruleAlpha;
			
			invalidateHLines();
		}
		
		public function getCurrentSecondFromSpan( span:int ):Number
		{
			return currentSpan / _pxPerSecond;
		}
		
		private function invalidateHOffset():void
		{
			var gridSectionWidth:int = _hasRuleLines ? _linesPerRule * _pxPerLine : _pxPerLine; 
			//hOffset = ( ( _currentSpan + _viewportWidth - _historySpan ) % gridSectionWidth );
			hOffset = ( gridSectionWidth - ( _historySpan % gridSectionWidth ) + _currentSpan ) % gridSectionWidth;
		}
		
		private function invalidateHLines():void
		{
			// 1. Determine How many lines are needed
			var numLines:int = Math.ceil( _viewportWidth / _pxPerLine ) + ( _hasRuleLines ? _linesPerRule : 1 );
			
			// 2. Determine the lineGap
			var workingGap:int = _pxPerLine - _hLineWeight;
			
			// 3. create new hLines Collection
			var lines:Array = new Array();
			
			if( _hasRuleLines )
			{
				var workingAlpha:Number;

				for( var i:int = 1; i < numLines + 1; i++ )
				{
					if( i % _linesPerRule == 0 )
					{
						workingAlpha = _hRuleAlpha;
					}
					else
					{
						workingAlpha = _hLineAlpha;
					}
					
					lines.push( new GridLineVO( _hLineWeight, workingGap, _hLineColor, workingAlpha, _viewportHeight ) );
				}
			}
			else
			{
				for( i = 0; i < numLines; i++ )
				{
					lines.push( new GridLineVO( _hLineWeight, workingGap, _hLineColor, _hLineAlpha, _viewportHeight ) );
				}
			}
			hLines = new ArrayCollection( lines );
			//trace( "invalidatedLines, width = " + viewportWidth + ", height = " + viewportHeight + ", numLines = " + numLines );
			invalidateTimeLine();
		}
		
		private function invalidateTimeLine():void
		{
			if( _hasRuleLines )
			{
				var ruleSeconds:Number = _pxPerRule / _pxPerSecond;
				var roundedViewportWidth:int = Math.ceil( _viewportWidth / _pxPerRule );
				var roundedTimeSpan:int = Math.ceil( _timeSpan / _pxPerRule );
				
				var numRuleLines:int = roundedTimeSpan + roundedViewportWidth * 2;
				
				roundedTimeSpan *= _pxPerRule;
				roundedViewportWidth *= _pxPerRule;
				
				var times:Array = new Array();
				var runningSeconds:Number = 0;
				
				for( var i:int = 0; i < numRuleLines; i++ )
				{
					if( i * _pxPerRule < roundedViewportWidth )
					{
						times.push( new TimeLineVO( "", _pxPerRule ) );
					}
					else if( i * _pxPerRule <= roundedViewportWidth + roundedTimeSpan )
					{
						times.push( new TimeLineVO( StringFormatters.secondsToMMSS( String( runningSeconds ) ), _pxPerRule ) );
						runningSeconds += ruleSeconds;
					}
					else
					{
						times.push( new TimeLineVO( "", _pxPerRule ) );
					}
				}
				timeLine = new ArrayCollection( times );
			}
			else
			{
				timeLine = new ArrayCollection( [] );
			}
		}
		
		private function invalidateHScrollPos():void
		{
			var roundedViewportWidth:int = Math.ceil( _viewportWidth / _pxPerRule ) * _pxPerRule;
			
			hScrollP = _currentSpan + roundedViewportWidth - _historySpan;
			worldScrollP = _currentSpan - _historySpan;
		}
		
		private function updateCurrentPxPos():void
		{
			_currentPxPos = _historySpan + _currentSpan; 	
		}
	}
}