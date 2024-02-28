import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

# Read the data from the file
data = pd.read_csv('Datasets/real_world/drift/drift10_normal.txt', header=None, sep=' ')

data = data.iloc[199:500, 0:3]

plt.figure(figsize=(4, 2))
plt.margins(x=0)
plt.yticks(None)

# Plot the graph using seaborn
g = sns.lineplot(data=data, legend=False, lw=0.5, dashes=False)
g.set(xticklabels=[])
plt.tight_layout()
plt.savefig("_data/drift_normal.png")
