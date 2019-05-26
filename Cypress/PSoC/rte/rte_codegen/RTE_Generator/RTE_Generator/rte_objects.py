##############################################################################
# rte_objects.py
#
# Description:   Create RTE signal objects
# Author:        Peter Fromm
#
# Input:         XML stream
# Output:        signal objects
from fileinput import filename


    ########################################################################
    ## Public method for the h file creation
    ########################################################################  
    
    
    
    
    
def _create_h_file_header(root, fout_h):
    
    rline = "/**************************************************\n"
    fout_h.write(rline)
    
    rline = " ** The RTE signal blackboard (autogenerated)\n"
    fout_h.write(rline)
  
    rline = " **************************************************/\n\n"
    fout_h.write(rline)
    
    filename = "RTE_SIGNALPOOL"
    rline = "#ifndef " + filename + "_H\n"
    fout_h.write(rline)
    
    rline = "#define " + filename + "_H\n\n"
    fout_h.write(rline)
    
    rline = "\n/* Generated signal classes */\n"
    fout_h.write(rline)

    for signalclass in root.iter("signalclass"):
        rline = "#include \"" + signalclass.get("name").lower() + "_signal.h\"\n"
        fout_h.write(rline)

    fout_h.write("\n\n")

    ########################################################################
    ## Public method for the h file creation
    ########################################################################  
    
    
def _create_signalpool_h_file(root, config):
    
    #open filename
    filename_h = config["path_out"] + "\\" + "rte_signalpool.h"
    fout_h=open(filename_h, 'w')

    #Create file header with includes
    _create_h_file_header(root, fout_h)
    
    #create extern objects
    rline = "/* Generated objects */\n\n"
    fout_h.write(rline)
    
    print("   Writing Header File...")
    #Iterate through all signal objects
    for signalobject in root.iter('signalobject'):
        rline = "/* " + str(signalobject.get("description")) + " */\n"
        fout_h.write(rline)
        signaltype = str(signalobject.get("signalclass")).upper()
        signalname = str(signalobject.get("name"))
        print ("      Object: ", signalname)
        rline = "extern " + signaltype + "_t " + signalname.upper() + "_signal;\n\n"
        fout_h.write(rline)


    #create init functions per signallayer
#    self.create_h_init()

    #create timertick functions per signallayer
    rline = "/** Central Timertick function \n"
    fout_h.write(rline)
    rline = " *  must be called every " + config["timertickperiod"] + "ms \n"
    fout_h.write(rline)
    rline = " **/  \n"
    fout_h.write(rline)
    rline = "void RTE_timertick();\n"
    fout_h.write(rline)


    rline = "\n\n#endif\n"
    fout_h.write(rline)

    fout_h.close()


########################################################################
## Private methods for the c file creation
########################################################################        
def _create_c_file_header(root, config, fout_c):

    rline = "/**************************************************\n"
    fout_c.write(rline)
    
    rline = " ** The RTE signals (autogenerated)\n"
    fout_c.write(rline)
  
    rline = " **************************************************/\n\n"
    fout_c.write(rline)
    
    rline = "#include \"project.h\" \n"
    fout_c.write(rline)

    rline = "#include \"global.h\" \n"
    fout_c.write(rline)

    rline = "#include \"rte_signalpool.h\" \n\n"
    fout_c.write(rline)    
    
    rline = "\n/* Generated signal classes */\n"
    fout_c.write(rline)

    for signalclass in root.iter("signalclass"):
        rline = "#include \"" + signalclass.get("name").lower() + "_signal.h\"\n"
        fout_c.write(rline)

    fout_c.write("\n\n")

    
    
