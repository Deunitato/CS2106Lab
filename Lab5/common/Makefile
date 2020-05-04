all:	USFATI USFATNew USFATSetup

USFATI:	USFAT_Inspect.c USFAT_Util.c USFAT.h
	gcc USFAT_Inspect.c USFAT_Util.c -o USFATI

USFATNew:	USFAT_New.c USFAT_Util.c USFAT.h
	gcc USFAT_New.c USFAT_Util.c -o USFATNew

USFATSetup:	USFAT_Setup.c USFAT_Util.c USFAT.h
	gcc USFAT_Setup.c USFAT_Util.c -o USFATSetup

clean:
	rm USFATI USFATNew USFATSetup
