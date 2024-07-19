import os

f = open("CMakeLists.txt").readlines()
d = []

for r,idc,files in os.walk("src"):
    if files.__len__() == 0: continue 

    d.extend(os.path.join(r,file)+"\n" for file in files if file.endswith(".cpp") or file.endswith(".hpp"))

b, e = f.index("    # Fill start\n"), f.index("    # Fill end\n")

f = f[0:b+1]+d+f[e:len(f)]

open("CMakeLists.txt","w").writelines(f)
    
