import sqlite3
import os.path
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path


ROOT_FOLDER = str(Path(__file__).parent.parent.absolute())
CWD = os.getcwd()
db_path = os.path.join(ROOT_FOLDER, 'Results')

conn = sqlite3.connect(db_path)
c = conn.cursor()
label = 'sh1'
alg = "svt"
sql = "SELECT * FROM SVT WHERE Label =?"

graphArray = []
limit = 0.3

for row in c.execute(sql, [(label)]):
    startingInfo = str(row).replace(')','').replace('(','').replace('u\'','').replace("'","")
    splitInfo = startingInfo.split(',')
    graphArrayAppend = splitInfo[3] + splitInfo[4] + splitInfo[6]
    graphArray.append(graphArrayAppend)

data = np.loadtxt(graphArray)
data = data[data[:, 0].argsort()] # Sort by truncation
data2 = np.split(data, np.unique(data[:, 0], return_index=True)[1][1:])

for trunc in data2:
    trunc = trunc[trunc[:, 1].argsort()] # sort by max_iter
    k, max_iter, rmse = trunc.transpose()
    if rmse[0] > limit:
        plt.plot(max_iter, rmse, 'o-', alpha=0.2, lw=1, markersize=3)
    else:
        plt.plot(max_iter, rmse, 'o-', alpha=0.8, label=k[0], lw=1, markersize=3)
        plt.annotate(k[0], (max_iter[-1], rmse[-1]), fontsize=5)


plt.ylim(0,limit)
plt.title(f"{alg}-sh")
# plt.legend(prop={'size': 3})
plt.savefig(os.path.join(ROOT_FOLDER, "Graphs",  f'{alg}-sh-zoomed'), dpi=600)



