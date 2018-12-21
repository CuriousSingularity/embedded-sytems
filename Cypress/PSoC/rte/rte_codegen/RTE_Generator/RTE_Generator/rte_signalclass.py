##############################################################################
# rte_signalclass.py
#
# Description:   Create RTE signal classes
# Author:        Peter Fromm
#
# Input:         XML stream
# Output:        signal classes (header and source files)

import os

################################
## create_one_signal_class_file
def create_one_class_file(config, templatetype, signalclass, extension):
    
    name = signalclass.get("name")
    synchronisation = signalclass.get("synchronisation")
    
    #create filenames for template, out etc.
    template_file = config[templatetype] + "." + extension
    out_file = config["path_out"] + "\\" + name.lower() + "_" + templatetype + "." + extension
    
    
    if os.path.isfile(template_file): 

        fin = open(template_file,'r')
        fout = open(out_file,'w')

        print("    class: ", out_file )
    
        for line in fin:
            rline = line;
            rline = rline.replace("$Name/$",                name)
            rline = rline.replace("$NAME/$",                name.upper())
            rline = rline.replace("$Synchronisation/$",     synchronisation)
            rline = rline.replace("$SYNCHRONISATION/$",     synchronisation.upper())
            rline = rline.replace("$ReceiverTask/$",       config["taskEvent"])
            fout.write(rline)
        
        fin.close()
        fout.close()

## create_one_signal_class
################################
     


################################
## create_one_signal_class
def create_one_signal_class(signalclass, config):
    
    #Create signalclass file
    create_one_class_file(config, "signal", signalclass, "h")
    
    #Create typefile
    create_one_class_file(config, "type", signalclass, "h")
    create_one_class_file(config, "type", signalclass, "c")

  
## create_one_signal_class
################################


################################    