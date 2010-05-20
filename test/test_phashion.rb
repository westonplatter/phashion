require 'helper'

class TestPhashion < Test::Unit::TestCase

  def test_duplicate_detection
    files = %w(86x86-0a1e.jpeg 86x86-83d6.jpeg 86x86-a855.jpeg)
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

  private

  def assert_duplicate(a, b)
    raise ArgumentError, "#{a.filename} not dupe of #{b.filename}" unless a.duplicate?(b)
  end

  def assert_not_duplicate(a, b)
    raise ArgumentError, "#{a.filename} dupe of #{b.filename}" if a.duplicate?(b)
  end  
end
