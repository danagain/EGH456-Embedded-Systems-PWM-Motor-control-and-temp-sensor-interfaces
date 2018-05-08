# invoke SourceDir generated makefile for empty_min.pem4f
empty_min.pem4f: .libraries,empty_min.pem4f
.libraries,empty_min.pem4f: package/cfg/empty_min_pem4f.xdl
	$(MAKE) -f C:\Users\zachh\Documents\RTOS\Assignment\EGH456\empty_min_DK_TM4C129X_TI/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\zachh\Documents\RTOS\Assignment\EGH456\empty_min_DK_TM4C129X_TI/src/makefile.libs clean

