#!../../bin/windows-x64-debug/randomTest

## You may have to change randomTest to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/randomTest.dbd"
randomTest_registerRecordDeviceDriver pdbbase

##ISIS## Run IOC initialisation 
< $(IOCSTARTUP)/init.cmd

## Load record instances

##ISIS## Load common DB records 
< $(IOCSTARTUP)/dbload.cmd

## Load our record instances
dbLoadRecords("db/randomTest.db","P=$(MYPVPREFIX)$(IOCNAME):, D=Random, S=324235")
dbLoadRecords("db/randomTest.db","P=$(MYPVPREFIX)$(IOCNAME):ASYNC: ,D=Random Async,S=324235")

##ISIS## Stuff that needs to be done after all records are loaded but before iocInit is called 
< $(IOCSTARTUP)/preiocinit.cmd

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=faa59Host"

##ISIS## Stuff that needs to be done after iocInit is called e.g. sequence programs 
< $(IOCSTARTUP)/postiocinit.cmd
