package com.cmdev.fit.eg;
import java.util.Date;
import java.text.*;

import fit.*;

public class SimpleRowFixTest extends RowFixture {
    
    private static DateFormat dateFormat = new SimpleDateFormat("MM/dd");
    private static final Date now = new Date();
    public static class SimpleRow {
        public int task;
        public String endDate = dateFormat.format(now);
        public String startDate = dateFormat.format(now);
    }

    public Class getTargetClass() {
        return SimpleRow.class;
    }
    
    
    public Object[] query() {
        SimpleRow[] rows = new SimpleRow[10];
        for (int i = 0; i < 10; i++) {
            rows[i] = new SimpleRow();
            rows[i].task = i;
        }
        return rows;
    }

    public Object parse (String s, Class type) throws Exception {
        if (type.equals(Date.class))    {return dateFormat.parse(s);}
        return super.parse (s, type);
    }
}