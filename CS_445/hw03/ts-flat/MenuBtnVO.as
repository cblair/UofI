package com.ts.core.model.vo
{
	[Bindable]
	public class MenuBtnVO
	{
		public var label:String;
		public var command:String;
		
		public function MenuBtnVO( Label:String, Command:String )
		{
			label = Label;
			command = Command;
		}
	}
}