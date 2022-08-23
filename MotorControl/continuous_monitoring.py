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
    value = np.arange(64, dtype=int)
    values_to_send = np.arange(2, dtype=int)

    # Check if serial port is open
        # Sets the parameters if not open
    portOpen = False
    while not portOpen:
        try:
            # Make sure COM port is correct (see in Gestionnaire de périphériques)
            arduino = serial.Serial(port='COM7', baudrate=1000000, timeout=None, xonxoff=False, rtscts=False,
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
                    
                    connection.send(values_to_send[0])
                    counter = counter + 1

                    timer = timeit.default_timer() - start
                    f.write(str(timer))
                    f.write('\n')
        except KeyboardInterrupt:
            f.write('\n')
            f.write(str(counter))
            f.close()



def animate(i, data, line, connection):
    # Receive data
    pos_x = connection.recv()

    # New version of array to assign to updated_data
    updated_data = data
    updated_data[-1] = pos_x
    line.set_ydata(updated_data)  # update the data.

    # Transfer new version of data to the data array
    for j in range(len(updated_data) - 1):
        data[j] = updated_data[j + 1]

    return line,

def plot_data(connection):
    # Plot data
    fig, ax = plt.subplots()
    ax.set_ylim(0.0, 4096.0)

    t = np.arange(0, 10, (1/10))
    data = np.arange(0, 100, 1)
    line, = ax.plot(t, data)

    ani = animation.FuncAnimation(
        fig, animate,fargs=(data, line, connection), interval=0.1, blit=True)

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