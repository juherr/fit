# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

require 'fit/scientific_double'
require 'parsedate'

module Fit

  class RunTime
    def initialize
      @elapsed = 0
      @start = Time.now
    end
    def to_s
      @elapsed = Time.now.to_f - @start.to_f
      if @elapsed > 1
        @elapsed = @elapsed.round
        return d(3600) + ':' + d(600) + d(60) + ':' + d(10) + d(1)
      else
        @elapsed = (@elapsed * 100).round
        return d(6000) + ':' + d(1000) + d(100) + '.' + d(10) + d(1)
      end
    end
    def d scale
      report = @elapsed / scale
      @elapsed -= report * scale
      report.to_s
    end
  end

  class Counts
    attr_accessor :right, :wrong, :ignores, :exceptions
    def initialize
      @right, @wrong, @ignores, @exceptions = 0, 0, 0, 0
    end
    def to_s
      "#@right right, #@wrong wrong, #@ignores ignored, #@exceptions exceptions"
    end
    def tally c
      @right += c.right
      @wrong += c.wrong
      @ignores += c.ignores
      @exceptions += c.exceptions
    end
    def total_errors
      @wrong + @exceptions
    end
  end

  class Fixture

    attr_accessor :summary, :counts

    def initialize
      @summary = {}
      @counts = Counts.new
    end

    def total_errors
      @counts.total_errors
    end
    
    # Traversal

    def do_tables tables
      @summary['run date'] = Time.now.to_s
      @summary['run elapsed time'] = RunTime.new
      until tables.nil?
        heading = tables.at 0, 0, 0
        unless heading.nil?
          begin
            fixture = find heading.text # using reflection
            fixture.summary = @summary
            fixture.counts = @counts
            fixture.do_table tables
          rescue Exception => e
            exception heading, e
          end
        end
        tables = tables.more
      end
    end

    def do_table table
      do_rows table.parts.more
    end

    def do_rows rows
      until rows.nil?
        more = rows.more
        do_row rows
        rows = more
      end
    end

    def do_row row
      do_cells row.parts
    end

    def do_cells cells
      i = 0
      until cells.nil?
        begin
          do_cell cells, i
        rescue Exception => e
          exception cells, e
        end
        cells = cells.more
        i += 1
      end
    end

    def do_cell cell, column_number
      ignore cell
    end

    # Reflection methods to find a fixture class from its name.

    def find name
      klass = find_class name
      raise "Fixture #{name} not found" unless klass
      raise "#{name} is not a fixture" unless klass < Fixture
      klass.new
    end

    # Try to load the named class. We first see if it's already loaded by looking
    # for the class name. If not, we convert the class name to a file name (by
    # changing '::' to '/', then try to require that file. We then look for the
    # constant again. This means that the class Example::Sqrt must be in the file
    # Example/sqrt.rb or the file example/sqrt.rb.
    def find_class name
      klass = find_constant name
      unless klass
        file_path = name.gsub(/::/, '/').gsub(/\./, '/')
        classname = basename = File::basename(file_path)
        if basename.index('$')
          basename_parts = basename.split(/\$/)
          basename = basename_parts[0]
          classname = basename_parts.join('::')
        end
        file_basename = basename.split(/([A-Z][^A-Z]+)/).delete_if {|e| e.empty?}.collect {|e| e.downcase!}.join('_')
        file_name = File::dirname(file_path) + '/' + file_basename
        begin
          require file_name
        rescue LoadError => e
          # puts e.message
          # puts e.backtrace
          raise "Couldn't find file #{file_name} or error in file #{file_name}"
        end
        klass_name =  File::dirname(file_path).split(%r{/}).collect {
          |e| e.index(/[A-Z]/).nil? ? e.capitalize : e
        }.join('::') + "::#{classname}"
        klass = find_constant klass_name
      end
      raise "Couldn't find class #{klass_name}" unless klass
      klass
    end

    def find_constant name
      parent = Object
      name.split(/::/).each do |next_class|
        parent = parent.const_get(next_class)
      end
      parent
    rescue Exception
      nil
    end

    # Annotation methods

    GREEN = '#cfffcf'
    RED = '#ffcfcf'
    GRAY = '#efefef'
    YELLOW = '#ffffcf'

    def right cell
      cell.add_to_tag('bgcolor="' + GREEN + '"')
      @counts.right += 1
    end

    def wrong cell, actual = nil
      cell.add_to_tag('bgcolor="' + RED + '"')
      @counts.wrong += 1
      unless actual.nil?
        cell.add_to_body(Fixture.label('expected') + '<hr>' + Fixture.escape(actual) + Fixture.label('actual'))
      end
    end

    def ignore cell
      cell.add_to_tag('bgcolor="' + GRAY + '"')
      @counts.ignores += 1
    end

    def exception cell, e
      stacktrace = e.backtrace.join "\n"
      message = e.message.gsub(/>/, '&gt;').gsub(/</, '&lt;')
      cell.add_to_body "<hr><font size=-2><pre>#{message}\n#{stacktrace}</pre></font>"
      cell.add_to_tag('bgcolor="' + YELLOW + '"')
      @counts.exceptions += 1
    end

    # Utility methods
    
    # This method was originally called counts, but the name has been
    # changed to avoid shadowing the counts accessor attribute.
    def totals
      @counts.to_s
    end

    def Fixture.label s
      ' <font size="-1" color="#c08080"><i>' + s + '</i></font>'
    end

    def Fixture.gray s
      ' <font color="#808080">' + s + '</font>'
    end

    def Fixture.escape s
      str = s.gsub(/&/, '&amp;').gsub(/</, '&lt;').gsub(/  /, ' &nbsp;')
      str.gsub(/\r\n/, '<br />').gsub(/\n\r/, '<br />').gsub(/\r/, '<br />').gsub(/\n/, '<br />')
    end

    # Originally, this method built the name of a Java method from multiple,
    # space-separated, downcased words, morphing them into a single camelcased
    # word. Ruby just needs those words to be joined with an underscore. For
    # historical reasons, this method still maintains its original name.
    def Fixture.camel name
      name.gsub(/ /, '_')
    end

    def parse string, klass
      return ParseDate.parsedate(string) if klass == ParseDate
      return ScientificDouble.value_of(string) if klass == ScientificDouble
      return string if klass == String
      nil
    end

    def check cell, adapter
      text = cell.text
      if text.empty?
        begin
          cell.add_to_body(Fixture.gray(adapter.to_s(adapter.get)))
        rescue Exception
          cell.add_to_body(Fixture.gray('error'))
        end
      elsif adapter.nil?
        ignore cell
      elsif text == 'error'
        begin
          result = adapter.invoke # TypeAdapter.invoke does not exist...
          wrong cell, adapter.to_s(result)
        rescue Exception => e
          right cell # no IllegalAccessException?
        end
      else
        begin
          result = adapter.get
          if adapter.equals(adapter.parse(text), result)
            right cell
          else
            wrong cell, adapter.to_s(result)
          end
        rescue Exception => e
          exception cell, e
        end
      end
    end

  end

end
