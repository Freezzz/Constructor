#!/usr/bin/env python
import json
import sys
import argparse

parser = argparse.ArgumentParser(description='Do some basic modifications to transform user level to story levels')
parser.add_argument('-f','--file', required=True, dest='fileName', help='read data from level with FILENAME')
parser.add_argument('-d','--dest', dest='destination', help='Path to save modified levels ( default: Resources/levels/story_levels/test_chapter ) ')
args = parser.parse_args()

fileName = args.fileName

storyLevelsPath = "Resources/levels/story_levels/test_chapter"
if args.destination:
	destination = args.destination
else:
	destination = storyLevelsPath

print "Using destiontin: " + destination


#Object Types
objectTypes = {0:"Box", 1:"Area", 3:"Spring"}

def dumpGameObjects(data):
	print "\n\nLEVEL OBEJECTS:"
	i = 0
	print "ID: \tBODY_ID: \tTYPE: \tSPECIAL \tMODIFIERS(delete/move/rotatre/static)"
	for gameObject in data["game objects"]:
		objectType = objectTypes.get(data["inventory items"][gameObject["inventory item"]]["type"])

		specials = "\t"
		if i == data["target"]:
			specials = "[TARGET]"
		if i == data["win area"]:
			specials = "[WINAREA]"
		
		modifiers = ""
		if gameObject["isDeletable"] == True:
			modifiers+="d"
		if gameObject["isMovable"] == True:
			modifiers+="m"
		if gameObject["isRotatable"] == True:
			modifiers+="r"
		if gameObject["isStatic"] == True:
			modifiers+="s"

		s = str(i)+" \t"+str(gameObject["body"]) + " \t\t" + objectType + "\t" + specials+ "\t"+modifiers
		
		print s

		i+=1
	return
def makeUneditable(data):
	for gameObject in data["game objects"]:
		gameObject["isDeletable"] = False
		gameObject["isMovable"] = False
		gameObject["isRotatable"] = False	
	print "Convertion Done"	
	return
def resetTarget(data):
	dumpGameObjects(data)
	print "New target index:"
	newIndex = int(sys.stdin.readline().strip())
	if newIndex > len(data["game objects"]) - 1:
		print "Invalid index (should be lesser then " + str(len(data["game objects"])-1) + ")"
		resetTarget(data)
	elif newIndex == data["win area"]:
		print "Invalid index ( should not be equal to WINAREA)"
		resetTarget(data)		
	else:
		data["target"] = newIndex
	return
def writeOut(data):
	print "File name:"
	reply = sys.stdin.readline().strip();
	s = json.dumps(data,sort_keys=True, indent=2)	
	f = open(destination+"/"+reply, 'w+')
	f.write(s)
	f.close()
	print "Done: "
	return

def printMenu(data):
	while True:
		print "\n---==== Level Convertion ====---"		
		print "1: Dump GameObjects info"
		print "2: Convert all objects to uneditable"
		print "3: Change/Set target"
		print "\n5: WriteOut"
		print "0: Exit"		
		reply = sys.stdin.readline().strip()
		if reply == "1":
			dumpGameObjects(data)
		elif reply == "2":
			makeUneditable(data)
		elif reply == "3":
			resetTarget(data)
		elif reply == "5":
			writeOut(data)
		else:
			return

json_data=open(fileName)
level_data = json.load(json_data)
json_data.close()
printMenu(level_data)

