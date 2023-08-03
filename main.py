import csv
import serial
import time
from datetime import datetime

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=1.10)
module_settings_dict = {'asd|0000': 'setting|temp|humidity|water|pressure'}


def print_hi(name):
    print(f'Hi, {name}')


def talkToArduino(message):
    arduino.write(bytes(message, 'utf-8'))
    time.sleep(.5)
    data = arduino.readline()
    data = data.replace(b'\r\n', b'')
    return data


def breakDownCSV():
    with (open('New_folder/gHouseTesterCSV.csv', newline='') as csvfile):
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            splitrow1 = row[1].split("|")
            if splitrow1[0] == "setting":
                module_dynamics = {
                    'temp': splitrow1[2],
                    'humidity': splitrow1[3]
                }
                module_settings_dict[row[0] + "|" + splitrow1[1]] = module_dynamics


        for x in module_settings_dict.keys():
            spez = str(module_settings_dict[x])


            spez = spez.replace("{", "")
            spez = spez.replace("}", "")
            spez = spez.replace(" ", "")
            b = spez.split(",")

            time = datetime.now()
            foo = int(time.strftime("%H%M"))
            if foo < int(x.split("|")[1]):
                print(str(x))
                print(b[0])
                print(b[1])
            # ID = talkToArduino(row[0]).decode()
            # function = talkToArduino(row[1]).decode()

            # print('ID = ' + ID)
            # print('function =' + function)


if __name__ == '__main__':
    print_hi('PyCharm')
    breakDownCSV()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
