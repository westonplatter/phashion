require 'mkmf'
require 'fileutils'

HERE = File.expand_path(File.dirname(__FILE__))
BUNDLE = Dir.glob("#{HERE}/pHash-*.tar.gz").first
BUNDLE_PATH = BUNDLE.gsub(".tar.gz", "")
$CFLAGS = " -x c++ #{ENV["CFLAGS"]}"
$CFLAGS += " -fdeclspec" if RUBY_PLATFORM =~ /darwin/
$includes = " -I#{HERE}/include"
$libraries = " -L#{HERE}/lib -L/usr/local/lib -L/opt/homebrew/lib"
$LIBPATH = ["#{HERE}/lib"]
$CFLAGS = "#{$includes} #{$libraries} #{$CFLAGS}"
$LDFLAGS = "#{$libraries} #{$LDFLAGS}"
$CXXFLAGS = ' -pthread -I/opt/homebrew/include'

Dir.chdir(HERE) do
  if File.exist?("lib")
    puts "pHash already built; run 'rake clean' first if you need to rebuild."
  else

    puts(cmd = "tar xzf #{BUNDLE} 2>&1")
    raise "'#{cmd}' failed" unless system(cmd)

    FileUtils.cp "#{HERE}/config.guess", "#{BUNDLE_PATH}/config.guess"
    FileUtils.cp "#{HERE}/config.sub", "#{BUNDLE_PATH}/config.sub"

    Dir.chdir(BUNDLE_PATH) do
      puts(cmd = "env CXXFLAGS='#{$CXXFLAGS}' CFLAGS='#{$CFLAGS}' LDFLAGS='#{$LDFLAGS}' ./configure --prefix=#{HERE} --disable-audio-hash --disable-video-hash --disable-shared --with-pic 2>&1")
      raise "'#{cmd}' failed" unless system(cmd)

      puts(cmd = "make || true 2>&1")
      raise "'#{cmd}' failed" unless system(cmd)

      puts(cmd = "make install || true 2>&1")
      raise "'#{cmd}' failed" unless system(cmd)

      puts(cmd = "mv CImg.h ../include 2>&1")
      raise "'#{cmd}' failed" unless system(cmd)
    end

    system("rm -rf #{BUNDLE_PATH}") unless ENV['DEBUG'] or ENV['DEV']
  end

  Dir.chdir("#{HERE}/lib") do
    system("cp -f libpHash.a libpHash_gem.a")
    system("cp -f libpHash.la libpHash_gem.la")
  end
  $LIBS = " -lpthread -lpHash_gem -lstdc++ -ljpeg -lpng -lm"
end

have_header 'sqlite3ext.h'

create_makefile 'phashion_ext'
