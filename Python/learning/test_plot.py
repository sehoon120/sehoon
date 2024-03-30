
import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0,6,0.1)
y1 = np.sin(x)
y2 = np.cos(x)

plt.plot(x,y1,label='sin y1')
plt.plot(x,y2,linestyle = '--',label='cos y2')
plt.xlabel('x')
plt.ylabel("y's")
plt.title('sin & cos')
plt.legend()        # What is it??


plt.show()
