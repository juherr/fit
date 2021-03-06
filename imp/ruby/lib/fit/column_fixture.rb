# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

require 'fit/fixture'
require 'fit/type_adapter'

module Fit

  class ColumnFixture < Fixture

    attr_accessor :column_bindings, :has_executed
    protected :column_bindings, :column_bindings=, :has_executed, :has_executed=

    # Traversal

    def do_rows rows
      bind rows.parts
      super(rows.more)
    end

    def do_row row
      @has_executed = false
      begin
        reset
        super
        execute if not @has_executed
      rescue Exception => e
        exception row.leaf, e
      end
    end

    def do_cell cell, column_index
      adapter = @column_bindings[column_index]
      begin
        text = cell.text
        if text.empty?
          check cell, adapter
        elsif adapter.nil?
          ignore cell
        elsif adapter.is_output?
          check cell, adapter
        else
          adapter.set(adapter.parse(text))
        end
      rescue Exception => e
        exception cell, e
      end
    end

    def check cell, adapter
      unless @has_executed
        begin
          execute
        rescue Exception => e
          exception cell, e
        end
        @has_executed = true
      end
      super
    end

    def reset
      # about to process first cell of row
    end

    def execute
      # about to process first method call of row
    end

    # Utility

    protected

    def bind heads
      @column_bindings = []
      until heads.nil?
        name = heads.text
        begin
          if name.empty?
            @column_bindings << nil
          elsif name =~ /\(\)$/
            @column_bindings << TypeAdapter.for(self, name[0..-3], true)
          else
            @column_bindings << TypeAdapter.for(self, name, false)
          end
        rescue Exception => e
          exception heads, e
        end
      heads = heads.more
      end
    end

  end

end
