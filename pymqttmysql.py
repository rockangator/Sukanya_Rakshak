# -*- coding: utf-8 -*-
"""
Created on Tue Nov 13 17:54:22 2018

@author: ambuj
"""

"""
Python MQTT Subscription client
Thomas Varnish (https://github.com/tvarnish), (https://www.instructables.com/member/Tango172)
Written for my Instructable - "How to use MQTT with the Raspberry Pi and ESP8266"
"""
import paho.mqtt.client as mqtt
import PyMySQL
import datetime

# Open database connection
db = PyMySQL.connect("localhost","root","admin","dummy" )

# prepare a cursor object using cursor() method
cursor = db.cursor()

# Don't forget to change the variables for the MQTT broker!
mqtt_username = "pi"
mqtt_password = "raspberry"
mqtt_topic = "test1"
mqtt_broker_ip = "192.168.43.238"

client = mqtt.Client()
# Set the username and password for the MQTT client
client.username_pw_set(mqtt_username, mqtt_password)

# These functions handle what happens when the MQTT client connects
# to the broker, and what happens then the topic receives a message
def on_connect(client, userdata, flags,rc):
    # rc is the error code returned when connecting to the broker
    print ("Connected!", str(rc))
    
    # Once the client has connected to the broker, subscribe to the topic
    client.subscribe(mqtt_topic)
    
def on_message(client, userdata, msg):
    # This function is called everytime the topic is published to.
    # If you want to check each message, and do something depending on
    # the content, the code to do this should be run in this function
    
    print ("Topic: ", msg.topic + "\nMessage: " + str(msg.payload))
    time = datetime.datetime.now().time()
    #print(time)
    date = datetime.datetime.now().date()
    #print(date)
    query1 = "INSERT INTO dummmyLog(TID,iTime,iDate,DID,Distance)VALUES(%s,%s,%s,%s,%s)"
    # execute SQL query using execute() method.
    cursor.execute(query1,('1',time,date,'1',msg.payload))
    
    query2 = "INSERT INTO IntruderLog(TID,TTime,TDate,Location,DID,Distance,Status,Details) VALUES(%s,%s,%s,%s,%s,%s,%s,%s)"
    cursor.execute(query2,('TID',time,date,'1','1',msg.payload,'status','details'))
    # The message itself is stored in the msg variable
    # and details about who sent it are stored in userdata

# Here, we are telling the client which functions are to be run
# on connecting, and on receiving a message
client.on_connect = on_connect
client.on_message = on_message

# Once everything has been set up, we can (finally) connect to the broker
# 1883 is the listener port that the MQTT broker is using
client.connect(mqtt_broker_ip, 1883)

# Once we have told the client to connect, let the client object run itself
client.loop_forever()
client.disconnect()

# disconnect from server
db.close()