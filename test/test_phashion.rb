require 'helper'

class TestPhashion < Test::Unit::TestCase

  def test_duplicate_detection
    files = %w(86x86-0a1e.jpeg 86x86-83d6.jpeg 86x86-a855.jpeg)
    images = files.map {|f| Phashion::Image.new("#{File.dirname(__FILE__) + '/../test/'}#{f}")}
    assert_duplicate images[0], images[1]
    assert_duplicate images[1], images[2]
    assert_duplicate images[0], images[2]
  end

  def test_duplicate_detection_2
    files = %w(b32aade8c590e2d776c24f35868f0c7a588f51e1.jpeg df9cc82f5b32d7463f36620c61854fde9d939f7f.jpeg e7397898a7e395c2524978a5e64de0efabf08290.jpeg)
    images = files.map {|f| Phashion::Image.new("#{File.dirname(__FILE__) + '/../test/'}#{f}")}
    assert_duplicate images[0], images[1]
    assert_duplicate images[1], images[2]
    assert_duplicate images[0], images[2]
  end

  def test_not_duplicate
    files = %w(86x86-0a1e.jpeg 86x86-83d6.jpeg 86x86-a855.jpeg avatar.jpg)
    images = files.map {|f| Phashion::Image.new("#{File.dirname(__FILE__) + '/../test/'}#{f}")}
    assert_not_duplicate images[0], images[3]
    assert_not_duplicate images[1], images[3]
    assert_not_duplicate images[2], images[3]
  end

  def test_hamming_distance
    files = %w(86x86-0a1e.jpeg 86x86-83d6.jpeg 86x86-a855.jpeg avatar.jpg b32aade8c590e2d776c24f35868f0c7a588f51e1.jpeg)
    images = files.map {|f| Phashion::Image.new("#{File.dirname(__FILE__) + '/../test/'}#{f}")}
    assert_hamming_distance(images[0], images[0], 0)
    assert_hamming_distance(images[0], images[1], 2)
    assert_hamming_distance(images[0], images[2], 2)
    assert_hamming_distance(images[0], images[3], 26)
    assert_hamming_distance(images[0], images[4], 28)
  end

  private

  def assert_duplicate(a, b)
    assert a.duplicate?(b), "#{a.filename} not dupe of #{b.filename}"
  end

  def assert_not_duplicate(a, b)
    assert !a.duplicate?(b), "#{a.filename} dupe of #{b.filename}"
  end

  def assert_hamming_distance(a, b, c)
    Phashion.hamming_distance(a.fingerprint, b.fingerprint) == c
  end  
end
