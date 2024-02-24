import numpy as np
import random
import matplotlib.pyplot as plt
import matplotlib as mpl
import ts_algorithms

class Particle:
    w = 0.8
    c1 = 0.1
    c2 = 0.1
    gbest = []
    gbest_rmse = 3

    def __init__(self, X, X_min, X_max, foo, keys, dataset, fixed_param):
        self.X = np.array(X)
        self.X_min = X_min
        self.X_max = X_max
        self.n = len(self.X)
        self.foo = foo
        self.keys = keys
        self.V = np.array([random.uniform(0.1, 0.8) for _ in range(len(X))])
        self.dataset = dataset
        self.pbest = X
        self.fixed_param = fixed_param
        self.pbest_rmse = self.execute()

    def execute(self):
        param_dict = {}

        for i in range(len(self.X)):
            param_dict[self.keys[i]] = self.X[i]
        rmse, *_ = self.foo(**param_dict, **self.fixed_param, dataset=self.dataset)
        return rmse

    def update(self):
        r1, r2 = np.random.rand(2)
        self.V = Particle.w * self.V + \
                 Particle.c1*r1*(self.pbest - self.X) + \
                 Particle.c2*r2*(Particle.gbest-self.X)
        self.X = self.X + self.V
        print("X : " + str(self.X) + "     V : " + str(self.V))
        self._check_bounds()
        rmse = self.execute()

        if rmse < self.pbest_rmse:
            self.pbest_rmse = rmse
            self.pbest = self.X

        if rmse < Particle.gbest_rmse:
            Particle.gbest_rmse = rmse
            Particle.gbest = self.X

    def _check_bounds(self):
        for i in range(len(self.X)):
            if self.X[i] < self.X_min[i]:
                self.X[i] = self.X_min[i]
            if self.X[i] > self.X_max[i]:
                self.X[i] = self.X_max[i]


def ps_execute(algorithm=None, dataset=None,
               sample_size=5,
               n_iter=10,
               distribution={},
               fixed_param={}):
    """ Starts the random search algorithm with the given settings
    Args:
            algorithm:      str
                name of the algorithm
            dataset:        str
                name of the dataset
            sample_size:    int
                number of particles
            n_iter:         int
                number of iteration
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
    X_min = []
    X_max = []
    pbest = 3

    for key in distribution_keys:
        min_v, max_v, step = distribution[key]
        X_min.append(min_v)
        X_max.append(max_v)
        distri_lst = list(np.arange(*distribution[key]))
        sample_lst.append(random.choices(distri_lst, k=sample_size))
    
    sample_zip = zip(*sample_lst)

    alg, *_ = ts_algorithms.get_algorithm(algorithm)
    particles = []
    for s in sample_zip:
        p = Particle(s, X_min, X_max, alg, list(distribution.keys()), dataset, fixed_param)
        particles.append(p)

    # Initialize data
    Particle.gbest = particles[0].pbest
    Particle.gbest_rmse = particles[0].pbest_rmse
    for p in particles:
        if p.pbest_rmse < Particle.gbest_rmse:
            Particle.gbest_rmse = p.pbest_rmse
            Particle.gbest = p.pbest

    # Run iterations
    for _ in range(n_iter):
        for p in particles:
            p.update()
        print("\n")

    best_keys = list(distribution.keys())
    best_values = Particle.gbest
    best_dict = dict(zip(best_keys, best_values))

    sep = "  |  "
    print("Best Results")
    print(sep.join(best_keys))
    print(sep.join([str(x) for x in Particle.gbest]))
    print("rmse : " + str(Particle.gbest_rmse))

    return best_dict, Particle.gbest_rmse

def ps_plot_ranges():
    alg = "spirit"
    dataset = "drift10"
    n_particles = list(range(1, 7))
    n_iter = list(range(5, 35, 5))
    results = np.zeros((len(n_particles), len(n_iter)))
    for i in range(len(n_particles)):
        for j in range(len(n_iter)):
            _, rmse = ps_execute(algorithm=alg, dataset=dataset, distribution={"lbda": (0.01, 1, 0.01),
                                                                      "truncation": (1, 10, 1),
                                                                      "win_size": (2, 10, 1)},
                       sample_size=n_particles[i],
                       n_iter=n_iter[j])
            results[i][j] = rmse

    fig, ax = plt.subplots()
    im = ax.imshow(results)

    # Show all ticks and label them with the respective list entries
    ax.set_xticks(np.arange(len(n_particles)), labels=n_particles)
    ax.set_yticks(np.arange(len(n_iter)), labels=n_iter)
    ax.set_xlabel("n_particules")
    ax.set_ylabel("n_iter")

    # Loop over data dimensions and create text annotations.
    for i in range(len(n_particles)):
        for j in range(len(n_iter)):
            text = ax.text(j, i, "{:.4f}".format(results[i][j]),
                           ha="center", va="center", color="w")

    ax.set_title(f"Best rmse found with respective parameters in swarm particle\nAlg: {alg}\nDataset: {dataset}")
    fig.tight_layout()
    fig.savefig(f"Graphs/swarmp/compare_ranges_{alg}_{dataset}")


if __name__ == "__main__":
    ps_plot_ranges()
    # ps_execute(algorithm="stmvl", dataset="airq", distribution={"alpha": (0.01, 1, 0.01),
    #                                                          "gamma": (0.01, 1, 0.01),
    #                                                             "win_size": (2, 10, 1)},
    #            sample_size=5,
    #            n_iter=5)