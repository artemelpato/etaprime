#!/bin/sh

cent1=$1
cent2=$2
pt1=$3
pt2=$4

infile=cabanaboy-runs3/cabanaboy-merged.root
combined_histos_file=temp/CombinedHistos.root
outfile=temp/IntegratedAfterCombining.root

echo $infile
[ -e $combined_histos_file ] && rm -f $combined_histos_file

root -l -b -q "CombineHistos.C($cent1, $cent2, 0, 2, \"$infile\", \"$combined_histos_file\")"
root -l -b -q "IntegrateAfterCombining.C($cent1, $cent2, 0, 2, $pt1, $pt2, \"$combined_histos_file\", \"$outfile\")"

echo =================== DONE =================

root -l -e 'new TBrowser'


