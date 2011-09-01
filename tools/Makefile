subdirs	      = LabControl2Root \
		LabControl2Report \
		quickplot

all:
	@for dir in $(subdirs); do (cd $$dir; make); done

clean:
	@rm -f *~
	@for dir in $(subdirs); do (cd $$dir; make clean); done
	@rm -rf bin
