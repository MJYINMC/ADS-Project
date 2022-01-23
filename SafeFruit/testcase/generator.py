 import random
f = open("./testcase.txt", "w")
n = int(input("Number of tips > "))
m = int(input("Number of fruits > "))
f.write("%d %d\n"%(n, m))
l1 = []
l2 = []
l3 = []

i = 0
while i < m:
    x = random.randint(0, 999)
    y = random.randint(1, 1000)
    if((x in l3)):
        continue;
    i += 1
    l3.append(x)
    l2.append("%03d %d\n"%(x, y))

i = 0
while i < n:
    x = random.randint(0, 999)
    y = random.randint(0, 999)
    if((x not in l3) or (y not in l3)  or (x == y)):
        continue
    i += 1
    l1.append("%03d %03d\n"%(x, y))
f.writelines(l1)
f.writelines(l2)
