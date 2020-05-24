import sys
from mod_pbxproj import XcodeProject
print "Komut Satiri Argumanlari"
arg=sys.argv

if len(arg)<2:
    exit()

del arg[0]

try:
    xcpIndex=arg.index("-xcp")
except ValueError:
    xcpIndex=-1

try:
    fpIndex=arg.index("-fp")
except ValueError:
    fpIndex=-1

try:
    rfpIndex=arg.index("-rfp")
except ValueError:
    rfpIndex=-1

try:
    rpfIndex=arg.index("-rfp")
except ValueError:
    rpfIndex=-1

try:
    nmIndex=arg.index("-nm")
except ValueError:
    nmIndex=-1

try:
    pfpIndex=arg.index("-pfp")
except ValueError:
    pfpIndex=-1

try:
    rpfpIndex=arg.index("-rpfp")
except ValueError:
    rpfpIndex=-1

'''Add Files Needs=> Relative File Path List(file1.cpp,file2.cpp ), Relative Parent Folder Path'''
if arg[0]=="-addfiles":
    if len(arg)<5:
        print "Error! Need to more argumants"
        exit()
    
    if xcpIndex!=-1:
        project = XcodeProject.Load(arg[ xcpIndex+1]+"/project.pbxproj")
        print "Xcode Path: "+arg[ xcpIndex+1]
    else:
        print "Error: Need to -xcp argumant (Xcode project file path)"
        exit()
    if rfpIndex!=-1:
        relativeFilePath=arg[ rfpIndex+1 ]
        print "Relative File Path: "+arg[ rfpIndex+1 ]
    else:
        print "Error: Need to -rfp argumant (Relative File Path List)"
        exit()
    if rpfpIndex!=-1:
        relativeParentFolderPath=arg[ rpfpIndex+1 ]
        print "Target Folder: "+arg[ rpfpIndex+1 ]
    else:
        relativeParentFolderPath=""
    if relativeParentFolderPath!="":
        matchedGroups=project.get_groups_by_os_path(relativeParentFolderPath)
        if len(matchedGroups)==0:
            print "Error: Can't find parent folder path!"
            exit()
        else:
            parentGroup=matchedGroups[0]
    multipleFiles=relativeFilePath.split(",")
    print len(multipleFiles)
    for fPart in multipleFiles:
        if relativeParentFolderPath=="":
            project.add_file_if_doesnt_exist(fPart)
        else:
            project.add_file_if_doesnt_exist(fPart, parent=parentGroup)
        print "added file:"+fPart
    project.save()


'''Add Folder Needs=> Folder Path(-fp), Relative Parent Folder Path(-rpfp) '''
if arg[0]=="-addfolder":
    if len(arg)<5:
        print "Error! Need to more argumants"
        exit()
    
    if xcpIndex!=-1:
        project = XcodeProject.Load(arg[ xcpIndex+1]+"/project.pbxproj")
        print "Xcode Path: "+arg[ xcpIndex+1]
    else:
        print "Error: Need to -xcp argumant (Xcode project file path)"
        exit()
    if fpIndex!=-1:
        filePath=arg[ fpIndex+1 ]
        print "Folder Path: "+arg[ fpIndex+1 ]
    else:
        print "Error: Need to -fp argumant (Folder path)"
        exit()
    if rpfpIndex!=-1:
        relativeParentFolderPath=arg[ rpfpIndex+1 ]
        print "Target Folder: "+arg[ rpfpIndex+1 ]
    else:
        relativeParentFolderPath="";
    if relativeParentFolderPath=="":
        project.add_folder(filePath)
    else:
        matchedGroups=project.get_groups_by_os_path(relativeParentFolderPath)
        if len(matchedGroups)==0:
            print "Error: Can't find folder path!"
            exit()
        else:
            group=matchedGroups[0]
        project.add_folder(filePath,parent=group)
    project.save()

