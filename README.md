# mysql_udf_bundle
Random collection of MySQL UDFs

To install (assuming Ubuntu and the likes):

```
apt-get install libmysqlclient-dev g++
cmake
make
sudo make install
```

In MySQL as root:

```
create function earth_distance returns real soname 'libearth_distance.so';
create function write_to_file returns int soname 'libwrite_to_file.so';
create function hello returns string soname 'libhello.so';
```

Usage:

```
earth_distance(lat1,lon1,lat2,lon2)
write_to_file(fname,fdata);
hello();
```

Usage examples:

```
select earth_distance(40.0,-111.0,41.0,-112);
+---------------------------------------+
| earth_distance(40.0,-111.0,41.0,-112) |
+---------------------------------------+
|                                 86.90 |
+---------------------------------------+
1 row in set (0.00 sec)

select earth_distance(40.0,-111.0,41.0,NULL);
+---------------------------------------+
| earth_distance(40.0,-111.0,41.0,NULL) |
+---------------------------------------+
|                                  NULL |
+---------------------------------------+
1 row in set (0.03 sec)

select earth_distance(40.0,-111.0,41.0,"-112");
+-----------------------------------------+
| earth_distance(40.0,-111.0,41.0,"-112") |
+-----------------------------------------+
|                                   86.90 |
+-----------------------------------------+
1 row in set (0.01 sec)
```
