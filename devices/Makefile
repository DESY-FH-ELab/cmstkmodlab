subdirs	      = Julabo \
		Keithley \
                Greisinger \
		Hameg \
		Canon \
		Conrad \
		Pfeiffer

all:
	@for dir in $(subdirs); do (cd $$dir; make); done

clean:
	@rm -f *~
	@for dir in $(subdirs); do (cd $$dir; make clean); done
	@rm -rf lib
