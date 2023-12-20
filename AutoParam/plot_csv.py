import pandas as pd
import matplotlib.pyplot as plt
import json

def plot_filtered_data(csv_path, algorithm=None, autoparam=None, autoparam_settings=None, seed="-"):
    df = pd.read_csv(csv_path)

    df = df[df['Seed'] == seed]

    if algorithm:
        df = df[df['Algorithm'] == algorithm]

    if autoparam:
        df = df[df['Autoparam'] == autoparam]

    if autoparam_settings:
        condition = json.dumps(autoparam_settings)
        df = df[df['Autoparam_Settings'] == condition]
    else:
        df['Autoparam_Settings_Dict'] = df['Autoparam_Settings'].apply(parse_json)
        df['sample_size'] = df['Autoparam_Settings_Dict'].apply(lambda x : x.get('sample_size'))
        print(df)

    plt.figure(figsize=(8, 4)) 

    df = df.sort_values(by='sample_size')

    ax1 = plt.gca()
    plt.xticks([5, 25, 50, 100, 150, 200, 250])
    ax1.scatter(df['sample_size'], df['Average_RMSE'], color='blue')
    ax1.plot(df['sample_size'], df['Average_RMSE'], color='blue')

    ax1.set_xlabel('Sample Size')
    ax1.set_ylabel('Average RMSE', color='blue')
    ax1.tick_params(axis='y', labelcolor='blue')
    ax1.grid(True)

    ax2 = ax1.twinx() 
    ax2.bar(df['sample_size'], df['Total_Runtime'], color='red', alpha=0.5, width=2)
    ax2.set_ylabel('Total Runtime (s)', color='red')
    ax2.set_yscale('log')
    ax2.tick_params(axis='y', labelcolor='red')

    plt.title('Average RMSE vs Sample Size')
    plt.tight_layout()
    plt.savefig(f"_data/{algorithm}_{autoparam}.png")

def parse_json(json_str):
    try:
        return json.loads(json_str.replace("'", "\""))
    except json.JSONDecodeError:
        return {}
    
if __name__ == '__main__':
    plot_filtered_data("_data/data_autoparam.csv", "cdrec", "rsearch", seed="1")