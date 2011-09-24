package com.ts.core.model.vo
{
	import mx.collections.ArrayCollection;

	public class MenuBtnCollectionVO
	{
		private var _array:Array;
		public function MenuBtnCollectionVO()
		{
			_array = new Array();
		}
		
		protected function addMenuBtn( label:String, command:String ):void
		{
			_array.push( new MenuBtnVO( label, command ) );
		}
		
		public function get collection():ArrayCollection
		{
			return new ArrayCollection( _array );
		}
	}
}