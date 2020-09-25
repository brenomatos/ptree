import os
import matplotlib.pyplot as plt
import time
import pandas as pd

MAX = 10
max_factor = 2
NUM_THREADS = 10
num_threads_factor = 1
results_list = []


for i in range(max_factor, MAX, max_factor):
    for j in range(num_threads_factor, NUM_THREADS+1, num_threads_factor):
        if(i > j): #precisamos de mais nos que threads
            start = time.time()
            os.system("./tree " + str(i) + " " + str(j))
            end = time.time()
            elapsed = end - start
            results_list.append([i,j,elapsed])

# # print(results_list)
df = pd.DataFrame(results_list, columns=["tree_size","num_threads","time"])
# # print(df)
# plt.plot(df["tree_size"],df["time"])
# # plt.plot(df["num_threads"], df["time"])
# plt.xlabel("time")
# plt.ylabel("num_threads")
# plt.title("time x num_threads")
# plt.show()


def plot_graphs(x_axis, y_axis):
    plt.plot(df[x_axis],df[y_axis])
    plt.xlabel(x_axis)
    plt.ylabel(y_axis)
    plt.title(str(x_axis) + " x " + str(y_axis))
    plt.show()

plot_graphs("tree_size", "time")