'''Remove Files Needs-> Relative File Path List (class1.cpp,class1.h,class2.cpp...)'''
if arg[0]=="-rmfiles":
    if len(arg)<5:
        print "Error! Need to more argumants"
        exit()
    
    if xcpIndex!=-1:
        project = XcodeProject.Load(arg[ xcpIndex+1]+"/project.pbxproj")
        print "Xcode Path: "+arg[ xcpIndex+1]
    else:
        print "Error: Need to -xcp argumant (Xcode project file path)"
        exit()
    if rfpIndex!=-1:
        relativeFilePaths=arg[ rfpIndex+1 ]
        print "File Path: "+arg[ rfpIndex+1 ]
    else:
        print "Error: Need to -fp argumant (Class file path)"
        exit()
    multipleFiles=relativeFilePaths.split(",")
    for fPart in multipleFiles:
        project.remove_file_by_path(fPart)
        print "Removed => " + fPart

    project.save()

'''Remove Folder Needs=> Relative Folder Path(-rfp) '''
if arg[0]=="-rmfolder":
    if len(arg)<5:
        print "Error! Need to more argumants"
        exit()
    
    if xcpIndex!=-1:
        project = XcodeProject.Load(arg[ xcpIndex+1]+"/project.pbxproj")
        print "Xcode Path: "+arg[ xcpIndex+1]
    else:
        print "Error: Need to -xcp argumant (Xcode project file path)"
        exit()
    if rfpIndex!=-1:
        relativeFolderPath=arg[ rfpIndex+1 ]
        print "Folder Path: "+arg[ rfpIndex+1 ]
    else:
        print "Error: Need to -fp argumant (Folder Path)"
        exit()
    matchedGroups=project.get_groups_by_os_path(relativeFolderPath)
    if len(matchedGroups)==0:
        print "Error: Can't find folder path!"
        exit()
    else:
        groupID=matchedGroups[0].id
    project.remove_group(groupID)
    project.save()

'''Update Folder ::> Needs: folder path,relative folder path, relative parent folder path '''
if arg[0]=="-updatefolder":
    if len(arg)<5:
        print "Error! Need to more argumants"
        exit()
    
    if xcpIndex!=-1:
        project = XcodeProject.Load(arg[ xcpIndex+1]+"/project.pbxproj")
        print "Xcode Path: "+arg[ xcpIndex+1]
    else:
        print "Error: Need to -xcp argumant (Xcode project file path)"
        exit()
    if fpIndex!=-1:
        folderPath=arg[ fpIndex+1 ]
        print "Folder Path: "+arg[ fpIndex+1 ]
    else:
        print "Error: Need to -fp argumant (Folder path)"
        exit()
    if rfpIndex!=-1:
        relativeFolderPath=arg[ rfpIndex+1 ]
        print "Relative Folder Path: "+arg[ rfpIndex+1 ]
    else:
        print "Error: Need to -rpf argumant (Relative folder Path)"
        exit()
    if rpfpIndex!=-1:
        relativeParentFolderPath=arg[ rpfpIndex+1 ]
        print "Relative Parent Folder Path: "+arg[ rpfpIndex+1 ]
    else:
        relativeParentFolderPath=""
    matchedGroups=project.get_groups_by_os_path(relativeFolderPath)
    if len(matchedGroups)>0:
        groupID=matchedGroups[0].id
        project.remove_group(groupID)
        print "->Deleted group."
    if relativeParentFolderPath!="":
        matchedGroups=project.get_groups_by_os_path(relativeParentFolderPath)
        if len(matchedGroups)>0:
            parentGroup=matchedGroups[0]
        else:
            print "Error: Can't find relative parent folder path: "+relativeParentFolderPath
            exit()
        project.add_folder(folderPath,parent=parentGroup)
        print "->Added "+folderPath+" located folder to project."
    else:
        project.add_folder(folderPath)
        print "->Added "+folderPath+" located folder to project."

    project.save()

exit()


