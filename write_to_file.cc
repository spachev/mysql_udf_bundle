#include <my_global.h>
#include <my_sys.h>

#if defined(MYSQL_SERVER)
#include <m_string.h>           /* To get my_stpcpy() */
#else
/* when compiled as standalone */
#include <string.h>
#define my_stpcpy(a,b) stpcpy(a,b)
#endif

#include <mysql.h>
#include <ctype.h>

C_MODE_START;

my_bool write_to_file_init(UDF_INIT *, UDF_ARGS *args, char *message);
longlong write_to_file(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                     char *error);

C_MODE_END;

#define OUTPUT_DIR "/tmp"

static int check_fname(const char* fname)
{
	if (strchr(fname,'/'))
		return 0;

	return 1;
}

my_bool write_to_file_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  uint i;

  if (args->arg_count != 2)
  {
    strcpy(message,"Usage: write_to_file(file_name,file_data)");
    return 1;
  }

  for (i=0 ; i < args->arg_count; i++)
    args->arg_type[i]=STRING_RESULT;

  initid->maybe_null=1;         /* The result may be null */
  return 0;
}


longlong write_to_file(UDF_INIT *initid __attribute__((unused)), UDF_ARGS *args,
                     char *is_null, char *error __attribute__((unused)))
{
	const char* fname = args->args[0];
	const char* fdata = args->args[1];
	char fname_buf[512];
	ulonglong data_len = args->lengths[1];
	int fd;


	if (!fname || !check_fname(fname) || args->lengths[0] + strlen(OUTPUT_DIR) + 2 > sizeof(fname_buf))
		goto err;

	if (!fdata)
		goto err;

	snprintf(fname_buf, sizeof(fname_buf), OUTPUT_DIR"/%s", fname);
	fd = open(fname_buf, O_WRONLY|O_CREAT, 0660);

	if (fd < 0)
	{
		fprintf(stderr, "Could not open file %s: %s\n", fname_buf, strerror(errno));
		goto err;
	}

	// TODO: retry on failed I/O
	if (write(fd,fdata,data_len) != data_len)
	{
		fprintf(stderr, "Error writing to file %s: %s\n", fname_buf, strerror(errno));
		goto err;
	}
	close(fd);
	*is_null = 0;
	return 1;
err:
	*is_null = 1;
	return 0;
}


