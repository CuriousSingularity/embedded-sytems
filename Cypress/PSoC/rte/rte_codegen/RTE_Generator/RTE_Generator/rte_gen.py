##############################################################################
# rte_gen.py
#
# Description:   Create RTE Layer files
# Author:        Peter Fromm
#
# Main module of the system


#system modules
import argparse
import os.path
import xml.etree.ElementTree as ET

#own modules
import rte_signalclass
import rte_objects
import rte_activation

VERSION = "Erika 1"

#https://docs.python.org/2/library/xml.etree.elementtree.html

################################
## create_config_list
##
## This function checks the config section of the xml file
## and creates a hashtable for all entries
##
## In: root of xml file
## Out: Hashtable with config names and values
##
def create_config_list(root):
    
    #create empty hash table
    config_list = { }
    
    #iterate through all elements - brute force
    for config in root.iter("config"):
        config_list[config.get("name")] = config.get("value")
    
    return config_list
## end of function 
################################  




################################
## main
################################

#Check the command line options
parser = argparse.ArgumentParser(description = "RTE Generator")
parser.add_argument("xml_file", help="the name of the input xml file", default=".\in\rte.xml")
args = parser.parse_args()





#Welcome
print("RTE Generator " , VERSION)
print("Processing: ", args.xml_file, " based on version ", VERSION)

#Check if xml file exists
if (not os.path.isfile(args.xml_file)):
    print("ERROR: XML file does not exist, check parameter")
    exit(-1)

#parse the input file and get the rott for the upcoming functions to work with
tree = ET.parse(args.xml_file)
root = tree.getroot()



#Create config list, which will be used by most functions
print("Checking the configuration...")
config = create_config_list(root)

if (not config["version"] == VERSION):
    print("ERROR: Version conflict of XML file. Verion in file: ", config["version"], " expected: ", VERSION)
    exit(-1)

#Create the signal class files
print ("Creating the signalclasses...")
for signalclass in root.iter('signalclass'):
    rte_signalclass.create_one_signal_class(signalclass, config)


#Create the object blackboard
print ("Creating the signal object pool...")
rte_objects.create(root, config)


#Create the activation engine
print ("Creating the signal activation engine...")
rte_activation.create(root, config)






