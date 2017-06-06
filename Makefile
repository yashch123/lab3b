default:
	g++ -o lab3b -g lab3b.cpp
dist:
	tar -czvf lab3b-704630134.tar.gz lab3b.cpp Makefile README
clean:
	rm -rf lab3b lab3b-704630134.tar.gz 
