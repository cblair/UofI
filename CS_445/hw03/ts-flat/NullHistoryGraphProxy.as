package com.ts.aspect.development
{
	public class NullHistoryGraphProxy extends HistoryGraphProxy
	{
		public function NullHistoryGraphProxy( Label:HistoryLabel )
		{
			super( Label, null, null );
		}
	}
}