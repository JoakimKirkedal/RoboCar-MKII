# The User Interface Class definitions
import paho.mqtt.client as mqtt
from os import system
import keyboard


class Sensor:
    def __init__(self):
        self.__topic_sgp40 = "eaaa/itek/e21a/car2/air"
        self.__topic_dust = "eaaa/itek/e21a/car2/dust"
        self.__topic_smoke = "eaaa/itek/e21a/car2/smoke"
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        # self.client.connect("10.120.0.212", 1883, 60)
        self.client.connect("10.120.0.230", 1883, 60)
        self.__sgp_data = "0"
        self.__smoke_data = "0"
        self.__dust_data = "0"
        self.client.loop_start()

    def get_SGP(self):
        return self.__sgp_data

    def get_Dust(self):
        return self.__dust_data

    def get_smoke(self):
        return self.__smoke_data

    def on_connect(self, client, userdata, flags, rc):
        print("connected  with code" + str(rc))
        client.subscribe(self.__topic_sgp40)
        client.subscribe(self.__topic_dust)
        client.subscribe(self.__topic_smoke)

    def on_message(self, client, userdata, msg):
        # print(msg.topic + "  " + str(msg.payload))
        if (msg.topic == self.__topic_sgp40):
            self.__sgp_data = msg.payload.decode()
        if (msg.topic == self.__topic_dust):
            self.__dust_data = msg.payload.decode()
        if (msg.topic == self.__topic_smoke):
            self.__smoke_data = msg.payload.decode()


class User_Interface:

    def __init__(self, RC, IF_Type):
        self.__RC = RC #RoboCar Object Reference
        self.__IF_Type = IF_Type

    def Clear_Screen(self):
        print('Clear User_Interface Screen')

    def Print_Menu(self):
        print('User_Interface Menu')

    def Read_Key(self):
        print('Read a key')

class Terminal_Interface(User_Interface):

    def __init__(self,RC):
        User_Interface.__init__(self,RC,'Terminal')
        self.__RC = RC  # RoboCar Object Reference

        self.Print_Menu()
   
    def Clear_Screen(self):
        system('cls')

    def Print_Menu(self):
        self.Clear_Screen()
        print('----------------------', self.__RC.Get_Name(), '---------------------')
        print('                                                                   ')
        print('       Use the WASD keys to navigate the RoboCar              ')
        print('                           ^                                  ')
        print('                           |                                  ')
        print('                       <------->                              ')
        print('                           |                                  ')
        print('                           v                                  ')
        print('                    Press q to quit                           ')
        print('                                                              ')
        print('----------------------', self.__RC.Get_Interface_Type(), '------------------------')
        print('VOC index: ', self.__RC.get_Sensor_data_sgp())
        print('Dust Particles is: ', self.__RC.get_Sensor_data_dust(), "ug/m3")
        print('Smoke Particles is: ', self.__RC.get_Sensor_data_smoke(), "ug/m3")

    def Read_Key(self):

        if keyboard.is_pressed("w"):
            return 'Arrow_Up'
        elif keyboard.is_pressed("s"):
            return 'Arrow_Down'
        elif keyboard.is_pressed("d"):
            return 'Arrow_Right'
        elif keyboard.is_pressed("a"):
            return 'Arrow_Left'
        elif keyboard.is_pressed("q"):
            return 'q'
        else:
            return "stop"


class Web_Interface(User_Interface):
    def __init__(self,RC):
        User_Interface.__init__(self,RC,'Web')
        self.__RC = RC  # RoboCar Object
        self.Print_Menu()

    def Clear_Screen(self):
        print('Clear Screen on Web Interface')

    def Print_Menu(self):
        self.Clear_Screen()
        print(self.__RC.Get_Name())
        print('Print Web Interface Menu')
        print(self.__RC.Get_Interface_Type())

    def Read_Key(self):
        key = 'A'
        return key
