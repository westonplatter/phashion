require 'rubygems'
require 'rake'
require 'bundler'
require 'rake/testtask'
require "rake/extensiontask"

SPEC = Bundler.load_gemspec("phashion.gemspec")

Rake::TestTask.new(:test) do |test|
  test.libs << 'lib' << 'test'
  test.pattern = 'test/**/test_*.rb'
  test.verbose = true
end

begin
  require 'rcov/rcovtask'
  Rcov::RcovTask.new do |test|
    test.libs << 'test'
    test.pattern = 'test/**/test_*.rb'
    test.verbose = true
  end
rescue LoadError
  task :rcov do
    abort "RCov is not available. In order to run rcov, you must: sudo gem install spicycode-rcov"
  end
end

task :test

task :default => :test

require 'rdoc/task'
Rake::RDocTask.new do |rdoc|
  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "phashion"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

Gem::PackageTask.new(SPEC) do |pkg|
end

Rake::ExtensionTask.new("phashion_ext", SPEC) do |ext|
  ext.lib_dir = "lib/phashion_ext"
  ext.cross_compile = true
  ext.cross_platform = %w(arm64-darwin x86_64-linux x86_64-darwin)
  ext.cross_config_options << {
    "arm64-darwin" => "--with-opt-dir=/opt/homebrew/opt/openssl",
  }
end
