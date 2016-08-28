#ifndef __DEBUGLOG_H__
#define __DEBUGLOG_H__
#include <windows.h>
#include <..\um\debugapi.h>
#include <sstream>

#ifdef _DEBUG
#define DEBUGLOG( __log__ )            \
{                             \
   std::wostringstream ss;    \
   ss << __log__ << std::endl;                   \
   OutputDebugStringW( ss.str().c_str() );  \
}
#else
#define DEBUGLOG( __log__ )
#endif
#endif // !__DEBUGLOG_H__
