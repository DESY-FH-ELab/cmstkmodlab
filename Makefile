BASEPATH      = $(shell pwd)

subdirs	      = devices \
		thermal \
		tools

qtsubdirs     = thermal

all: qt scripts
	@for dir in $(subdirs); do (cd $$dir; make); done

qt:
	@for dir in $(qtsubdirs); do (cd $$dir; qmake); done

scripts: tkmodlabenv.sh.in tkmodlabenv.csh.in
	@sed -e 's?@basepath@?$(BASEPATH)?g' tkmodlabenv.sh.in > tkmodlabenv.sh
	@sed -e 's?@basepath@?$(BASEPATH)?g' tkmodlabenv.csh.in > tkmodlabenv.csh

clean:
	@rm -f *~
	@for dir in $(subdirs); do (cd $$dir; make clean); done
	@rm -f tkmodlabenv.sh
	@rm -f tkmodlabenv.csh
