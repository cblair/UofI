package com.ts.global.view.components
{
	import com.ts.core.model.vo.MenuBtnVO;
	
	import flashx.textLayout.formats.TextAlign;
	
	import spark.components.Button;
	
	public class MenuButton extends Button
	{
		private var _menuBtnVO:MenuBtnVO;
		public function MenuButton()
		{
			super();
		}
		
		public function get menuBtnVO():MenuBtnVO{ return _menuBtnVO; }
		public function set menuBtnVO( value:MenuBtnVO ):void
		{
			_menuBtnVO = value;
			label = value.label;
		}
	}
}