#define ARRAY_DEBUG 0

//Method1: method for trigger a break point.
#ifdef _WIN64
#define ASSERT( x ) { if( !( x ) ) __debugbreak(); }
#else // !_WIN64
#ifdef _WIN32
#define ASSERT( x ) { if( !( x ) ) _asm{ int 0x03 } }
#else // !_WIN32
#define ASSERT( x ) { if( !( x ) ) exit(0); }
#endif // _WIN32
#endif // _WIN64


//Method2: copy from assert.h
#include <crtdefs.h>
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t *_Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#define ASSERT2(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

//#error "error test"

#include <assert.h>
//#include <cassert>
int main() {
  int n = 0;
  assert(n == 1);//assertion for debug mode.
  //static_assert(sizeof(n) == 8, "n==8");
  //ASSERT(n == 1);//trigger a break point in both debug and release mode.
  //ASSERT2(n == 2);
  return 0;
}