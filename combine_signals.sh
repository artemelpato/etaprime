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

cent1=$1
cent2=$2
pt1=$3
pt2=$4
integral_min=0.7
integral_max=0.91
 
[ -e temp/CombinedSignals.root ] && rm -f temp/CombinedSignals.root

root -l -b -q "GetSignals.C($pt1, $pt2, $integral_min, $integral_max)"
root -l -q "CombineSignals3.C($cent1, $cent2, 0, 2)"

echo "=======================DONE====================="

root -l -e 'new TBrowser'
