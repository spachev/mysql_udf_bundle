#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <ctype.h>

extern "C"
{
	my_bool word_count_init(UDF_INIT *initid, UDF_ARGS* args, char* message);
	longlong word_count(UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error);
}

my_bool word_count_init(UDF_INIT *initid, UDF_ARGS* args, char* message)
{
	if (args->arg_count != 1)
	{
		strcpy(message, "Usage: word_count(str)");
		return 1;
	}

	args->arg_type[0] = STRING_RESULT;
	initid->maybe_null = 1;
	return 0;
}

longlong word_count(UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error __attribute__((unused)))
{
	const char* s = args->args[0];

	if (!s)
	{
		*is_null = 1;
		return 0;
	}

	ulonglong len = args->lengths[0];
	const char* s_end = s + len;
	int prev_was_wchar = 0;
	longlong n_words = 0;

	for (; s < s_end; s++)
	{
		int cur_is_wchar = (isalnum(*s));
		if (!prev_was_wchar && cur_is_wchar)
			n_words++;
		prev_was_wchar = cur_is_wchar;
	}

	*is_null = 0;
	return n_words;
}
