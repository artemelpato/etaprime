def Settings( **kwargs ):
    return {
        'flags': ['-pthread', '-std=c++14', '-m64', '-I/usr/share/ROOT/include', '-L/usr/share/ROOT/lib', '-lGui', '-lCore', '-lImt', '-lRIO', '-lNet', '-lHist', '-lGraf', '-lGraf3d', '-lGpad', '-lROOTVecOps', '-lTree', '-lTreePlayer', '-lRint', '-lPostscript', '-lMatrix', '-lPhysics', '-lMathCore', '-lThread', '-lMultiProc', '-lROOTDataFrame', '-pthread', '-lm', '-ldl', '-rdynamic'],
}
