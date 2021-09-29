#!/bin/sh

# Just give first and last centrality (vertex) as arguments
# In our case:
# 0 ---  0-10%
# 1 --- 10-20%
# 2 --- 20-30%
# 3 --- 30-40%
# 4 --- 40-50%
# 5 --- 50-60%
# 6 --- 60-70%
# 7 --- 70-80%
# 8 --- 80-90%
# 9 --- 90-100%

[ -e temp/CombinedCabanaBoy.root ] && rm -f temp/CombinedCabanaBoy.root

root -l -b -q 'CombineCabanaBoy.C(6, 8, 0, 2)'

echo "DONE"
