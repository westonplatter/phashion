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

Usage
---------

    require 'phashion'
    img1 = Phashion::Image.new(filename1)
    img2 = Phashion::Image.new(filename2)
    img1.duplicate?(img2)
    --> true

Find the fingerprint of an image:

    img1.fingerprint.to_s(2)
    --> "1000001000110101101111100100001111011001101001100100110101101010" 

Find the hamming distance between two images:

    Phashion.hamming_distance(img1.fingerprint, img2.fingerprint)
    --> 28

Author
==========

Mike Perham, http://mikeperham.com, http://twitter.com/mperham, mperham AT gmail.com

Copyright
----------

Copyright (c) 2010 Mike Perham. See LICENSE for details.
