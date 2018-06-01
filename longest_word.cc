#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <ctype.h>

#define INIT_ALLOC_LEN 256

extern "C"
{
	my_bool longest_word_init(UDF_INIT *initid, UDF_ARGS* args, char* message);
	void longest_word_deinit(UDF_INIT *initid);
	char* longest_word( UDF_INIT* initid, UDF_ARGS* args __attribute__((unused)),
         char* result, unsigned long* length, char* is_null, char* error __attribute__((unused)));
	void longest_word_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error);
	void longest_word_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error);
	void longest_word_clear( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error);
}

typedef struct
{
	char* word;
	ulonglong len;
	ulonglong alloc_len;
	ulonglong local_len;
} DATA_BUF;

DATA_BUF* get_data_buf()
{
	DATA_BUF* buf = (DATA_BUF*)malloc(sizeof(DATA_BUF) + INIT_ALLOC_LEN);
	if (!buf) return 0;
	buf->word = (char*)(buf + 1);
	buf->len = 0;
	buf->local_len = INIT_ALLOC_LEN;
	buf->alloc_len = 0;
	return buf;
}

my_bool update_word(DATA_BUF* buf, const char* word, ulonglong word_len)
{
	if (buf->alloc_len || word_len > buf->local_len)
	{
		if (buf->alloc_len < word_len)
		{
			if (!buf->alloc_len)
				buf->word = 0;
			if (!(buf->word = (char*)realloc(buf->word, word_len)))
				return 1;

			buf->alloc_len = word_len;
		}
	}

	memcpy(buf->word, word, word_len);
	buf->len = word_len;
	return 0;
}

my_bool longest_word_init( UDF_INIT* initid, UDF_ARGS* args, char* message)
{
	if (args->arg_count != 1)
	{
		strcpy(message, "Usage: longest_word(body)");
		return 1;
	}

	if ((args->arg_type[0] != STRING_RESULT))
	{
		strcpy(message, "body argument must be a string");
		return 1;
	}

	if (!(initid->ptr = (char*)get_data_buf()))
	{
		strcpy(message, "Out of memory");
		return 1;
	}

	return 0;
}

void longest_word_deinit(UDF_INIT *initid)
{
	free(initid->ptr);
}

void longest_word_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error)
{
	longest_word_clear(initid, args, is_null, error);
	longest_word_add(initid, args, is_null, error);
}

void longest_word_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error)
{
	const char* body = args->args[0];
	if (!body) return;
	longlong body_len = args->lengths[0];
	const char* body_end = body + body_len;
	const char* word = 0;
	DATA_BUF* buf = (DATA_BUF*)initid->ptr;

	for (; body <= body_end; body++) /* <= is intententional, special handling of end of string */
	{
		int is_alnum = (body < body_end) ? isalnum(*body) : 0;

		/* do not touch *body if  body == body_end */
		if (!word && is_alnum) /* this will not happen on body_end due to is_alnum == 0 */
		{
			word = body;
			continue;
		}

		/* is one is OK on body == body_end */
		if (word && !is_alnum)
		{
			longlong cur_len = body - word;

			if (buf->len < cur_len)
			{
				if (update_word(buf, word, cur_len))
				{
					strcpy(error, "Out of memory");
					return;
				}
			}

			word = 0;
		}
	}
}

void longest_word_clear( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error)
{
	DATA_BUF* buf = (DATA_BUF*)initid->ptr;
	buf->len = 0;
}

char* longest_word( UDF_INIT* initid, UDF_ARGS* args __attribute__((unused)),
         char* result, unsigned long* length, char* is_null, char* error __attribute__((unused)))
{
	DATA_BUF* buf = (DATA_BUF*)initid->ptr;

	if (!buf || !buf->word || !buf->len)
	{
		*is_null = 1;
		return 0;
	}

	*is_null = 0;
	*length = buf->len;
	return buf->word;
}



