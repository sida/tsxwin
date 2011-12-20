/* TinyScheme 拡張DLL
 *  (system "コマンド")
 *  (getenv "環境変数名")
 */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#define _SCHEME_SOURCE
#include "scheme-private.h"

SCHEME_EXPORT void init_tsx_system(scheme *sc);
SCHEME_EXPORT pointer tsx_system(scheme *sc, pointer args);
SCHEME_EXPORT pointer tsx_getenv(scheme *sc, pointer args);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//初期化
void init_tsx_system(scheme *sc)
{
	sc->vptr->scheme_define(sc ,sc->global_env ,sc->vptr->mk_symbol(sc,"system") ,sc->vptr->mk_foreign_func(sc, tsx_system));
	sc->vptr->scheme_define(sc ,sc->global_env ,sc->vptr->mk_symbol(sc,"getenv") ,sc->vptr->mk_foreign_func(sc, tsx_getenv));
}

//外部プログラムの実行
pointer tsx_system(scheme *sc, pointer args)
{
	pointer first_arg;
	pointer retval;
	char *command;
	int ret;

	if ((args != sc->NIL) && sc->vptr->is_string((first_arg = sc->vptr->pair_car(args))))
	{
		command = sc->vptr->string_value(first_arg);
		ret = system(command);
		retval = sc->vptr->mk_integer(sc, (long)ret);	//戻り値
	}
	else
	{
		retval = sc -> F;
	}
	return(retval);
}

//getenv
pointer tsx_getenv(scheme *sc, pointer args)
{
	pointer arg_name;
	char *pvarname;
	char *penv;
	pointer retval;

	if ((args != sc->NIL) && sc->vptr->is_string((arg_name = sc->vptr->pair_car(args))))
	{
		pvarname = sc->vptr->string_value(arg_name);
		penv = getenv(pvarname);
		if (NULL==penv)
		{
			retval = sc -> F;
		}
		else
		{
			retval = sc->vptr->mk_string(sc, penv);	//戻り値
		}
	}
	else
	{
		retval = sc -> F;
	}
	return(retval);
}



