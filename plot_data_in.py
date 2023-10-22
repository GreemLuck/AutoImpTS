import os
import matplotlib.pyplot as plt
import pandas as pd

folder_path = "_data/missingblock"
output_path = "_data/in"

for filename in os.listdir(folder_path):
    if filename.endswith(".txt"):
        data = pd.read_csv(os.path.join(folder_path, filename), sep="  ", header=None,
                           na_values="nan")
        data[0].plot(linewidth=0.5)
        plt.ylim(-5,5)
        plt.savefig(os.path.join(output_path, filename + ".png"))
        plt.clf()