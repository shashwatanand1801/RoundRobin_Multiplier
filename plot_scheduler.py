import matplotlib.pyplot as plt
import os
ps_coord=[]
tatp1_coord = []
wtp1_coord = []
tatp2_coord =[]
wtp2_coord = []
cst_coord = []
tq=1
# plt.ylim(ymax = 1, ymin = 0)

with open('data3.txt') as f:
    for line in f:
        ps, tatp1 , wtp1, tatp2, wtp2,cst = line.split()
        ps_coord.append(ps)
        tatp1_coord.append(tatp1)
        wtp1_coord.append(wtp1)
        tatp2_coord.append(tatp2)
        wtp2_coord.append(wtp2)
        cst_coord.append(cst)
   
ps_coord.reverse()
tatp1_coord.reverse()
wtp1_coord.reverse()
tatp2_coord.reverse()
wtp2_coord.reverse()
cst_coord.reverse()
ps_coord=[float(x) for x in ps_coord]
tatp1_coord=[float(x) for x in tatp1_coord]
wtp1_coord=[float(x) for x in wtp1_coord]
tatp2_coord=[float(x) for x in tatp2_coord]
wtp2_coord=[float(x) for x in wtp2_coord]
cst_coord=[float(x) for x in cst_coord]
# print(x_coord[14],y_coord[14])
plt.scatter(ps_coord, tatp1_coord, label = "TATP1")
plt.legend(['TATP1'])
plt.xlabel("Log of problem size")
plt.ylabel("Time in milliseconds")
plt.savefig("tatp1_tq"+str(tq)+".png")
plt.close()
# plt.show()
plt.scatter(ps_coord, wtp1_coord, label = "WTP1")
plt.legend(['WTP1'])
plt.xlabel("Log of problem size")
plt.ylabel("Time in milliseconds")
plt.savefig('wtp1_tq'+str(tq)+".png")
plt.close()
# plt.show()
plt.scatter(ps_coord, tatp2_coord, label = "TATP2")
plt.legend(['TATP2'])
plt.xlabel("Log of problem size")
plt.ylabel("Time in milliseconds")
plt.savefig('tatp2_tq'+str(tq)+".png")
plt.close()
# plt.show()
plt.scatter(ps_coord, wtp2_coord, label = "WTP2")
plt.legend(['WTP2'])
plt.xlabel("Log of problem size")
plt.ylabel("Time in milliseconds")
plt.savefig('wtp2_tq'+str(tq)+".png")
plt.close()
# plt.show()
plt.scatter(ps_coord, cst_coord, label = "CST")
plt.ylim([0,10])
plt.legend(['CST'])
plt.xlabel("Log of problem size")
plt.ylabel("Time in milliseconds")
plt.savefig("cst_tq"+str(tq)+".png")
plt.close()
# plt.show()
