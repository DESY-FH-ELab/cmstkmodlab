subdirs	      = quickplot \
		ReadGMH3750 \
		ReadTPG262 \
		ReadKeithley2700 \
		JulaboFP50Control \
		TkModLabRoot

all:
	@for dir in $(subdirs); do (cd $$dir; make); done

clean:
	@rm -f *~
	@for dir in $(subdirs); do (cd $$dir; make clean); done
	@rm -rf bin
