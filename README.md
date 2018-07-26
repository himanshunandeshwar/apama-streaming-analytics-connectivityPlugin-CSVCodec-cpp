# apama-streaming-analytics-connectivityPlugin-CSVCodec-cpp

Cpp based Connectivity Codec for converting to/from CSV for use with Apama.

## Description
Converts the given event map data to the CSV format (and vice versa). For more information on the Apama Connectivity Framework, as well as Apama in general, please see the community website. 

## Set-up
First, ensure you have an install of the Apama engine; a free edition is available at the community website. This plugin assumes the user has familiarity with the basic structure of the install, more information of which can also be found on the community site.

#### Configuration Steps -
1) Open Apama command prompt.
   - Ensure that the Visual Studio environment is set correctly for the 
   Apama architecture you are building against; this typically means running:
   "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
   
   - Change Directory where make file is present. (\connectivity-plugin-csvcodec-sample\CSVCodec)
   
   - Execute ```make clean```
   
   - Execute ```make release```
   
   This should create a CSVCodec.dll in connectivity-plugin-csvcodec-sample\CSVCodec\x64\Release folder.
2) Copy and Paste the generated CSVCodec.dll to C:\SoftwareAG\Apama\bin.
3) Copy and Paste CSVSender folder to Software AG Designer Workspace. Let's call this Workspace as Workspace1.
4) Copy and PAste CSVReceiver folder to SOftware AG Designer Workspace which is different from Workspace1. Let's call this Workspace as Workspace2.
5) Open CSVReciever in Software AG Designer from Workspace1-

   - Open Run Configurations. (Click on Run->Run Configurations)
  
   - Add a new Correlator. (Click on Add->Add Correlator)
  
   - Uncheck all the unecessary correlator. Only the new correlator should be kept checked.
  
   - Click on Run.
  
**Now this First Correlator is ready to receive events.**
  
6) Open CSVSender in Software AG Designer from WOrkspace2.
   - Click on Run->Run to start the Correlator to send events. 
   
**Once this step is done this second Correlator will be sending the events    which can be verified on the console of the previously created correlator.**
   
**Note** - *In this example The Message sent by CSV sender is sent to an online MQTT broker broker.hivemq.com over mqtt:/csvtest channel which is defined in MQTT.properties config file of CSVSender. The CSVReceiver recieves the message from this same broker.hivemq.com MQTT broker over the channel mqtt:/csvtest. To verify that the message is being transmitted from sender to receiver via broker.hivemq.com a tool called **Eclipse paho MQTT Utility** is available from https://www.eclipse.org/paho/.*
