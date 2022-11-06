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
    value = np.arange(16, dtype=int) # Changed from 32
    values_to_send = np.arange(4, dtype=int)

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
    
    #counter = 0
    #with open('test.txt', 'w') as f:
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
                counter = 0
                for i in range(len(value), __step=4):
                    values_to_send[counter] = value[i] + (value[i + 1] << 8) + (value[i + 2] << 16) + (value[i + 3] << 24)
                    values_to_send[counter] = values_to_send[counter] - 5000
                    counter = counter + 1

                connection.send(values_to_send[0])
                connection.send(values_to_send[1])
                connection.send(values_to_send[2])
                connection.send(values_to_send[3])

                """
                timer = timeit.default_timer() - start
                f.write(str(timer))
                f.write('\n')
                """
    except KeyboardInterrupt:
        #f.close()
        print("Stopped by user")


def animate(data, lines, connection):
    # Receive data
    encoder_1 = connection.recv()
    encoder_2 = connection.recv()
    encoder_3 = connection.recv()
    encoder_4 = connection.recv()

    # New version of array to assign to updated_data
    updated_data_encoder_1 = data[0]
    updated_data_encoder_2 = data[1]
    updated_data_encoder_3 = data[2]
    updated_data_encoder_4 = data[3]

    updated_data_encoder_1[-1] = encoder_1
    updated_data_encoder_2[-1] = encoder_2
    updated_data_encoder_3[-1] = encoder_3
    updated_data_encoder_4[-1] = encoder_4

    lines[0].set_ydata(updated_data_encoder_1)
    lines[1].set_ydata(updated_data_encoder_2)
    lines[2].set_ydata(updated_data_encoder_3)
    lines[3].set_ydata(updated_data_encoder_4)

    # Transfer new version of data to the data array
    for j in range(len(updated_data_encoder_1) - 1):
        data[0][j] = updated_data_encoder_1[j + 1]
        data[1][j] = updated_data_encoder_2[j + 1]
        data[2][j] = updated_data_encoder_3[j + 1]
        data[3][j] = updated_data_encoder_4[j + 1]

    return lines[0], lines[1], lines[2], lines[3],

def plot_data(connection):
    # Plot data
    fig, ax = plt.subplots()
    ax.set_ylim(-4000.0, 4000.0)

    t = np.arange(0, 10, (1/10))
    data_encoder_1 = np.arange(0, 100, 1)
    data_encoder_2 = np.arange(0, 100, 1)
    data_encoder_3 = np.arange(0, 100, 1)
    data_encoder_4 = np.arange(0, 100, 1)

    data = []
    data.append(data_encoder_1)
    data.append(data_encoder_2)
    data.append(data_encoder_3)
    data.append(data_encoder_4)

    # Create line object to contain both lines to plot
    line_encoder_1, = ax.plot(t, data_encoder_1)
    line_encoder_2, = ax.plot(t, data_encoder_2)
    line_encoder_3, = ax.plot(t, data_encoder_3)
    line_encoder_4, = ax.plot(t, data_encoder_4)

    lines = []
    lines.append(line_encoder_1)
    lines.append(line_encoder_2)
    lines.append(line_encoder_3)
    lines.append(line_encoder_4)

    ani = animation.FuncAnimation(
        fig, animate,fargs=(data, lines, connection), interval=0.5, blit=True)

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