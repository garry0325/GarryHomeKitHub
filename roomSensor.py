import bluetooth
import time

refreshFrequency=60

sensorAddress="98:D3:31:40:52:94"
sensorPort=1

fileLocation="/var/www/html/roomSensor/roomSensor.json"
batteryFileLocation="/var/www/html/roomSensor/battery.txt"

logLocation="/home/pi/Desktop/HomeKit Log/log.txt"

temperature=1
humidity=2
lux=3
battery=1


def receiveSensorData(MACAddress,port):

    
    while True:
        try:
            socketBluetooth=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            time.sleep(0.1)
            print "Connecting "+MACAddress+"..."
            socketBluetooth.connect((MACAddress,port))
        except:
            print "Target not detected, reconnecting..."
            time.sleep(10)
            continue
            
        break
    print "Connected."

    batteryFile=open(batteryFileLocation,"w")
    batteryFile.write("0")
    batteryFile.close()
    
    lastTimeWasFailed=True
    lastTimeWasSucceed=False
    while True:
        socketBluetooth.settimeout(20.0)
        try:
            data=socketBluetooth.recv(1024)
            print data
            if data[0]!=',' and len(data)<=20 and len(data)>=10 and data.count(',')==3:
                temperature=data[:data.find(',')]
                data=data[data.find(',')+1:]
                humidity=data[:data.find(',')]
                data=data[data.find(',')+1:]
                lux=data[:data.find(',')]
                air=int(data[data.find(',')+1:])
                
                air=airqualityIndex(air)
                if validate(temperature,humidity,lux):
                    if not lastTimeWasFailed:
                        file=open(fileLocation,"w")
                        file.write("{\n\t\"temperature\": "+str(temperature)+",\n"+
                                   "\t\"humidity\": "+str(humidity)+",\n"+
                                   "\t\"lux\": "+str(lux)+",\n"+
                                   "\t\"air\": "+str(air)+"\n"+
                                   "}")
                        file.close()

                        #fileBattery=open(batteryFileLocation,"w")
                        #fileBattery.write(str(battery))
                        #fileBattery.close()
                        
                        if lastTimeWasSucceed:
                            ErrorOutput("Error Fixed") 
                            lastTimeWasSucceed=False
                        #print "file"
                    else:
                        lastTimeWasFailed=False
                        lastTimeWasSucceed=True
                else:
                    ErrorOutput("Validation Failed")
                    lastTimeWasFailed=True
                    
            else:
                ErrorOutput("Data Not Clean")
                #time.sleep(1)
                lastTimeWasFailed=True
            #print "-------"
            time.sleep(refreshFrequency)
        except bluetooth.btcommon.BluetoothError:
            print "Bluetooth Error"
            fileBattery=open(batteryFileLocation,"w")
            fileBattery.write("1 "+ time.strftime("%b %d %H:%M ",time.localtime()))
            fileBattery.close()
            socketBluetooth.close()
            break
        except:
            print "Recving timed out"
            break
            
    
    

def airqualityIndex(raw):
    if raw>=200:
        return 4
    elif raw>=100:
        return 3
    elif raw>=50:
        return 2
    elif raw>=10:
        return 1
    else:
        return 0

def validate(temperature,humidity,lux):
    try:
        temperature=float(temperature)
        humidity=float(humidity)
        lux=int(lux)
        return True
    except SyntaxError:
        ErrorOutput("Syntax Error")
        return False
    except ValueError:
        ErrorOutput("Value Error")
        return False

def ErrorOutput(errorMessage):
    print time.strftime("%b %d %H:%M ",time.localtime())+errorMessage


while True:
    receiveSensorData(sensorAddress,sensorPort)
    time.sleep(10)
