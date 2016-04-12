#define DECLARE_ALIGNED_NEW_DELETE				\
static void* operator new(size_t inSize)		\
{												\
	return _aligned_malloc( inSize, 16 );		\
}												\
static void operator delete(void* inPtr)		\
{												\
	_aligned_free( inPtr );						\
}