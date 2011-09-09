package com.ts.global.interfaces
{
	public interface IDelegate
	{
		function get responder():IResponder;
		function set responder( value:IResponder ):void;
		function delegate( key:*, data:* = null ):void;
	}
}