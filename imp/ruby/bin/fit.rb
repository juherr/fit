$:.unshift File.join(File.dirname(__FILE__), '..', 'lib')

require 'fit/file_runner'

Fit::FileRunner.new.run ARGV if __FILE__ == $0
