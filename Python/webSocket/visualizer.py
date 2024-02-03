import matplotlib.pyplot as plt
import numpy as np


# Create a figure and an axis
fig, ax = plt.subplots()

time = 10
refreshTime = 50/1000
num = int(time / refreshTime)

# Initialize your data
tData = np.linspace(-time, 0, num).tolist()
yData = tData

dataMatrix = np.array([[tData], [tData], [tData], [tData]])

# Create a line object that will be updated
line, = ax.plot(yData)

# Initialize the maximum y-value
max_y = 0

def updateLine1(newX, newY):
    global backUptData, backUpyData, unrelatedData
    #backUptData.append(newX)
    #backUpyData.append(newX+1)
    dataMatrix[0].append(newX)
    dataMatrix[1].append(newY)


class Line:
    def __init__(self, height):
        global num
        self.height = height
        self.y_data=[height]*num
        self.x_data=[0]*num
        self.line, = ax.plot(self.y_data)
        self.line.set_xdata(self.x_data)
        self.line.set_ydata(self.y_data)
    def update_line(self):
        global tData
        self.line.set_xdata(tData)
        self.line.set_ydata([self.height]*len(tData))

# Example usage
xAxis = Line(0)
target = Line(90)
lTarget= Line(-90)



def limitData():
    global num, time, tData, yData, backUptData, backUpyData, unrelatedData
    tData = dataMatrix[0][-100:]
    yData = dataMatrix[1][-100:]
    print(f"tData ({dataMatrix[0][-1]:.3f}, {dataMatrix[0][-2]:.3f}, {dataMatrix[0][-3]:.3f}, {dataMatrix[0][-4]:.3f}, {dataMatrix[0][-5]:.3f}, {dataMatrix[0][-6]:.3f})")
    line.set_data(tData, yData)

def update():
    global max_y  # Declare max_y as global so we can modify it
    global num, time
    limitData()
    xAxis.update_line()
    target.update_line()
    lTarget.update_line()
    # Update the line object


    # Update max_y if necessary
    current_max_y = max(yData)
    if current_max_y < abs(min(yData)):
        current_max_y = abs(min(yData))
    if current_max_y > max_y:
        max_y = current_max_y
    # Adjust the plot limits
    ax.relim()
    ax.autoscale_view()
    # Set the y-limit to max_y
    ax.set_ylim(bottom=-max_y*1.2, top=max_y*1.2)
    # Redraw the plot
    plt.draw()
    plt.pause(0.001)