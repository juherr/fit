#!/bin/sh
#(c) 2002, Ward Cunningham

echo "Content-type: text/html"
echo 

(cd Release
export TERM=vt100
export TMPDIR=/usr/tmp

# real work happens in next two lines
curl $HTTP_REFERER >Documents/$$
FileRunner Documents/$$ Reports/$$

cat Reports/$$
rm Documents/$$ Reports/$$) 2>run.log
