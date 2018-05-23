#!/usr/bin/env python
"""
Run 1D Cratering Simulation for different
object sizes and place results in separate folders.
"""

import os

# Read the impactor sized from a file
# One object size per line
with open("impactors.txt", "r") as f:
    print("Reading object sizes")
    ob_size = []

    for line in f:
        if line != "":
            ob_size.append(float(line))
        
print("Starting Simulation with %d objects of sizes: " % (len(ob_size)))

for size in ob_size:
    print(size)

# Compile the program
print("Compiling...")

# Use the SConstruct file to make the program.
os.system("scons")

sim_count = 1
for ob in ob_size:
    print("Starting Simulation %d of %d..." % (sim_count, len(ob_size))) 
    
    # Write the impactor size to file to be run by c++ program
    os.system("rm -f impactor_width.txt")
    with open("impactor_width.txt", "w") as f:
        f.write(str(ob))
    
    # Run the program and pipe the output
    os.system("./build/g++/release/rich > term_log.txt")
    
    # Make a folder for the sim data and move the reslts there
    f_name = "res/gamma1-1/res_" + str(ob)
    os.system("mkdir -p " + f_name)
    os.system("mv *.h5 " + f_name)
    os.system("mv term_log.txt " + f_name)
    os.system("mv velocity_history.txt " + f_name)
    os.system("mv dt.txt " + f_name)
    os.system("mv time.txt " + f_name)
    
    print("Finished %d of %d simulations" % (sim_count, len(ob_size)))
    sim_count += 1
    
# All the simulations are done, clean up the directory
os.system("rm -f impactor_width.txt")
print("%d Simulations Completed. See term_log.txt files for full output logs." % (len(ob_size)))
    
