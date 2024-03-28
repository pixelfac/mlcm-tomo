# Import libraries
from typing import List
import matplotlib.pyplot as plt
import numpy as np

def box(samples, iterations):

    file = open("test", "w")
    for i in range(len(samples)): # Specific size
        for j in range(len(samples[i])):
            file.write(f"{samples[i][j][0]},{samples[i][j][1]},{samples[i][j][2]}\n")

    # astra_cpu = np.zeros((len(samples), iterations))
    # astra_gpu = np.zeros((len(samples), iterations))
    # tigre = np.zeros((len(samples), iterations))

    # for i in range(len(samples)): # Specific size
    #     for j in range(len(samples[i])):
    #         astra_cpu[i][j] = (samples[i][j][0])
    #         astra_gpu[i][j] = (samples[i][j][1])
    #         tigre[i][j] = (samples[i][j][2])

        

    # data1 = [astra_cpu[0], astra_cpu[1], astra_cpu[2], astra_cpu[3], astra_cpu[4], astra_cpu[5], astra_cpu[6], astra_cpu[7], astra_cpu[8]]
    # data2 = [astra_gpu[0], astra_gpu[1], astra_gpu[2], astra_gpu[3], astra_gpu[4], astra_gpu[5], astra_gpu[6], astra_gpu[7], astra_gpu[8]]
    # data3 = [tigre[0], tigre[1], tigre[2], tigre[3], tigre[4], tigre[5], tigre[6], tigre[7], tigre[8]]
            
    # data1 = [astra_cpu[0], astra_cpu[1], astra_cpu[2], astra_cpu[3], astra_cpu[4], astra_cpu[5], astra_cpu[6]]
    # data2 = [astra_gpu[0], astra_gpu[1], astra_gpu[2], astra_gpu[3], astra_gpu[4], astra_gpu[5], astra_gpu[6]]
    # data3 = [tigre[0], tigre[1], tigre[2], tigre[3], tigre[4], tigre[5], tigre[6]]

    # data1 = [astra_cpu[0]]
    # data2 = [astra_gpu[0]]
    # data3 = [tigre[0]]


    # # Create the boxplot
    # fig, [ax1, ax2, ax3] = plt.subplots(1, 3)
    # ax1.boxplot(data1)
    # ax2.boxplot(data2)
    # ax3.boxplot(data3)

    # # Set the title and axis labels
    # ax1.set_title('Compute Ax at size 128')
    # ax1.set_xlabel('Astra CPU')
    # ax1.set_ylabel('Time (ms)')

    # ax2.set_title('Compute Ax at size 128')
    # ax2.set_xlabel('Astra GPU')
    # ax2.set_ylabel('Time (ms)')

    # ax3.set_title('Compute Ax at size 128')
    # ax3.set_xlabel('Tigre GPU')
    # ax3.set_ylabel('Time (ms)')

    # ax1.margins(x=0.5, y=0.2)
    # ax2.margins(x=0.5, y=0.2)
    # ax3.margins(x=0.5, y=0.2)

    # # fig = plt.figure(figsize =(10, 7))

    # # # Creating axes instance
    # # ax = fig.add_axes([0, 0, 1, 1])
    
    # # # Creating plot
    # # bp = ax.boxplot(data)
    
    # # show plot
    # plt.show()
    
    
   


if __name__ == "__main__":
    data = [[[893.3419,   2.6571,  13.6167,   0,    ],
  [891.75,     2.6267,  13.6225,   0,    ],
  [884.9146,   2.8019,  13.7327,   0,    ],
  [892.3646,   2.6623,  13.6038,   0,    ],
  [887.8172,   2.6328,  14.0162,   0.,    ]]]
    box(data)
