#include "SoftwareSerial.h"


class BluetoothSerial{
private:
	int rxPin;
	int txPin;
	bool incoming = false;
	char packet[4];
	int counter = 0;


	SoftwareSerial* bt;
public:
	BluetoothSerial(int rxPin, int txPin){
		this->rxPin = rxPin;
		this->txPin = txPin;
		bt = new SoftwareSerial(txPin, rxPin);
	}

	bool newData(){
		if(!(bt->available())) return false;
		
		char in = bt->read();
		if(in == 'S')
			incoming = true;
		if(!incoming) return false;

		while(incoming){
			in = bt->read();
			if(in == 'E')
				incoming = false;
			else 
				packet[counter++] = in;
		}
		if(counter<3) return false;
		
		counter = 0;
		return true;	
	}


	/*
	Returns the data sent to this bluetooth serial device. 
		Structure 4 bytes
		Throttle|Yaw|Pitch|Roll
	*/
	void getPacket(char* out){
		for(int i=0; i<4; i++)
			out[i] = packet[i];
	}

};