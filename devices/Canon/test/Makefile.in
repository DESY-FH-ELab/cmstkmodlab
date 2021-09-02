

MAIN 		= test2

.cc.o:
		g++ -c $<

$(MAIN): 	$(MAIN).o
		g++ -o $(MAIN) $(MAIN).o -L../../lib -lTkModLabCanon -lgphoto2
