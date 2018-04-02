Phashion
========
[![Build Status](https://travis-ci.org/westonplatter/phashion.svg?branch=tests-travisci)](https://travis-ci.org/westonplatter/phashion)

Phashion is a Ruby wrapper around the [pHash library](http://phash.org/), "perceptual hash", which detects duplicate and near-duplicate multimedia files (e.g. images, audio, video, though Phashion currently only supports images.). "Near-duplicates" are images that come from the same source and show essentially the same thing, but may have differences in such features as dimensions, bytesizes, lossy-compression artifacts, and color levels.

[See an overview of Phashion on Mike's blog](http://www.mikeperham.com/2010/05/21/detecting-duplicate-images-with-phashion/).

Installation
------------

You install it just like any other Ruby gem:

    gem install phashion

Phashion is somewhat involved to install as it has a few dependencies. Phashion
wraps these dependencies into a custom tarball that is built locally just
for this gem so you don't have to do anything special. Look in the
`ext/phashion_ext` folder for more details.


### Compatibility
Because of this complexity, it is possible the gem install will fail on your
platform. Phashion has been tested on:

* Mac OSX 10.6
* Mac OSX 10.9
* Ubuntu 8.04
* Ubuntu 12.04

Please open a [GitHub issue](https://github.com/westonplatter/phashion/issues/) if you have installation problems.

### Prerequisites

Linux  
- libjpeg-dev
- libpng-dev

Mac
- [imagemagick](http://www.imagemagick.org/)
- libjpeg (Hombrew, `brew install libjpeg`; Macports, `port install jpeg`)
- libpng (Hombrew, `brew install libpng`; Macports, `port install libpng`)


#### Common Errors
- `ld: library not found for -ljpeg` &ndash; Did you install `libjpeg`?
- `... sh: convert: command not found; sh: gm: command not found` &ndash; Did you install [imagemagick](http://www.imagemagick.org/)?
- `... checking for sqlite3ext.h... *** extconf.rb failed *** ...` &ndash; Did you install `libpng-dev` and/or `libjpeg-dev`?


Usage
-----

### Testing if one image is a duplicate of another

```ruby
require 'phashion'
img1 = Phashion::Image.new(filename1)
img2 = Phashion::Image.new(filename2)
img1.duplicate?(img2)
# --> true
```
Optionally, you can set the minimum Hamming distance in the second argument, an options Hash:
```ruby
img1.duplicate?(img2, :threshold => 5)
# --> true

img1.duplicate?(img2, :threshold => 0)  
# --> false
```

### Finding the Hamming distance between two images

```ruby
require 'phashion'
img1 = Phashion::Image.new(filename1)
img2 = Phashion::Image.new(filename2)
img1.distance_from(img2)  
# --> 6
```

### Threshold for dupe-detection

Currently, the maximum Hamming distance between two duplicate images is set at 15. As per [mperham's explanation](http://www.mikeperham.com/2010/05/21/detecting-duplicate-images-with-phashion/):

> A “perceptual hash”is a 64-bit value based on the discrete cosine transform of the image’s frequency spectrum data. Similar images will have hashes that are close in terms of Hamming distance. That is, a binary hash value of 1000 is closer to 0000 than 0011 because it only has one bit different whereas the latter value has two bits different. The duplicate threshold defines how many bits must be different between two hashes for the two associated images to be considered different images. Our testing showed that 15 bits is a good value to start with, it detected all duplicates with a minimum of false positives.

As a reference point, here are the Hamming distances in these test comparisons using [/test/jpg/Broccoli_Super_Food.jpg](https://github.com/westonplatter/phashion/blob/master/test/jpg/Broccoli_Super_Food.jpg) as the source image:


| Variation                                            | Hamming distance  
| ---------------------------------------------------- | ----------------:
| JPG to PNG                                           | 0
| Lossy JPG (Photoshop Save for Web quality = 20)      | 0                 
| Thumbnail (from 500px to 100px)                      | 2
| Color correction (saturation +20 w auto-correct)     | 2          
| Black and white                                      | 2
| Extraneous whitespace cropped (500x349 to 466x312)   | 12
| A sloppy rotation of 5 degrees clockwise             | 14
| Horizontally-flipped                                 | 32





Gem uses customized pHash 0.9.6
-------------------------------

In order to detech duplicate alpha PNGs, the gem uses a custom version of pHash
0.9.6. The customization is limited to only these changes,
[westonplatter/phash@ff255d2](https://github.com/westonplatter/phash/commit/ff255d2d3f93c841b98923ecbde997027f21ae36).
The gem will be moving back to the pHash master branch once it supports
detection of alpha PNG file types.


Testing
-------

#### To run the test suite:

    $ cd phashion
    $ bundle install
    $ rake compile
    $ rake test


Author
======

Mike Perham,
http://mikeperham.com,
http://twitter.com/mperham,
mperham AT gmail.com

Copyright
---------

Copyright (c) 2010-2014 Mike Perham. See LICENSE for details.
