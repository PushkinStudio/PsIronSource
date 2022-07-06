UPLFile = open("PsIronSource_UPL_IOS.xml", "r")
UPLContents = UPLFile.readlines()
UPLFile.close()

Networks = open("skadnetworks", "r")
NetworksLines = Networks.readlines()
Networks.close()

NetworkDictionary = """
<setElement result="dSKAdNetworkItemDict_{Name}" value="dict"/>
<addElement tag="$dSKAdNetworkItemDict_{Name}" name="dSKAdNetworkIdentifierKey"/>
<setElement result="dSKAdNetworkId_{Name}" value="string" text="{NetworkId}"/>
<addElement tag="$dSKAdNetworkItemDict_{Name}" name="dSKAdNetworkId_{Name}"/>
<addElement tag="$" name="dSKAdNetworkItemDict_{Name}"/>
"""

for Index, Line in enumerate(UPLContents):
	if Line.strip() == "<!-- SKAdNetworkId start -->":
		StartIndex = Index
	elif Line.strip() == "<!-- SKAdNetworkId end -->":
		EndIndex = Index
		break

if 'StartIndex' not in globals():
	print("Can't find start marker")
	quit()

if 'EndIndex' not in globals():
	print("Can't find end marker")
	quit()

del UPLContents[StartIndex + 1:EndIndex]

for Index, Line in enumerate(UPLContents):
	if Line.strip() == "<!-- SKAdNetworkId start -->":
		for NetworkId in NetworksLines:			
			UPLContents.insert(Index + 1, NetworkDictionary.format(Name=NetworkId.split(".")[0], NetworkId=NetworkId.rstrip()))
		break

UPLFile = open("PsIronSource_UPL_IOS.xml", "w")
UPLFile.writelines(UPLContents)
UPLFile.close()