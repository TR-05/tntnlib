import matplotlib.pyplot as plt


# Create a figure and an axis
fig, ax = plt.subplots()

time = 5
refreshTime = 50/1000
num = int(time / refreshTime)

# Initialize your data
xData = [0.0]*num
yData = [0.01]*num
# Create a line object that will be updated
line, = ax.plot(yData)

# Initialize the maximum y-value
max_y = 0




class Line:
    def __init__(self, height):
        global num
        self.y_data=[height]*num
        self.x_data=[0]*num
        self.line, = ax.plot(self.y_data)
        self.line.set_xdata(self.x_data)
        self.line.set_ydata(self.y_data)
    def update_line(self):
        global xData
        self.line.set_xdata(xData)
        self.line.set_ydata(self.y_data)

# Example usage
xAxis = Line(0)
target = Line(90)





def update():
    global max_y  # Declare max_y as global so we can modify it
    global num
    # Limit the length of xData and yData to 100

    while len(xData) > num:
        xData.pop(0)
    while len(yData) > num:
        yData.pop(0)

    xAxis.update_line()
    target.update_line()
    # Update the line object
    line.set_ydata(yData)
    line.set_xdata(xData)

    # Update max_y if necessary
    current_max_y = max(yData)
    if current_max_y > max_y:
        max_y = current_max_y

    # Adjust the plot limits
    ax.relim()
    ax.autoscale_view()

    # Set the y-limit to max_y
    #ax.set_ylim(bottom=-max_y, top=max_y)

    # Redraw the plot
    plt.draw()
    plt.pause(0.01)