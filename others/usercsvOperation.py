#在原基础中 将密码凯撒加密   加上power deleted

import csv

def kaisa(data):
    s = ''
    for i in data:
        char = chr(ord(i) + 3)
        s += char

    return s




def add():

    data = []
    with open("demo/userInfo.csv", "r", encoding="utf-8") as f:
        reader = csv.reader(f)
        for row in reader:

            row[2] = kaisa(row[2])

            row.append(1)
            row.append(0)
            
            data.append(tuple(row))
    # 写入
    with open("demo/userInfo.csv", "w", encoding="utf-8", newline='') as fw:
        writer = csv.writer(fw)
        writer.writerows(data)


add()