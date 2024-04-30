import matplotlib.pyplot as plt

number_of_points = 10
def BuildGraph(number_of_points, index):
    global axs
    file = open(f"../Description/Results/graph_building_total_{number_of_points}.txt")

    data = [[], [], [], []]
    line = file.readline()
    while line:
        number_of_rectangles = int(line)
        data[0].append(number_of_rectangles)
        line = file.readline()
        total_brute_force = float(line.split()[2])
        data[1].append(total_brute_force)
        line = file.readline()
        total_map = float(line.split()[2])
        data[2].append(total_map)
        line = file.readline()
        total_tree = float(line.split()[2])
        data[3].append(total_tree)
        line = file.readline()

    file.close()

    axs[index].plot(data[0], data[1], label='BruteForce')
    axs[index].plot(data[0], data[2], label='MapAlgorithm')
    axs[index].plot(data[0], data[3], label='PersistentST')
    if index != 2:
        axs[index].set_yscale("log")
    axs[index].set_xlabel('Number of rectangles')
    axs[index].set_ylabel('Time in milliseconds')
    axs[index].set_title(f'Total time, M = {number_of_points}', fontsize='16')
    axs[index].legend()


fig, axs = plt.subplots(1, 3, figsize=(15, 5))
BuildGraph(10, 0)
BuildGraph(1000, 1)
BuildGraph(1000000, 2)
plt.savefig('../Description/Results/plot.png')
plt.show()
