# The Motion Control Class definitions
from enum import Enum
import paho.mqtt.client as mqtt

from time import sleep

topic_control = "eaaa/itek/e21a/car2"
def on_connect(client, userdata, flags, rc):
  print("connected  with code" + str(rc))
  client.subscribe(topic_control)


def on_message(client, userdata, msg):
  print(msg.topic_control + "  " + str(msg.payload))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
#client.connect("10.120.0.212", 1883, 60)
client.connect("10.120.0.230", 1883, 60)



class DriveStates(Enum):
  STOP     = 0
  FORWARD  = 1
  BACKWARD = 2
  LEFT     = 3
  RIGHT    = 4
  
class Motion_Control:

  def __init__(self):
    self.__Left_DriveUnit = Drive_Unit('LeftDrive')
    self.__Right_DriveUnit = Drive_Unit('RightDrive')

  def Stop(self):
    self.__Left_DriveUnit.Set_Drive_State(DriveStates.STOP, 1) 
    self.__Right_DriveUnit.Set_Drive_State(DriveStates.STOP, 1)
    
  def Forward(self, speed):
    self.__Left_DriveUnit.Set_Drive_State(DriveStates.FORWARD, speed) 
    self.__Right_DriveUnit.Set_Drive_State(DriveStates.FORWARD, speed)

  def Backward(self, speed):
    self.__Left_DriveUnit.Set_Drive_State(DriveStates.BACKWARD, speed) 
    self.__Right_DriveUnit.Set_Drive_State(DriveStates.BACKWARD, speed)
    
  def Left(self):
    self.__Right_DriveUnit.Set_Drive_State(DriveStates.LEFT,1)

  def Right(self):
    self.__Left_DriveUnit.Set_Drive_State(DriveStates.RIGHT, 1)


class Drive_Unit:

  def __init__(self, Name):
    self.__Drive_State = DriveStates.STOP
    self.__Last_Drive_State = DriveStates.STOP
    self.__Speed = 1
    self.__Name = Name
        
  def Set_Drive_State(self, Drive_State, Speed):
  
    DriveActions = {DriveStates.STOP     : self.__Drive_Stop,
                    DriveStates.FORWARD  : self.__Motor_forward,
                    DriveStates.BACKWARD : self.__Motor_backward,
                    DriveStates.LEFT     : self.__Motor_left,
                    DriveStates.RIGHT    : self.__Motor_right}

    self.__Drive_State = Drive_State
    self.__Speed = Speed
    
    if Drive_State != self.__Last_Drive_State: #To avoid multiple calls to the motors
      action = DriveActions.get(Drive_State)
      action(Speed)
      self.__Last_Drive_State = Drive_State
    
  def __Drive_Stop(self, speed):
    self.__speed = speed
    #print('Motor Stop')
    client.publish(topic_control, 'S')

  def __Motor_forward(self, speed):
      #print('Motor Forward')
      self.__speed = speed
      client.publish(topic_control, 'F')

  def __Motor_backward(self, speed):
    #print('Motor Backward')
    self.__speed = speed
    client.publish(topic_control, 'B')

  def __Motor_left(self, speed):
      #print('Motor Left')
      self.__speed = speed
      client.publish(topic_control, 'L')

  def __Motor_right(self, speed):
      #print('Motor Right')
      self.__speed = speed
      client.publish(topic_control, 'R')

  def Get_Drive_State(self):
    return self.__Drive_State

  def Get_Drive_Speed(self):
    return self.__Speed
