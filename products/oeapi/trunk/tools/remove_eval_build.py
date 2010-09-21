import sys 
import xml.dom.minidom
from xml.dom.minidom import parse,Node

projfile = sys.argv[1]
 
doc = parse(projfile)
 
mapping = {}

sys.stderr.write("processing "+projfile+"\n")
 
for node in doc.getElementsByTagName("Configuration"):
    confname = node.getAttribute("Name")
    if confname.startswith("Evaluation"):
        node.parentNode.removeChild(node)
        sys.stderr.write("Configuration \""+ confname + "\" removed\n")


for node in doc.getElementsByTagName("FileConfiguration"):
    confname = node.getAttribute("Name")
    if confname.startswith("Evaluation"):
        node.parentNode.removeChild(node)
	sys.stderr.write("FileConfiguration \""+ confname + "\" removed\n")

newfile = projfile.replace('.vcproj','-distro.vcproj');

print doc.toprettyxml()

#f = open(newfile, 'w')
#doc.writexml(f)
#f.close()
