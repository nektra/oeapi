import zipfile
import os
import sys

import buildversion

dir = sys.argv[1]
destdir = sys.argv[2]
project = sys.argv[3]
releasename = sys.argv[4]

zfilename = destdir +"/"+ project + "_v" +buildversion.version+"."+buildversion.wcrev+"_"+releasename+"_symbols.zip"

dir_list = os.listdir(dir)

symbol_files = filter(lambda x: x.endswith('.pdb'), dir_list)

zout = zipfile.ZipFile(zfilename, "w")
sys.stderr.write("Creating symbol zip "+zfilename+"\n")
for fname in symbol_files:
	zout.write(dir+"/"+fname,fname)
	sys.stderr.write("Saving symbol "+fname+"\n")
zout.close()
