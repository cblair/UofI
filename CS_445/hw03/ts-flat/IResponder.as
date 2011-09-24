package com.ts.global.interfaces
{
	public interface IResponder
	{
		function fault( error:* ):void;
		function result( value:* ):void;
		function progress( value:* ):void;
	}
}