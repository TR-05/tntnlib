import matplotlib.pyplot as plt
import numpy as np


# Create a figure and an axis
fig, axs = plt.subplots(nrows=2, ncols=2)



#axs[0].set_title('Data over time')
axs[0,0].set_xlabel('Time (s)')
axs[0,0].set_ylabel('d1')
axs[0,0].grid(True)
#axs[1].set_title('Data over time')
axs[1,0].set_xlabel('Time (s)')
axs[1,0].set_ylabel('d2')
axs[1,0].grid(True)
#axs[2].set_title('Data over time')
axs[0,1].set_xlabel('Time (s)')
axs[0,1].set_ylabel('d3')
axs[0,1].grid(True)
axs[1,1].set_xlabel('x (inch)')
axs[1,1].set_ylabel('y (inch)')
axs[1,1].grid(True)

#ax.annotate('Peak', xy=(1.5, 1), xytext=(3, 1.5), arrowprops=dict(facecolor='black', shrink=0.05))
#ax.legend()

time = 20
refreshTime = 10/1000
num = int(time / refreshTime)

# Initialize your data
tData = np.linspace(-time, 0, num).tolist()
backUptData = np.linspace(-time, 0, num).tolist()

p1Data = np.linspace(0, 0, num).tolist()
p2Data = np.linspace(0, 0, num).tolist()
p3Data = np.linspace(0, 0, num).tolist()
backUpP1Data = np.linspace(0, 0, num).tolist()
backUpP2Data = np.linspace(0, 0, num).tolist()
backUpP3Data = np.linspace(0, 0, num).tolist()

xData = [0]
yData = [0]

coords, = axs[1,1].plot(xData, yData, 'o', color='black', markersize=1)

def updateLine1(newT, newP1, newP2, newP3, x, y):
    global backUptData, backUpP1Data
    backUptData.append(newT)
    backUpP1Data.append(newP1)
    backUpP2Data.append(newP2)
    backUpP3Data.append(newP3)
    xData.append(x)
    yData.append(y)
    coords.set_xdata(xData)
    coords.set_ydata(yData)

class Line:
    def __init__(self, height, plotx, ploty):
        global num
        self.height = height
        self.y_data=[height]*num
        self.x_data=[0]*num
        self.line, = axs[plotx,ploty].plot(self.x_data, self.y_data,linestyle='dashed', color='black')
        self.line.set_xdata(self.x_data)
        self.line.set_ydata(self.y_data)
    def update_line(self):
        global tData
        self.line.set_xdata(tData)
        self.line.set_ydata([self.height]*len(tData))


class dataOverTimeLine:
    def __init__(self, plotx, ploty):
        global num, tData
        self.line, = axs[plotx,ploty].plot(tData, np.linspace(0, 0, num).tolist(), linestyle='solid', color='red', linewidth=2)
        self.max_y = 0
        self.min_y = 0
    def update_line(self, data):
        global tData
        self.line.set_xdata(tData)
        self.line.set_ydata(data)
        # Update max_y if necessary
        current_max_y = max(data)
        current_min_y = min(data)
        if current_max_y > self.max_y:
            self.max_y = current_max_y
        if current_min_y < self.min_y:
            self.min_y = current_min_y


# Create a line object that will be updated
line1 = dataOverTimeLine(0,0)
line2 = dataOverTimeLine(1,0)
line3 = dataOverTimeLine(0,1)

# Example usage
xAxis1 = Line(0, 0,0)
target1 = Line(3.6, 0,0)
targetB1 = Line(0, 0,0)
# Example usage
xAxis2 = Line(0, 1,0)
target2 = Line(3.6, 1,0)
targetB2= Line(0, 1,0)
# Example usage
xAxis3 = Line(0, 0,1)
target3 = Line(3.6, 0,1)
targetB3 = Line(0, 0,1)



def limitData():
    global num, time, tData, p1Data, backUptData, backUpP1Data 
    while backUptData[-1] - backUptData[-num] > time:
        num = num - 1
    while backUptData[-1] - backUptData[-num] < time:
        num = num + 1
    tData = backUptData[-num:]
    p1Data = backUpP1Data[-num:]
    p2Data = backUpP2Data[-num:]
    p3Data = backUpP3Data[-num:]

    print(num)
    #print(f"tData ({backUptData[-1]:.3f}, {backUptData[-2]:.3f}, {backUptData[-3]:.3f}, {backUptData[-4]:.3f}, {backUptData[-5]:.3f}, {backUptData[-6]:.3f})")
    line1.update_line(p1Data)
    line2.update_line(p2Data)
    line3.update_line(p3Data)

def update():
    global max_y, min_y # Declare max_y as global so we can modify it
    global num, time
    limitData()
    xAxis1.update_line()
    target1.update_line()
    targetB1.update_line()

    xAxis2.update_line()
    target2.update_line()
    targetB2.update_line()
    
    xAxis3.update_line()
    target3.update_line()
    targetB3.update_line()
    # Update the line object

    # Set the y-limit to max_y
    axs[0,0].set_ylim(bottom=line1.min_y*1.2, top=line1.max_y*1.2)
    axs[1,0].set_ylim(bottom=line2.min_y*1.2, top=line2.max_y*1.2)
    axs[0,1].set_ylim(bottom=line3.min_y*1.2, top=line3.max_y*1.2)
    axs[1,1].set_xlim(left=0, right=144)
    axs[1,1].set_ylim(bottom=0, top=144)

    # Adjust the plot limits
    axs[0,0].relim()
    axs[1,0].relim()
    axs[0,1].relim()
    axs[1,1].relim()

    axs[0,0].autoscale_view()
    axs[1,0].autoscale_view()
    axs[0,1].autoscale_view()
    axs[1,1].autoscale_view()

    # Redraw the plot
    plt.draw()
    plt.pause(0.001)