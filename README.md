Phashion
===========

Phashion is a Ruby wrapper around the pHash library, "perceptual hash", which detects duplicate
and near duplicate multimedia files (images, audio, video).  The wrapper currently only supports images.

[See an overview of Phashion on my blog](http://www.mikeperham.com/2010/05/21/detecting-duplicate-images-with-phashion/).

Installation
-------------

First you need to install pHash.  pHash requires three libraries: CImg, ffmpeg and libjpeg.  My system already came with libjpeg on it so I didn't have to do anything for it.  YMMV.

Install CImg.h by downloading the latest version from http://cimg.sf.net and placing the CImg.h header file in the root of the pHash source.

If you are working with audio or video, you will need to install ffmpeg:

    port install ffmpeg (OR)
    brew install ffmpeg

Alternatively you can configure pHash to not support audio/video:

    ./configure --disable-audio-hash --disable-video-hash

Download and install the latest pHash tarball from http://phash.org/download/.  With 0.9.0, there are several issues with OSX: I had to disable audio and video support to avoid compilation issues and modify `ph_num_threads` in pHash.cpp to avoid Linux-specific code:

    ./configure --disable-audio-hash --disable-video-hash
    
    int ph_num_threads()
    {
    	int numCPU = 2;
    	return numCPU;
    }

Finally, run `make && make install` to install the pHash binaries.

Now you can install this gem:

    gem install phashion

On Linux, I had to use two further flags to get pHash to compile correctly for use as a Ruby extension:

    ./configure --disable-audio-hash --disable-video-hash --disable-pthread --with-pic

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
