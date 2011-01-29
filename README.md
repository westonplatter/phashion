Phashion
===========

Phashion is a Ruby wrapper around the pHash library, "perceptual hash", which detects duplicate
and near duplicate multimedia files (images, audio, video).  The wrapper currently only supports images.

[See an overview of Phashion on my blog](http://www.mikeperham.com/2010/05/21/detecting-duplicate-images-with-phashion/).

Installation
-------------

You install it just like any other Ruby gem:

    gem install phashion

Phashion is somewhat involved to install as it has a few dependencies.  I've wrapped up those
dependencies into a custom tarball that is built locally just for this gem so you don't have to
do anything special.  See the code in `ext/phashion_ext` for more details.

Because of this complexity, it is possible the gem install will fail on your platform.  I've tested
it on Mac OSX 10.6 and Ubuntu 8.04 but please contact me if you have installation problems.

If you have an error upon install, like:

    ld: library not found for -ljpeg

you need to install libjpeg.  "brew install libjpeg" or "port install jpeg" on OSX.

Usage
---------

    require 'phashion'
    img1 = Phashion::Image.new(filename1)
    img2 = Phashion::Image.new(filename2)
    img1.duplicate?(img2)
    --> true

Author
==========

Mike Perham, http://mikeperham.com, http://twitter.com/mperham, mperham AT gmail.com

Copyright
----------

Copyright (c) 2010 Mike Perham. See LICENSE for details.
