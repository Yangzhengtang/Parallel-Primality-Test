#   Script to dump csv to json string, which can be used to curve in antv-g2 js.
import csv
import json
import pandas as pd
import math

csvfilePath = './all_data.csv'
# jsonfilePath = 'Downloads/all_data.json'
#fieldnames = ("Start","End","Round","Seq", "Par-2", "Par-4", "Par-8", "Par-16")

df = pd.read_csv(csvfilePath)

df['Start'] = pd.to_numeric(df['Start'])
df['Start'] = df['Start'].apply(lambda x: round(math.log(x, 2)))

df = df.loc[df['Start'] >= 32]

df = df.loc[df['Round'] == 32768]

impl_list = ["Seq", "Par-2", "Par-4", "Par-8", "Par-16"]
outputStr = ""

for impl in impl_list:
    tempDf = df.filter(items=['Start', impl])
    tempDf = tempDf.rename(columns={impl: "Time"})

    # tempDf['Time'] = tempDf['Time'].apply(lambda x: (x*100))

    out = tempDf.to_dict(orient='records')
    for item in out:
        item['Impl'] = impl

    json_str = str(out)
    json_str = json_str.replace("'Start'", 'Start')
    json_str = json_str.replace("'Time'", 'Time')
    json_str = json_str.replace("'Impl'", 'Impl')
    json_str = json_str.replace("[", "")
    json_str = json_str.replace("]", "")
    outputStr = json_str + "," + outputStr

print(outputStr)
