import sys
import os
import sqlite3
from pathlib import Path
import matplotlib.pyplot as plt

ROOT_FOLDER = str(Path(__file__).parent.parent.absolute())

def main(alg, label):
    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    result = cursor.execute(f"SELECT Dataset, Ticks, Max_iter, Truncation, Rmse, Runtime "
                            f"FROM {alg} WHERE Label='{label}'").fetchall()

    dataset, tick, max_iter, truncation, rmse, runtime = tuple(zip(*result))

    plt.figure(figsize=(16,9))
    plt.plot(max_iter, rmse)
    plt.savefig(os.path.join(ROOT_FOLDER, "Graphs", "testsh"))


if __name__ == "__main__":
    main(*sys.argv[1:])