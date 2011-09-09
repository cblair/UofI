package com.ts.aspect.model
{
	import com.ts.aspect.model.vo.DecorationVO;
	
	import org.puremvc.as3.patterns.proxy.Proxy;
	
	public class DecorationProxy extends Proxy
	{
		public static const NAME:String = "decoratedProxy";
		
		public static const NOTE_DECORATION_CHANGED:String		= NAME + "/note/decorationChanged";
		
		public var decoration:DecorationVO;

		private var _currentAlpha:Number;
		private var _currentColor:uint;
		
		public function get currentAlpha():Number{ return _currentAlpha; }
		public function get currentColor():uint{ return _currentColor; }
		
		public function get subduedAlpha()	:Number{ return decoration.subdueAlpha; }
		public function get subduedColor()	:uint  { return decoration.subdueColor; }
		public function get alpha()			:Number{ return decoration.alpha; }
		public function get color()			:uint  { return decoration.color; }
		public function get emphasisAlpha()	:Number{ return decoration.emphasisAlpha; }
		public function get emphasisColor()	:uint  { return decoration.emphasisColor; }
		public function get subdued()		:Boolean{ return decoration.subdued; }
		public function get emphasized()	:Boolean{ return decoration.emphasized; }
		public function get tooltip()		:String{ return decoration.tooltip; }
		public function get weblink()		:String{ return decoration.weblink; }
		
		public function DecorationProxy( name:String, Decoration:DecorationVO )
		{
			decoration = Decoration;
			
			super( name );
			
			if( Decoration.subdued ) subdue();
			else if( Decoration.emphasized ) emphasize();
			else standardize();
		}
		
		public function toString( asList:Boolean = true ):String
		{
			var string:String = "--[ " + proxyName + " ] -- \n";
			
			string += decoration.toString( asList );
			string += "\n";
			
			return string;
		}
		
		public function subdue( notify:Boolean = false ):void
		{
			decoration.subdued = true;
			decoration.emphasized = false;
			_currentAlpha = decoration.subdueAlpha;
			_currentColor = decoration.subdueColor;
			if( notify )updateDecoration();
		}
		
		public function emphasize( notify:Boolean = false ):void
		{
			decoration.emphasized = true;
			decoration.subdued = false;
			_currentAlpha = decoration.emphasisAlpha;
			_currentColor = decoration.emphasisColor;
			if( notify )updateDecoration();
		}
		
		public function standardize( notify:Boolean = false ):void
		{
			decoration.emphasized = false;
			decoration.subdued = false;
			_currentAlpha = decoration.alpha;
			_currentColor = decoration.color;
			if( notify )updateDecoration();
		}
		
		protected function updateDecoration():void
		{
			sendNotification( NOTE_DECORATION_CHANGED, [ _currentAlpha, _currentColor ] );
		}
	}
}