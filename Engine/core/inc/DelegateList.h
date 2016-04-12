namespace ITP485
{
	template< typename... ArgTypes >
	class DelegateList
	{
	public:
		void Register( uint32_t inId, const function< void( ArgTypes... ) >& inFunction )
		{
			//run through, if we have something of this name, replace it
			int index = GetIndexById( inId );
			Dbg_Assert( index == -1, "delegate with id already registered" );
			mDelegates.push_back( Delegate( inId, inFunction ) );
		}

		void Unregister( uint32_t inId )
		{
			int index = GetIndexById( inId );
			if( index != -1 )
			{
				mDelegates.erase( mDelegates.begin() + index );
			}
		}

		
		void operator()( ArgTypes... inArgs ) const
		{
			for( const auto& d : mDelegates )
			{
				d.GetFunction()( inArgs... );
			}
		}
		

	private:

		int GetIndexById( uint32_t inId ) const
		{
			for( int i = 0, c = mDelegates.size(); i < c; ++i )
			{
				if( mDelegates[ i ].GetId() == inId )
				{
					return i;
				}
			}

			return -1;
		}
		
		class Delegate
		{
		public:

			Delegate( uint32_t inId, const function< void( ArgTypes... ) >& inFunction ) :
				mId( inId ),
				mFunction( inFunction )
			{}

			const uint32_t							GetId()			const	{ return mId; }
			const function< void( ArgTypes... ) >&	GetFunction()	const	{ return mFunction; }
		private:
			uint32_t		mId;
			function< void( ArgTypes... ) >	mFunction;
		};

		vector< Delegate >	mDelegates;
	};
}