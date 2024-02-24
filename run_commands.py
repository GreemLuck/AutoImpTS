import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import ast
import seaborn as sns
from tqdm import tqdm
import json

exploration_values = [10, 5, 15, 1, 19]
exploitation_values = [10, 15, 5, 19, 1]
sample_size = [5, 10, 20, 50]
nparticles = [12, 24]
niter = [5, 10, 15, 20]
repetitions = 10

# command = "python3 AutoParam/start_imputation_benchmarks.py --technique bayes --algorithm cdrec --dataset airq --scenario MCAR"
# command = "python3 AutoParam/start_imputation_benchmarks.py --technique rsearch --algorithm cdrec --dataset airq --scenario MCAR"
command = "python3 AutoParam/start_imputation_benchmarks.py --technique swarm_particle --algorithm cdrec --dataset airq --scenario MCAR"


# total_iterations = sum(sample_size) * repetitions
# with tqdm(total=total_iterations, desc="Running Algorithms", unit="iteration") as pbar:
#     for exploration, exploitation in zip(exploration_values, exploitation_values):
#         for _ in range(repetitions):
#             full_command = f"{command} --exploration={exploration} --exploitation={exploitation}"
#             subprocess.run(full_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
#             pbar.update(1)

# total_iterations = len(nparticles) * len(niter) * repetitions
# with tqdm(total=total_iterations, desc="Running Algorithms", unit="iteration") as pbar:
#     for s in nparticles:
#         for i in niter:
#             for _ in range(repetitions):
#                 full_command = f"{command} --niter=10 --nparticles={s} --niter={i}"
#                 subprocess.run(full_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
#                 pbar.update(1)

# quit()


# Add a label line for each autoparam settings value

df = pd.read_csv('_data/data_autoparam.csv')
# First filter out all the rows that are not from the rsearch technique
df_filtered = df.loc[df['Autoparam'] == 'succ_halving']
# df_filtered = df_filtered.drop_duplicates(subset='Autoparam_Settings', keep='first')

def parse_column(row):
    for value in row:
        json_str = value.replace("'", '"')
        data = json.loads(json_str)
        for k, v in data.items():
            print(k, v)

parse_column(df_filtered['Autoparam_Settings'])

# replace all ',' by '\n' and remove the '{' and '}'
def parse_settings(settings):
    settings = settings.replace('{', '(')
    settings = settings.replace('}', ')')
    settings = settings.replace("'exploration': ", '')
    settings = settings.replace("'exploitation': ", '')
    return settings

# only take the values of the json object properties and if there is more than one property, return a tuple
def get_values(json_str):
    # replacing all ' by " to make it a valid json string
    json_str = json_str.replace("'", '"')
    data = json.loads(json_str)
    if len(data) > 1:
        return str(tuple(data.values()))
    else:
        return tuple(data.values())[0]
    
def extract_sample_size(json_str):
    try:
        json_obj = json.loads(json_str.replace("'", '"'))
        print(json_obj)
        return json_obj['sample_size']
    except json.JSONDecodeError:
        return None


df_filtered['Sample_Size'] = df_filtered['Autoparam_Settings'].apply(extract_sample_size)
    
print(df_filtered['Average_RMSE'])

fig = sns.lineplot(
    data=df_filtered, 
    x='Sample_Size', 
    y='Average_RMSE', 
    legend=False, 
    style='Autoparam', 
    markers=True, 
    dashes=False
    )
fig.set(xlabel='Sample Size', ylabel='Average RMSE')
ax2 = fig.twinx()
fig2 = sns.lineplot(
    data=df_filtered, 
    x='Sample_Size', 
    y='Total_Runtime', 
    ax=ax2, color='red', 
    legend=False, 
    style='Autoparam', 
    markers=True, 
    dashes=False
    )
fig2.set(ylabel='Total Runtime (s)')
fig2.set_yscale('log')
# make the boxplot median line bigger
# for line in fig.get_lines():
#     line.set_linewidth(3)


# plt.xticks(df_filtered['Autoparam_Settings'], ha='right')
# plt.xlabel('Autoparam_Settings')
# plt.ylabel('Average_RMSE')
# plt.title('Scatter plot of Average_RMSE vs Autoparam_Settings')
# plt.tight_layout()
plt.savefig("_data/cdrec_rsearch.png")