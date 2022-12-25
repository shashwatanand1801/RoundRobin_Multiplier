import matplotlib.pyplot as plt
import os
x_coord = []
y_coord = []

with open('data1.txt') as f:
    for line in f:
        x, y = line.split()
        x_coord.append(x)
        y_coord.append(y)
   
x_coord.reverse()
y_coord.reverse()
x_coord=[float(x) for x in x_coord]
y_coord=[float(x) for x in y_coord]
print(x_coord[14],y_coord[14])
plt.scatter(x_coord, y_coord)
plt.savefig('name1.png')
# os.system('eog name.png &')	
plt.show()
