# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

require 'fit/row_fixture'
require 'set'

module Eg

  class ColumnIndex < Fit::RowFixture

    def do_rows rows
      @rows = rows
      super
    end

    def query
      # first find what classes are mentioned in the table...
      names = Set.new
      column_index = 0
      cell = @rows.parts
      until cell.nil?
        break if cell.text == 'className'
        column_index += 1
        cell = cell.more
      end
      row = @rows.more
      until row.nil?
        names.add row.at(0, column_index).text
        row = row.more
      end
      # ...then find the columns in these classes
      columns = []
      names.each do |name|
        name = 'Object' if name == 'java.lang.Object' # an hack to find the right Object class
        obj = find_class(name).new
        attributes = obj.methods - Object.new.methods
        setters = attributes.dup.delete_if {|a| a[-1..-1] != "="}
        attributes -= setters
        setters.each do |s|
          getter = s[0..-2]
          if attributes.include? getter
            attributes.delete getter
            columns << Column.create(obj, getter)
          end
        end
        attributes.each do |m|
          meth = obj.method(m.to_sym)
          columns << Column.create(obj, "#{m}()") if meth.arity.zero?
        end
      end
      columns 
    end

    def parse string, klass
      return parse_class(string) if klass == Class
      super
    end

    def parse_class name
      case name
        when 'byte', 'short', 'int', 'long' then Integer
        when 'float', 'double' then Float
        when 'char' then String
        else find_class(name)
      end
    end

    # Helper class

    class Column
      attr_accessor :column, :class_name, :column_name, :column_type
      def Column.create obj, attribute
        c = new
        c.column = attribute
        klass_name = obj.class.to_s
        case klass_name
          when 'Eg::Music::Music' then c.class_name = 'eg.music.Music'
          when 'Eg::ColumnIndex::Column' then c.class_name = 'eg.ColumnIndex$Column'
        end
        column_name = attribute.to_s
        column_name_parts = column_name.split(/_/)
        c.column_name = column_name_parts.shift
        column_name_parts.each {|name| c.column_name += name.capitalize}
        c.column_type = 'Undefined in Ruby'
        c
      end
    end
    
  end

end
