import pandas as pd

serial = pd.read_csv("./serial.csv")
para2 = pd.read_csv("./para-2.csv")
para4 = pd.read_csv("./para-4.csv")
para8 = pd.read_csv("./para-8.csv")
para16 = pd.read_csv("./para-16.csv")
data = serial
data['para2'] = para2['time']
data['para4'] = para4['time']
data['para8'] = para8['time']
data['para16'] = para16['time']

data.to_csv("stats.csv", index=None)




