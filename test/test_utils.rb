require 'helper'

class TestUtils < Minitest::Test
  def test_is_remote_file
    assert Utils.is_remote?('http://www.wakanda.org/sites/default/files/blog/blog-github.png')
  end

  def test_not_is_remote_file
    assert_equal false, Utils.is_remote?(File.dirname(__FILE__) + '/jpg/Broccoli_Super_Food.jpg')
  end
end