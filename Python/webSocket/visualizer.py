import matplotlib.pyplot as plt
import numpy as np


# Create a figure and an axis
fig, ax = plt.subplots()

time = 10
refreshTime = 50/1000
num = int(time / refreshTime)

# Initialize your data
tData = np.linspace(-time, 0, num).tolist()
yData = [0.01]*num
# Create a line object that will be updated
line, = ax.plot(yData)

# Initialize the maximum y-value
max_y = 0




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
        self.line.set_ydata([self.height]*num)

# Example usage
xAxis = Line(0)
target = Line(90)
lTarget= Line(-90)



def limitData():
    global num, time
    ''' while (tData[-1] - tData[0] > (time+.2)):
        num = num - 1
        #print("t", tData[-1] - tData[0])
        #print ("num", num)
        while len(tData) > num:
            tData.pop(0)
        while len(yData) > num:
            yData.pop(0)'''
    while len(tData) > num:
        tData.pop(0)
    while len(yData) > num:
        yData.pop(0)
    line.set_ydata(yData)
    line.set_xdata(tData)

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