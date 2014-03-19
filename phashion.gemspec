# -*- encoding: utf-8 -*-
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'phashion/version'
  
Gem::Specification.new do |s|
  s.name          = 'phashion'
  s.version       = Phashion::VERSION
  s.authors       = ["Mike Perham"]
  s.email         = ["mperham@gmail.com"]
  s.description   = 'Simple wrapper around the pHash library'
  s.homepage      = 'http://github.com/westonplatter/phashion'
  
  s.extensions    = ["ext/phashion_ext/extconf.rb"]
  s.files         = `git ls-files`.split("\n")
  s.rdoc_options  = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.summary       = %q{Simple wrapper around the pHash library}
  s.test_files    = `git ls-files test`.split("\n")
  
  s.add_development_dependency "rake-compiler", ">= 0.7.0"
  s.add_development_dependency "sqlite3"
  s.add_development_dependency "minitest", "~> 5.2.2"
end

