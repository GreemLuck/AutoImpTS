import sys
import os
import sqlite3
from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np
import math
import ts_algorithms
import random
from tqdm import tqdm

ROOT_FOLDER = str(Path(__file__).parent.parent.absolute())

0
def sh_execute(algorithm=None, dataset=None,
               sample_size=5,
               distribution={},
               resource_name='max_iter', resource_max=100, resource_min=5,
               keep_losers=False):
    """ Starts the successive halving algorithm with the given settings
    Args:
            algorithm:      str
                name of the algorithm
            dataset:        str
                name of the dataset
            distribution:   dict
                distribution of the parameters given in this form:
                {
                    pname1 : (min, max, step),
                    pname2 : (min, max, step),
                    ...
                }
            resource_name: str
                name of the resource parameter
            resource_max:   int/float
                maximum value the resource can attain
            resource_min:   int/float
                minimum value the resource can start with
    """
    # getting random samples
    distribution_keys = distribution.keys()
    sample_lst = []
    for key in distribution_keys:
        min_v, max_v, step = distribution[key]
        try:
            distri_lst = list(np.arange(*distribution[key]))
            sample_lst.append(random.sample(distri_lst, sample_size))
        except ValueError:
            raise ValueError(f"Sample size is not supported. "
                             f"Got {sample_size}, expected {min_v - max_v}.")
    sample_zip = zip(*sample_lst)
    competitors = []
    for v in sample_zip:
        p = dict(zip(distribution_keys,v))
        competitors.append(([], p, []))
        # the first element of the tuples are rmse history
        # the second is the parameters values
        # the third is rmse history after elimination (for checking the correctness of the run)

    # setting up variables
    alg, _ = ts_algorithms.get_algorithm(algorithm)
    n_competitors = sample_size
    eta = np.exp( np.log(resource_max/float(resource_min))/(math.floor(np.log(sample_size)/np.log(2.))-1))
    resource = resource_min
    resource_lst = []

    pbar = tqdm(total=math.floor(math.log(sample_size, 2)))
    while n_competitors > 1:
        resource_lst.append(resource)
        for c in tqdm(competitors[:n_competitors]):
            print(c[1])
            rmse, *_ = alg(dataset=dataset, verbose=True, **c[1], **{resource_name: resource})
            c[0].append(rmse)
        if keep_losers:
            for c in tqdm(competitors[n_competitors:]):
                print(c[1])
                rmse, _ = alg(dataset=dataset, **c[1], **{resource_name: resource})
                c[2].append(rmse)
        competitors[:n_competitors] = sorted(competitors[:n_competitors], key=lambda r: r[0][-1])
        resource = math.floor(resource*eta)
        n_competitors = math.floor(n_competitors/2)
        pbar.update(1)
    pbar.close()

    print(competitors)

    sh_plot(competitors, algorithm=algorithm, dataset=dataset,
            resources=resource_lst, resource_name=resource_name)
    return competitors[0][1]


def sh_plot(competitors, algorithm=None, dataset=None, resources=[], resource_name='resource'):
    n_rounds = len(competitors[0][0])
    size = len(competitors)
    x_tick_labels = [f"round: {i}\n{resource_name}: {x}" for i, x in enumerate(resources, start=1)]
    plt.figure(figsize=(7, 8))

    ax = plt.gca()
    colormap = plt.cm.get_cmap("Spectral")
    colors = [colormap(i) for i in np.linspace(0, 1, size)]
    ax.set_prop_cycle('color', colors)
    ax.set_facecolor('#DCDCDC')

    for c in competitors:
        c_label = '\n'.join([f"{key}={value:.2f}" for key,value in c[1].items()]).rstrip()
        y = c[0]
        x = range(1, len(y)+1)

        color=next(ax._get_lines.prop_cycler)['color']
        plt.plot(x, y, 'o-', label=c_label, color=color)

        if len(c[2])>0:
            y2 = [c[0][-1]] + c[2]
            x2 = range(len(y), len(y) + len(y2))
            plt.plot(x2, y2, 'o--', color=color)

    plt.legend(prop={'size': 9})
    plt.xticks(range(1, n_rounds+1), labels=x_tick_labels, rotation=30)
    plt.ylabel("rmse")
    plt.ylim(0,1)
    plt.title(f"Successive Halving\nalgorithm: {algorithm}\nsample_size: {size}")

    plt.savefig(f"Graphs/sh/sh_{algorithm}_{dataset}_smpl{size}")
    print(f"Figure saved as sh_{algorithm}_{dataset}_smpl{size}.png")


if __name__ == "__main__":
    sh_execute(algorithm='cd', dataset='airq', sample_size=9,
               distribution={'truncation': (1,10,1)},
               resource_name='max_iter',
               resource_max=100, resource_min=5, keep_losers=False)