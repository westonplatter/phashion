History
=========

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

Initial release.