def _create_c_one_object(rte_object, fout_c):
    
        objectname = rte_object.get("name")
        signalname = rte_object.get("signalclass") 
        indriver = rte_object.get("indriver")
        outdriver = rte_object.get("outdriver")
        evOnUpdate = rte_object.get("onDataUpdate")
        
        if (indriver == "default"):
            indriver = signalname.upper() + "_driverIn"

        if (outdriver == "default"):
            outdriver = signalname.upper() + "_driverOut"

        if (evOnUpdate == "default"):
            evOnUpdate = "ev_" + objectname + "_onUpdate"
            
        ## Create signal configuration
        rline = "\n\n/* signal configuration */\n\n"
        fout_c.write(rline)
    
        rline = "static const " + signalname.upper() + "_cfg_t " + objectname.upper() + "_cfg = \n{\n"
        fout_c.write(rline)
    
        rline = "   /* indriver      */  " + indriver + ",\n"
        fout_c.write(rline)
    
        rline = "   /* outdriver     */  " + outdriver + ",\n"
        fout_c.write(rline)
    
        rline = "   /* onUpdate      */  " + evOnUpdate + "\n"
        fout_c.write(rline)
    
        rline = "}; /* configuration " + objectname + " */\n"
        fout_c.write(rline)
    
   
        ## Create signal data 
        rline = "\n\n/* signal data */\n\n"
        fout_c.write(rline)
        
        rline = signalname.upper() + "_t " + objectname.upper() + "_signal = \n{\n"
        fout_c.write(rline)
        
        rline = "   /* init value    */  " + signalname.upper() + "_INIT_DATA,\n"
        fout_c.write(rline)
        
        rline = "   /* status        */  RTE_SIGNALSTATUS_STARTUP,\n"
        fout_c.write(rline)
        
        rline = "   /* age           */  0,\n"
        fout_c.write(rline)
        
        
        rline = "   /* cfg           */  &" + objectname.upper() + "_cfg\n"
        fout_c.write(rline)
        
        rline = "}; /* data " + objectname + " */\n"
        fout_c.write(rline)
  
  
def _create_c_timertick(root, config, fout_c):

    rline = "\n\n/*******************************************************************************\n"
    fout_c.write(rline)
    rline = " ** Central timertick function for ticktime " + config["timertickperiod"] + " \n"
    fout_c.write(rline)
    rline = " *******************************************************************************/ \n\n"
    fout_c.write(rline)


    rline = "void RTE_timertick()\n"
    fout_c.write(rline)
    rline = "{\n";
    fout_c.write(rline)
    
    #Create the list of existing templates
    for rte_object in root.iter('signalobject'):
        rline = "    RTE_" + rte_object.get("signalclass").upper() + "_incAge( &" + rte_object.get("name").upper() + "_signal, " + config["timertickperiod"] + ");\n"
        fout_c.write(rline)

    rline = "}\n\n";
    fout_c.write(rline)
  
  
def _create_signalpool_c_file(root, config):
    
    print("   Writing Source File...")
    
    #open filename
    filename_c = config["path_out"] + "\\" + "rte_signalpool.c"
    fout_c=open(filename_c, 'w')
    
    
    #Create file header with includes
    _create_c_file_header(root, config, fout_c)
    
    
    
    #Create the list of existing templates
    for signalobject in root.iter('signalobject'):
        rline = "\n\n/*****************************************************\n"
        fout_c.write(rline)
        rline = " ** RTE Object " + signalobject.get("name") + "\n"
        fout_c.write(rline)
        rline = " ** " + signalobject.get("description") + "\n"
        fout_c.write(rline)
        rline = " *****************************************************/\n"
        fout_c.write(rline)
        
        #object creation
        print("      Object", signalobject.get("name"))
        _create_c_one_object(signalobject, fout_c)
            
    
    #create timertick functions per signallayer
    _create_c_timertick(root, config, fout_c)
    
    

    fout_c.close()
    
################################
## create
## In: root of the xml data structure
def create(root, config):
    
      
    ##create the files        
    _create_signalpool_h_file(root, config)
#    create_c_file()
    _create_signalpool_c_file(root, config)
    
  
    

## create
################################   