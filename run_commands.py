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
repetitions = 20

# command = "python3 AutoParam/start_imputation_benchmarks.py --technique bayes --algorithm cdrec --dataset airq --scenario MCAR"
command = "python3 AutoParam/start_imputation_benchmarks.py --technique rsearch --algorithm cdrec --dataset airq --scenario MCAR"

# total_iterations = sum(sample_size) * repetitions
# with tqdm(total=total_iterations, desc="Running Algorithms", unit="iteration") as pbar:
#     for exploration, exploitation in zip(exploration_values, exploitation_values):
#         for _ in range(repetitions):
#             full_command = f"{command} --exploration={exploration} --exploitation={exploitation}"
#             subprocess.run(full_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
#             pbar.update(1)

# total_iterations = sum(sample_size) * repetitions
# with tqdm(total=total_iterations, desc="Running Algorithms", unit="iteration") as pbar:
#     for s in sample_size:
#         for _ in range(repetitions):
#             full_command = f"{command} --sample_size={s}"
#             subprocess.run(full_command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
#             pbar.update(1)


# Add a label line for each autoparam settings value

df = pd.read_csv('_data/data_autoparam.csv')
# First filter out all the rows that are not from the rsearch technique
df_filtered = df.loc[df['Autoparam'] == 'rsearch']
# Then take the AutoParam_Settings column and parse it to get the values and create a new column for each value

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



df_filtered['Autoparam_Settings'] = df_filtered['Autoparam_Settings'].apply(get_values)
    

fig = sns.boxplot(data=df_filtered, x='Autoparam_Settings', y='Average_RMSE', hue='Autoparam_Settings', legend=False)
fig.set(xlabel='Autoparam Settings (exploration, exploitation)', ylabel='Average RMSE')
# make the boxplot median line bigger
for line in fig.get_lines():
    line.set_linewidth(3)


# plt.xticks(df_filtered['Autoparam_Settings'], ha='right')
# plt.xlabel('Autoparam_Settings')
# plt.ylabel('Average_RMSE')
# plt.title('Scatter plot of Average_RMSE vs Autoparam_Settings')
# plt.tight_layout()
plt.savefig("_data/rsearch.png")