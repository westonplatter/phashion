##
# Provides a clean and simple API to detect duplicate image files using
# the pHash library under the covers.
#
# The C API:
# int ph_dct_imagehash(const char *file, ulong64 &hash);
# int ph_hamming_distance(ulong64 hasha, ulong64 hashb);

require 'rbconfig'
require 'open-uri'
require File.dirname(__FILE__) + '/utils'

module Phashion
  TextHashPoint = Struct.new :hash, :index
  TextMatch = Struct.new :first_index, :second_index, :length

  class Image
    DEFAULT_DUPE_THRESHOLD = 15
 
    attr_reader :filename
    def initialize(filename)
      @filename = (Utils.is_remote?(filename))? open(filename).path : filename ;
    end

    # returns: an Integer representing the hamming distance from :other
    def distance_from(other)
      Phashion.hamming_distance(fingerprint, other.fingerprint)
    end

    def mh_distance_from(other)
      Phashion.hamming_distance2(mh_fingerprint, other.mh_fingerprint)
    end

    def duplicate?(other, opts={})
      threshold = opts[:threshold] || DEFAULT_DUPE_THRESHOLD

      distance_from(other) <= threshold
    end

    def fingerprint
      @hash ||= Phashion.image_hash_for(@filename)
    end

    def mh_fingerprint
      @mh_hash ||= Phashion.mh_hash_for(@filename)
    end
  end

  def self.mh_hash_for(filename, alpha = 2.0, lvl = 1.0)
    _mh_hash_for(filename, alpha, lvl)
  end

  def self.so_file
    extname = RbConfig::CONFIG['DLEXT']
    File.join File.dirname(__FILE__), "phashion_ext.#{extname}"
  end



end

require 'phashion_ext'
