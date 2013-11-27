# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name          = %q{phashion}
  s.version       = "1.0.7"
  s.authors       = ["Mike Perham", "Weston Platter"]
  s.email         = ["mperham@gmail.com", "westonplatter@gmail.com"]
  s.description   = %q{Simple wrapper around the pHash library}
  s.homepage      = %q{http://github.com/westonplatter/phashion}
  
  s.extensions    = ["ext/phashion_ext/extconf.rb"]
  s.files         = `git ls-files`.split("\n")
  s.rdoc_options  = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.summary       = %q{Simple wrapper around the pHash library}
  s.test_files    = `git ls-files test`.split("\n")
  
  s.add_development_dependency "rake-compiler", ">= 0.7.0"
end

