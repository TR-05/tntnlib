import numpy as np
from matplotlib import pyplot as plt



def aDist(t, vi, a):
    return vi*t + 0.5*a*t*t
def jDist(vi, ai, af, j):
    t = jerkTime(ai, af, j)
    return aDist(t, vi, (af-ai)/2)
def acelTime(vi, vf, a):
    return abs((vf - vi)/a)
def jerkTime(ai, af, j):
    return abs((af - ai)/j)
t1, t2, t3, tj1, tj2, tj3, tj4 = 0, 0, 0, 0, 0, 0, 0
dj1, dj2, dj3, dj4, d1, d2, d3 = 0, 0, 0, 0, 0, 0, 0
d = 40
vi, vf, vMax, a, j = 0, 0, 65, 120, 100000
time = 0
def profileTime():
    global t1, t2, t3, d1, d2, d3, vi, vf, vMax, a, dj1, dj2, dj3, dj4, j
    t1 = acelTime(vi, vMax, a)
    t3 = acelTime(vMax-vf, vi, a);

    dj1 = jDist(vi, 0, a, j)
    dj2 = jDist(vMax, a, 0, j)
    dj3 = jDist(vMax, 0, -a, j)
    dj4 = jDist(vf, -a, 0, j)
    tj1 = jerkTime(0, a, j)
    tj2 = jerkTime(a, 0, j)
    tj3 = jerkTime(0, -a, j)
    tj4 = jerkTime(-a, 0, j)

    d1 = aDist(t1, vi, a) + dj1 + dj2
    d3 = aDist(t3, vMax, a) + dj3 + dj4
    d2 = d - d1 - d3
    if d2 < 0:
        d2 = 0
    t2 = d2/vMax
    global time
    time = t1 + t2 + t3 + tj1 + tj2 + tj3 + tj4
    return time
def v(t):
    global t1, t2, t3, d1, d2, d3, vi, vf, vMax, a
    if t < t1:
        return vi + a*t
    elif t < t1+t2:
        return vMax
    else:
        return vMax - (t - (t1+t2))*a
def dist(t):
    global t1, t2, t3, d1, d2, d3, vi, vf, vMax, a, dj1, dj2, dj3, dj4, j
    if t < tj1:
        return vi*t +.5*(t*j)*t*t
    elif t < t1-tj2:
        return vi*t + .5*a*t*t
    elif t < t1:
        d1 - dj2 + (0)
    elif t < t1+t2:
        return d1+vMax*(t-t1)
    else:
        return d1+d2+vMax*(t-t1-t2)-.5*a*(t-t1-t2)*(t-t1-t2)
def acel(t):
    global t1, t2, t3, d1, d2, d3, vi, vf, vMax, a
    if t < t1:
        return a
    elif t < t1+t2:
        return 0
    else:
        return -a
    
i = 0;
profileTime()
dData = np.zeros((round(time*100.0), 2))
vData = np.zeros((round(time*100.0), 2))
aData = np.zeros((round(time*100.0), 2))


for row in dData:
    t = i/1000.0*10
    vData[i, 0] = t
    vData[i, 1] = v(t);
    dData[i, 0] = t
    dData[i, 1] = dist(t)
    aData[i, 0] = t
    aData[i, 1] = acel(t)

    i = i + 1;

vx, vy = vData.T
dx, dy = dData.T
ax, ay = aData.T


# Create a figure and a set of subplots
fig, axs = plt.subplots(3)

# Plot data on each subplot
axs[0].plot(dx, dy, label = "D vs T", color='gray', linestyle='none', linewidth = 3,
         marker='.', markerfacecolor='blue', markersize=3)
axs[0].set_title('D vs T')
axs[1].plot(vx, vy, label = "V vs T", color='gray', linestyle='none', linewidth = 3,
         marker='.', markerfacecolor='blue', markersize=3)
axs[1].set_title('V vs T')
axs[2].plot(ax, ay, label = "A vs T", color='gray', linestyle='none', linewidth = 3,
         marker='.', markerfacecolor='blue', markersize=3)
axs[2].set_title('A vs T')

#plt.xlim(0,20)
#plt.ylim(0,20)

plt.show()



#example graph settings
#plt.plot(x,y)
#plt.plot(x, y, label = "Line 1", color='gray', linestyle='solid', linewidth = 3,
#         marker='o', markerfacecolor='black', markersize=3)
#plt.plot(x, y, label = "Line 1", color='gray', linestyle='solid', linewidth = 3,
#         marker='o', markerfacecolor='black', markersize=3)