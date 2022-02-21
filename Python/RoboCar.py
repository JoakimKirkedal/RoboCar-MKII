#The RoboCar Class definitions
from User_Interface import User_Interface, Terminal_Interface, Web_Interface
from Motion_Control import Motion_Control
from enum import Enum
from User_Interface import Sensor

class States(Enum):
  STOP     = 0
  LEFT     = 1
  RIGHT    = 2
  FORWARD  = 3
  BACKWARD = 4
  
class RoboCar:

  def __init__(self, Name, IF_Type):
    self.__Name = Name
    self.__IF_Type = IF_Type
    self.__State = States.STOP
    self.my_sensor = Sensor()

    # User Interface Object is created and RoboCar object reference is passed down
    if IF_Type == 'Terminal':
      self.MyFace = Terminal_Interface(self)
    elif IF_Type == 'Web':
      self.MyFace = Web_Interface(self)

    # Motion Control Object is created 

    self.MyMotion = Motion_Control()

    #Start the Main RoboCar Loop
    self.Main_Loop()

  def get_Sensor_data_sgp(self):
    return self.my_sensor.get_SGP()

  def get_Sensor_data_dust(self):
    return self.my_sensor.get_Dust()

  def get_Sensor_data_smoke(self):
    return self.my_sensor.get_smoke()

  def Stop(self):
    self.MyMotion.Stop()

  def GoLeft(self):
    self.MyMotion.Left()

  def GoRight(self):
    self.MyMotion.Right()

  def GoForward(self):
    self.MyMotion.Forward(1)

  def GoBackward(self):
    self.MyMotion.Backward(0)

  def Get_Interface_Type(self):
    return self.__IF_Type

  def Set_Name(self, Name):
    self.__Name = Name

  def Get_Name(self):
    return self.__Name

  def Set_State(self, State):
    self.__State = State

  def Get_State(self):
    return self.__State

  def Main_Loop(self):

    quit = False
    
    actions = {States.STOP:     self.Stop,
               States.LEFT:     self.GoLeft,
               States.RIGHT:    self.GoRight,
               States.FORWARD:  self.GoForward,
               States.BACKWARD: self.GoBackward}

    while not quit:
      self.MyFace.Print_Menu()
      key = self.MyFace.Read_Key()
      if key == 'q':
        quit = True
  
      if key == 'Arrow_Up':
        self.Set_State(States.FORWARD)
      elif key == 'Arrow_Down':
        self.Set_State(States.BACKWARD)
      elif key == 'Arrow_Left':
        self.Set_State(States.LEFT)
      elif key == 'Arrow_Right':
        self.Set_State(States.RIGHT)

      elif key == "stop":
        self.Set_State(States.STOP)


      action = actions.get(self.__State)
      action()
