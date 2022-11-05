from multiprocessing import Process
from multiprocessing import Pipe
import numpy as np
import serial
import timeit
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# generate work
def sender(connection):
    print('Sender: Running', flush=True)
    value = np.arange(32, dtype=int)
    values_to_send = np.arange(2, dtype=int)

    # Check if serial port is open
        # Sets the parameters if not open
    portOpen = False
    while not portOpen:
        try:
            # Make sure COM port is correct (see in Gestionnaire de périphériques)
            arduino = serial.Serial(port='COM4', baudrate=1000000, timeout=None, xonxoff=False, rtscts=False,
                                    dsrdtr=False)
            # Clear the serial buffer (input and output)
            arduino.flushInput()
            arduino.flushOutput()
            portOpen = True
            print('Found serial port', flush=True)
        except:
            pass
    
    counter = 0
    with open('test.txt', 'w') as f:
        try:
            while True:
                start = timeit.default_timer()
                bytes_to_read = arduino.in_waiting

                if bytes_to_read != 0:
                    data = arduino.read(bytes_to_read)
                    # Insert the bytes read in the queue
                    for i in range(len(data)):
                        value[i] = data[i]
                    
                    # Recompose values
                    values_to_send[0] = value[0] + (value[1] << 8)
                    values_to_send[1] = value[2] + (value[3] << 8)
                    
                    connection.send(values_to_send[0])
                    connection.send(values_to_send[1])
                    counter = counter + 1

                    timer = timeit.default_timer() - start
                    f.write(str(timer))
                    f.write('\n')
        except KeyboardInterrupt:
            f.write('\n')
            f.write(str(counter))
            f.close()



def animate(i, data_x, data_y, lines, connection):
    # Receive data
    pos_x = connection.recv()
    pos_y = connection.recv()

    # New version of array to assign to updated_data
    updated_data_x = data_x
    updated_data_y = data_y

    updated_data_x[-1] = pos_x
    updated_data_y[-1] = pos_y

    lines[0].set_ydata(updated_data_x)
    lines[1].set_ydata(updated_data_y)

    # Transfer new version of data to the data array
    for j in range(len(updated_data_x) - 1):
        data_x[j] = updated_data_x[j + 1]
        data_y[j] = updated_data_y[j + 1]

    return lines[0], lines[1],

def plot_data(connection):
    # Plot data
    fig, ax = plt.subplots()
    ax.set_ylim(0.0, 4096.0)

    t = np.arange(0, 10, (1/10))
    data_x = np.arange(0, 100, 1)
    data_y = np.arange(0, 100, 1)

    # Create line object to contain both lines to plot
    line_x, = ax.plot(t, data_x)
    line_y, = ax.plot(t, data_y)
    lines = []
    lines.append(line_x)
    lines.append(line_y)

    ani = animation.FuncAnimation(
        fig, animate,fargs=(data_x, data_y, lines, connection), interval=0.1, blit=True)

    plt.show()


# entry point
if __name__ == '__main__':
    # create the pipes
    conn_receiver, conn_sender = Pipe(duplex=True)
    conn_receiver_plot_data, conn_sender_plot_data = Pipe()

    # start the sender
    sender_process = Process(target=sender, args=(conn_sender,))
    sender_process.start()
    # start the data plotting receiver process
    plotting_data_process = Process(target=plot_data, args=(conn_receiver,))
    plotting_data_process.start()

    # wait for all processes to finish
    sender_process.join()
    plotting_data_process.join()