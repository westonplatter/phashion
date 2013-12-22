Phashion
========

Phashion is a Ruby wrapper around the pHash library, "perceptual hash", which 
detects duplicate and near duplicate multimedia files (images, audio, video). 
The wrapper currently only supports images.

[See an overview of Phashion on Mike's blog]
(http://www.mikeperham.com/2010/05/21/detecting-duplicate-images-with-phashion/).

Installation
------------

You install it just like any other Ruby gem:

    gem install phashion

Phashion is somewhat involved to install as it has a few dependencies. Phashion
wrapps these dependencies into a custom tarball that is built locally just 
for this gem so you don't have to do anything special. Look in the 
`ext/phashion_ext` folder for more details.

Because of this complexity, it is possible the gem install will fail on your 
platform. Phashion has been tested on:

* Mac OSX 10.6 
* Mac OSX 10.9
* Ubuntu 8.04
* Ubuntu 12.04

Please open a [GitHub issue](https://github.com/westonplatter/phashion/issues/) 
if you have installation problems.


Common Error: library not found for -ljpeg
------------------------------------------

If you have an error upon install, like:

    ld: library not found for -ljpeg

you need to install libjpeg.  If you're on a Mac, you can use 
[homebrew](http://brew.sh/)  `brew install libjpeg`  or 
[ports](http://www.macports.org/)  `port install jpeg` .


Usage
-----

    require 'phashion'
    img1 = Phashion::Image.new(filename1)
    img2 = Phashion::Image.new(filename2)
    img1.duplicate?(img2)
    --> true


Gem uses customized pHash 0.9.6
-------------------------------

In order to detech duplicate alpha PNGs, the gem uses a custom version of pHash 
0.9.6. The customization is limited to only these changes, 
[westonplatter/phash@ff255d2]
(https://github.com/westonplatter/phash/commit/ff255d2d3f93c841b98923ecbde997027f21ae36). 
The gem will be moving back to the pHash master branch once it supports 
detection of alpha PNG file types.
Testing
-------

To run the test suite:

    bundle
    rake compile
    rake


Author
======

Mike Perham, 
http://mikeperham.com, 
http://twitter.com/mperham, 
mperham AT gmail.com

Copyright
---------

Copyright (c) 2010-2014 Mike Perham. See LICENSE for details.
