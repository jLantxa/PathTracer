import os

spp = [2**x for x in range(10)]

for n in spp:
    print("spp = " + str(n))
    filename = "pic/test_" + str(n) + ".ppm"
    cmd = "./build/PathTracerTest 800 600 90 " + str(n) + " 5 " + filename
    print(cmd)
    os.system(cmd)
