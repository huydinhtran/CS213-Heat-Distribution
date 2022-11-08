import matplotlib.pyplot as plt

with open('heatseq_output.txt') as file:
    array2d = [[float(digit) for digit in line.split()] for line in file]

plt.imshow(array2d, cmap='hot', interpolation='nearest')
plt.show()
plt.savefig("heatseq_graph.png")

with open('heatpar_output.txt') as file:
    array2d = [[float(digit) for digit in line.split()] for line in file]

plt.imshow(array2d, cmap='hot', interpolation='nearest')
plt.show()
plt.savefig("heatpar_graph.png")
