TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += random

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install random.dbd into <top>/dbd
DBD += random.dbd

# specify all source files to be compiled and added to the library
random_SRCS += devRand.c devRandAsync.c

random_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

