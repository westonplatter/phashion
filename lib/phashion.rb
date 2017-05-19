##
# Provides a clean and simple API to detect duplicate image files using
# the pHash library under the covers.
#
# The C API:
# int ph_dct_imagehash(const char *file, ulong64 &hash);
# int ph_hamming_distance(ulong64 hasha, ulong64 hashb);

require 'rbconfig'

module Phashion
  TextHashPoint = Struct.new :hash, :index
  TextMatch = Struct.new :first_index, :second_index, :length

  DEFAULT_DUPE_THRESHOLD = 15

  class Image
    attr_reader :filename
    def initialize(filename)
      @filename = filename
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

    (0..64).each do |n|
      define_method("dupe_at_threshold_#{n}?") { |oth| distance_from(oth) <= n }
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

  def self.duplicate?(fingerprint1, fingerprint2, opts={})
    threshold = opts[:threshold] || DEFAULT_DUPE_THRESHOLD

    Phashion.hamming_distance(fingerprint1, fingerprint2) <= threshold
  end
end

require 'phashion_ext'
