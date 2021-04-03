import dateparser
import math


def cleanup(s: str) -> str:
    ret = s
    for c in "[]\x1b":
        ret = ret.replace(c, "")
    return ret


def parse(_a: str):
    try:
        a = cleanup(_a).split(" ")
        ret = {}
        ret['dt'] = dateparser.parse(f'{a[0]} {a[1]}')
        if ret['dt'] is None:
            return None
        for i, p in enumerate(a):
            if p.startswith('sA'):
                k, v = p.split("=")
                ret[k] = float(v)
                if math.isnan(ret[k]):
                    return None
            elif p.startswith('rA'):
                k, v = p.split("=")
                ret[k] = float(v)
                if math.isnan(ret[k]):
                    return None
                ret[k + '_count'] = int(a[i + 1].split(";")[0])
        if len(ret) < 10:
            return None
        return ret
    except Exception as e:
        return None


import pandas as pd

df_file = 'parsed.df'
evict = False
import os.path

if os.path.isfile(df_file) and not evict:
    df = pd.read_csv(df_file, parse_dates=["dt"])
    print(f'Restored {len(df)}')
else:
    with open('usbport.log') as f:
        lines = f.readlines()
    print(len(lines))
    rows = []
    print('start')
    for i, line in enumerate(lines):
        v = parse(line)
        if v is not None:
            rows.append(v)
        if i % 1000 == 0:
            print(f'Progress {i}/{len(lines)} ({round(i / len(lines) * 100)}%)')
    df = pd.DataFrame(rows)
    print(f'Stored {len(df)}')
    df.to_csv(df_file)
df = df.rename(columns={"sA4": "1_датчик", "rA5": "1_уровень", "rA5_count": "1_поливы",
                        "sA3": "2_датчик", "rA7": "2_уровень", "rA7_count": "2_поливы",
                        "sA2": "3_датчик", "rA6": "3_уровень", "rA6_count": "3_поливы"})
df = df.drop('Unnamed: 0', axis=1)
df.set_index('dt', inplace=True)
from matplotlib import pyplot

for i in range(1, 4):
    df[f'{i}_поливы'] = df[f'{i}_поливы'] / max(df[f'{i}_поливы'])
    df[f'{i}_датчик'] = df[f'{i}_датчик'] / 1024
    df[f'{i}_уровень'] = df[f'{i}_уровень'] / 1024
    df[[f'{i}_датчик', f'{i}_уровень', f'{i}_поливы']].plot()
pyplot.show()
