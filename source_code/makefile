CC = g++ -std=c++17
CFLAGS = -Wall -Wextra -Werror
BACKUP = backup_treasure_hunt_2
APP = TreasureHunt2.app
BUILD_FOLDER=build

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	LFLAGS += -pthread -lsubunit
	OPEN = -xdg-open
	INSTFOLDER = $(HOME)/trading
else
	OPEN = -open
	INSTFOLDER = $(HOME)/Applications/$(APP)
endif

PROFILE = TreasureHunt2.pro

all : clean build

build : mk replication
	cd $(BUILD_FOLDER); qmake $(PROFILE)
	make -C $(BUILD_FOLDER)/
	cd $(BUILD_FOLDER); $(RM) -rf *.cpp *.h *.hpp *.ui *.o *.icns *.pro *.user *.qrc controller model view img *.png Makefile

mk :
	-mkdir $(BUILD_FOLDER)/

replication :
	cp $(PROFILE) $(BUILD_FOLDER)/
	cp -rf controller $(BUILD_FOLDER)/
	cp -rf model $(BUILD_FOLDER)/
	cp -rf view $(BUILD_FOLDER)/
	cp -rf img $(BUILD_FOLDER)/
	cp main.cpp $(BUILD_FOLDER)/
	cp logo_game.png $(BUILD_FOLDER)/
	cp image.qrc $(BUILD_FOLDER)/


dist : clean
	-rm -rf $(BACKUP)
	-mkdir $(BACKUP)
	-cp $(PROFILE) $(BACKUP)/
	-cp -rf controller $(BACKUP)/
	-cp -rf model $(BACKUP)/
	-cp -rf view $(BACKUP)/
	-cp -rf img $(BACKUP)/
	-cp main.cpp $(BACKUP)/
	-cp logo_game.png $(BACKUP)/
	-cp image.qrc $(BACKUP)/
	-cp makefile $(BACKUP)/
	tar -cvzf $(HOME)/Desktop/TreasureHunt2.tgz $(BACKUP)/
	rm -rf $(BACKUP)/


clean :
	$(RM) -rf $(BUILD_FOLDER)
