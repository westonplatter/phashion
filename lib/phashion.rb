require 'rubygems'
require 'inline'

##
# Provides a clean and simple API to detect duplicate image files using
# the pHash library under the covers.
#
# The C API:
# int ph_dct_imagehash(const char *file, ulong64 &hash);
# int ph_hamming_distance(ulong64 hasha, ulong64 hashb);

module Phashion
  VERSION = '1.0.0'
  
  class Image
    DUPE_THRESHOLD = 15
    
    attr_reader :filename
    def initialize(filename)
      @filename = filename
    end
    
    def duplicate?(other)
      Phashion.hamming_distance(fingerprint, other.fingerprint) < DUPE_THRESHOLD
    end
    
    def fingerprint
      @hash ||= Phashion.image_hash_for(@filename)
    end
  end
  
  def self.image_hash_for(filename)
  end

  def self.hamming_distance(hashA, hashB)
  end

  inline do |builder|
    if test ?d, "/opt/local" then
      builder.add_compile_flags "-I/opt/local/include"
      builder.add_link_flags "-L/opt/local/lib"
    end

    builder.add_compile_flags '-x c++', '-lstdc++'
    builder.add_link_flags "-lpHash"
    builder.include '"pHash.h"'

    builder.c_singleton <<-"END"
      VALUE image_hash_for(const char *filename) {
        ulong64 hash;
        if (-1 == ph_dct_imagehash(filename, hash)) {
          rb_raise(rb_eRuntimeError, "Unknown pHash error");
        }
        return ULL2NUM(hash);
      }
    END

    builder.c_singleton <<-"END"
      VALUE hamming_distance(VALUE a, VALUE b) {
        int result = 0;
        result = ph_hamming_distance(NUM2ULL(a), NUM2ULL(b));
        if (-1 == result) {
          rb_raise(rb_eRuntimeError, "Unknown pHash error");
        }
        return INT2NUM(result);
      }
    END

  end
end

if __FILE__ == $0
  
  def memory
    `ps -o vsz,rss -p #{$$}`.strip
  end
  
  def assert_duplicate(a, b)
    raise ArgumentError, "#{a.filename} not dupe of #{b.filename}" unless a.duplicate?(b)
  end

  def assert_not_duplicate(a, b)
    raise ArgumentError, "#{a.filename} dupe of #{b.filename}" if a.duplicate?(b)
  end

  FILES = %w(86x86-0a1e.jpeg 86x86-83d6.jpeg 86x86-a855.jpeg avatar.jpg)

  images = FILES.map {|f| Phashion::Image.new("#{File.dirname(__FILE__) + '/../test/'}#{f}")}
  # GC.start
  # puts memory
  assert_duplicate images[0], images[1]
  assert_duplicate images[1], images[2]
  assert_duplicate images[0], images[2]

  assert_not_duplicate images[0], images[3]
  assert_not_duplicate images[1], images[3]
  assert_not_duplicate images[2], images[3]
  # GC.start
  # puts memory
  
end
