#include <mysql.h>
#include <string.h>

#define HELLO_STR "Hello, world"
#define HELLO_LEN strlen(HELLO_STR)

extern "C"
{
my_bool hello_init(UDF_INIT *initid, UDF_ARGS* args, char* message);
char* hello(UDF_INIT* initid, UDF_ARGS* args, char* res,
						uint* len, char* is_null, char* error);
}

my_bool hello_init(UDF_INIT *initid, UDF_ARGS* args, char* message)
{
	return 0;
}

char* hello(UDF_INIT* initid, UDF_ARGS* args, char* res,
						uint* len, char* is_null, char* error)
{
	*len = HELLO_LEN;
	memcpy(res, HELLO_STR, *len + 1);
	*is_null = 0;
	return res;
}
