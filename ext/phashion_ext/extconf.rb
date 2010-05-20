require 'mkmf'

$CFLAGS << " -x c++ #{ENV["CFLAGS"]}"
$LIBS << " -lpHash #{ENV["LIBS"]}"

# TODO: need to figure this stuff out
# dir_config 'pHash'
# if !have_library('pHash', 'ph_dct_imagehash')
#   puts "Unable to find pHash library, please use 'gem install phashion -- --with-pHash-dir=/phash/install/root'"
#   exit 1
# end

create_makefile('phashion_ext')
