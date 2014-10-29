History
=======

next
-----
* Added TravisCI testing support (issue #49)
* Renamed internally used Sqlite3 client function name (issue #49)

1.1.1 
-----
* Added Ruby method to call into pHash's Mexican Hat Wavelet funcition (issue #37, Terence Lee / @hone)
* Removed duplicate version reference

1.1.0
-----
* Converted to Minitest (issue #35)
* Added Phashion.distance_from method (issue #30)
* Add Sqlite3 extension to handle distance calculations (issue #27)

1.0.8
------
* Renamed the pHashion tar.gz archive as "pHash-0.9.6.tar.gz" to solve/enable install from github.

1.0.7
------
* Respository transferred to https://github.com/westonplatter/phashion
* Added custom pHash patch to support alpha channel PNG files (issue #20 and #23)
* Link `/user/local/lib` (issue #21)
* Explicitly include -pthreads flag for cpp compiler (issue #15)

1.0.6
------

* Update pHash to version 0.9.6
* PNG support added
* Requiring rdoc tasks fixed
 
1.0.5
-------
* Fix CImg.h compilation problems. [#10]

1.0.4
-------
* Fix pthread linking on Ubuntu 11.04

1.0.3
-------
* Update pHash to version 0.9.3 and CImg 1.4.7.
* Fix a few build issues.

1.0.2
-------
* Make installation much easier by distributing and building locally all the native dependencies.
  This includes pHash 0.9.0 and CImg 1.3.4.

1.0.1
-------
* Remove RubyInline, use standard Ruby extension infrastructure.
* Update duplicate threshold constant based on wider image testing.
* Make duplicate threshold variable so users can tune it based on their dataset.
* Add Phashion::Image#fingerprint method which exposes an Image's 64-bit hash.


1.0.0
-------
* Initial release.
