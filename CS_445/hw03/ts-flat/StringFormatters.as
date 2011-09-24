package com.ts.utilities
{
	public class StringFormatters
	{
		public function StringFormatters()
		{
		}
		
		public static function decimalToHex(decimal:uint, hexFormat:Boolean = true):String 
		{			
			var digits:String = "0123456789ABCDEF";
			var hex:String = '';
			
			while (decimal > 0) {
				
				var next:uint = decimal & 0xF;
				decimal >>= 4;
				hex = digits.charAt(next) + hex;
				
			}
			
			if (hex.length == 0) hex = '0';
			
			if( hexFormat )
			{
				hex = "0x" + hex;
			}
			else
			{
				hex = "#" + hex;
			}
				
			return hex;
			
		}
		
		public static function getDecimalPercision( rawString:String, percision:int = 2 ):String
		{
			var decimal:int = rawString.indexOf( "." )
			
			if( decimal == -1 && percision > 0 )
			{
				rawString += "."
				
				for ( var i:int = 0; i < percision; i++ )
				{
					rawString += "0"
				}
			}
			else
			{
				if( rawString.length - ( decimal + 1 ) > percision ) //-- MORE DECIMALS THAN WANTED
				{
					rawString = rawString.substr( 0, decimal + percision + 1 )
				}
				else
				{
					var length:int = percision - ( rawString.length - ( decimal + 1 ) )
					for ( i = 0; i < length; i++ )
					{
						rawString += "0"
					}
				}
			}
			
			return rawString
		}
		
		public static function fixedNumber( value:Number, percision:int = 2 ):Number
		{
			trace( Number( getDecimalPercision( String( value ), percision ) ) );
			return Number( getDecimalPercision( String( value ), percision ) );
		}
		
		public static function secondsToMMSS( rawString:String ):String
		{
			var minutes:Number = 0;
			var seconds:Number = 0;
			
			rawString = rawString.split( "." )[ 0 ];
			
			seconds = int( rawString );
			
			minutes = Math.floor( seconds / 60 );
			
			seconds = seconds%60;
			
			
			var timecode:String 
			if( minutes < 10 )
			{
				timecode = "0" + minutes + ":";
			}
			else
			{
				timecode = minutes + ":";
			}
			if( seconds < 10 )
			{
				timecode = timecode + "0" + seconds;
			}
			else
			{
				timecode = timecode + seconds;
			}
			return timecode;
		}
		
		public static function getTimePercision( rawString:String, percision:int = 2 ):String
		{
			var minutes:Number = 0;
			var seconds:Number = 0;
			var miliseconds:Number = 0;
			
			var decimal:int = rawString.indexOf( "." )
			
			if( decimal == -1 )
			{
				rawString += "."
				
				for ( var i:int = 0; i < percision; i++ )
				{
					rawString += "0"
				}
			}
			else
			{
				if( rawString.length - ( decimal + 1 ) > percision ) //-- MORE DECIMALS THAN WANTED
				{
					rawString = rawString.substr( 0, decimal + percision + 1 )
				}
				else
				{
					var length:int = percision - ( rawString.length - ( decimal + 1 ) )
					for ( i = 0; i < length; i++ )
					{
						rawString += "0"
					}
				}
			}
			decimal = rawString.indexOf( "." )
			
			miliseconds = int(rawString.substr( decimal + 1 ));
			
			seconds = int(rawString.substr( 0, decimal ));
			
			minutes = Math.floor( seconds / 60 );
			
			seconds = seconds%60;
			

			var timecode:String 
			if( minutes < 10 )
			{
				timecode = "0" + minutes + ":";
			}
			else
			{
				timecode = minutes + ":";
			}
			if( seconds < 10 )
			{
				timecode = timecode + "0" + seconds + ":";
			}
			else
			{
				timecode = timecode + seconds + ":";
			}
			if( miliseconds < 10 )
			{
				timecode = timecode + "0" + miliseconds;
			}
			else
			{
				timecode = timecode + miliseconds;
			}
			
			
			return timecode;
		}
	}
}