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

my_bool earth_distance_init(UDF_INIT *, UDF_ARGS *args, char *message);
double earth_distance(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                     char *error);

C_MODE_END;

my_bool earth_distance_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  uint i;

  if (args->arg_count != 4)
  {
    strcpy(message,"Usage: earth_distance(lat1,lon1,lat2,lon2)");
    return 1;
  }

  for (i=0 ; i < args->arg_count; i++)
    args->arg_type[i]=REAL_RESULT;

  initid->maybe_null=1;         /* The result may be null */
  initid->decimals=2;           /* We want 2 decimals in the result */
  initid->max_length=6;         /* 3 digits + . + 2 decimals */
  return 0;
}

#define R_EARTH_MILES 3963.1676

static inline double deg2rad(double deg)
{
  return (deg * M_PI / 180);
}

static inline double rad2deg(double rad)
{
  return (rad * 180 / M_PI);
}

#define GET_ARG(v,ind) do { if (args->args[ind] == NULL) \
  { *is_null = 1; return 0.0;} v = *((double*)args->args[ind]);} while (0);

double earth_distance(UDF_INIT *initid __attribute__((unused)), UDF_ARGS *args,
                     char *is_null, char *error __attribute__((unused)))
{
  double lat1,lat2,lon1,lon2;
  GET_ARG(lat1,0);
  GET_ARG(lon1,1);
  GET_ARG(lat2,2);
  GET_ARG(lon2,3);
  double dlat = deg2rad(lat2 - lat1), dlon = deg2rad(lon2 - lon1);
  double central_hs =  (1.0 - cos(dlat))/2.0 + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * (1.0 - cos(dlon))/2.0;
  double central_angle = 2 * atan2(sqrt(central_hs), sqrt(1 - central_hs));
  return R_EARTH_MILES * central_angle;
}


