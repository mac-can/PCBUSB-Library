LIB = PCBUSB

SRC = src

CFLAGS =
LDFLAGS = 

TARGET1 = pcbusb_send
FILES1 = $(SRC)/$(TARGET1).cpp
 
TARGET2 = pcbusb_recv
FILES2 = $(SRC)/$(TARGET2).cpp
 
all: $(TARGET1) $(TARGET2)

$(TARGET1):	$(FILES1)
	g++ $(FILES1) $(CFLAGS) -l $(LIB) $(LDFLAGS) -o $(TARGET1)

$(TARGET2):	$(FILES2)
	g++ $(FILES2) $(CFLAGS) -l $(LIB) $(LDFLAGS) -o $(TARGET2)

clean:
	@-rm -f $(SRC)/*~ $(SRC)/*.o *~ $(TARGET1) $(TARGET2)
	
install:
	@-cp $(TARGET1) /usr/local/bin
	@-cp $(TARGET2) /usr/local/bin
