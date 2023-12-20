import numpy as np
import random
import math
import ts_algorithms
from tqdm import tqdm

def rs_execute(algorithm=None, dataset=None,
               sample_size=5,
               distribution={},
               fixed_param={}):
    """ Starts the random search algorithm with the given settings
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
    """
    # getting random samples
    distribution_keys = distribution.keys()
    sample_lst = []
    for key in distribution_keys:
        min_v, max_v, step = distribution[key]
        distri_lst = list(np.arange(*distribution[key]))
        sample_lst.append(random.choices(distri_lst, k=sample_size))

    sample_zip = zip(*sample_lst)
    competitors = []
    for v in sample_zip:
        p = dict(zip(distribution_keys,v))
        competitors.append([p, 0])

    # setting up variables
    alg, _, _ = ts_algorithms.get_algorithm(algorithm)
    n_competitors = sample_size

    for c in tqdm(competitors):
        print(c[0])
        rmse, *_ = alg(dataset=dataset, verbose=True, **c[0], **fixed_param)
        c[1] = rmse

    competitors = sorted(competitors, key=lambda r: r[1])

    print(competitors)

    return competitors[0]

if __name__ == "__main__":
    rs_execute(algorithm="stmvl", dataset="airq", sample_size=5,
               distribution=ts_algorithms.ST_MVL_BOUNDS_STEP, fixed_param={})