#include <unistd.h>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <ulimit.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <string.h>
#include <thread>
#include <pthread.h>
#include <unistd.h> //usleep
#include "../Utils/api_gpio/pmap.h"
#include "../Utils/api_gpio/pin.h"
#include "../Utils/utils.h"

#define PORT_LDR 4 //Light Dependent Resistor
#define PORT_POT 1 //potentiometer

using namespace std;
int n = 20, m = 20;
bool jump = false;
bool arm = false;
bool running = true;
int atual = 2048;

void draw() {
	if(!arm) {
		if(!jump) {
			cout << "\r" << endl << endl << endl << endl << endl << endl << endl;
			string s1 = "";
			string s2 = "";
			for(int i = 0; i < n; i++) {
				s1 += " ";
			}
			for(int i = 0; i < m; i++) {
				s2 += " ";
			}
			cout << s1 << " o " << s2 << endl << s1 << "`|´" << s2 << endl << s1 << " ^ " << s2 << endl;
		}
		else {
			cout << "\r" << endl << endl << endl << endl;
			string s1 = "";
			string s2 = "";
			for(int i = 0; i < n; i++) {
				s1 += " ";
			}
			for(int i = 0; i < m; i++) {
				s2 += " ";
			}
			cout << s1 << " o " << s2 << endl << s1 << "`|´" << s2 << endl << s1 << " ^ " << s2 << endl;
			cout << endl << endl << endl;
		}
	}
	else {
		if(!jump) {
			cout << "\r" << endl << endl << endl << endl << endl << endl << endl;
			string s1 = "";
			string s2 = "";
			for(int i = 0; i < n; i++) {
				s1 += " ";
			}
			for(int i = 0; i < m; i++) {
				s2 += " ";
			}
			cout << s1 << " o " << s2 << endl << s1 << "`|," << s2 << endl << s1 << " ^ " << s2 << endl;
		}
		else {
			cout << "\r" << endl << endl << endl << endl;
			string s1 = "";
			string s2 = "";
			for(int i = 0; i < n; i++) {
				s1 += " ";
			}
			for(int i = 0; i < m; i++) {
				s2 += " ";
			}
			cout << s1 << " o " << s2 << endl << s1 << "`|," << s2 << endl << s1 << " ^ " << s2 << endl;
			cout << endl << endl << endl;
		}
	}
}

void read_pot(){
	int number = 1;
	while(running) {
		number = readAnalog(PORT_POT);
    	//cout << n << " " << m << " " << number<< endl;
    	if(number > atual + 10) {
    		atual = number;
    		if (n > 0) {
				n--;
				m++;
			}
    	}
    	else if (number < atual - 10){
    		atual = number;
    		if (m > 0) {
				m--;
				n++;
			}
    	}
    	system("clear");
    	draw();
    	number = 1;
    	std::this_thread::sleep_for( std::chrono::milliseconds(300) );
    }

}

void read_botao(){
	init();
	Pin btn ("P9_27", Direction::IN, Value::LOW);
	int btnValue;
	while(running){
		btnValue = btn.getValue();
		if (btnValue == 1){
			jump = true;
			system("clear");
			draw();
			usleep(1500000);
			jump = false;
		}
		system("clear");
		draw();
		std::this_thread::sleep_for( std::chrono::milliseconds(300) );
	}

}

void read_luz(){
	int number = 1;
	while (running){
		number = readAnalog(PORT_LDR);
		if(number < 1000) {
			arm = true;
			system("clear");
			draw();
			usleep(1500000);
			arm = false;
		}
		system("clear");
		draw();
		std::this_thread::sleep_for( std::chrono::milliseconds(300) );
	}

}


int main() {
	system("clear");
	draw();
	char c;
	std::thread pot(read_pot);
	std::thread botao(read_botao);
	std::thread luz(read_luz);
	struct sched_param param1;
	struct sched_param param2;
	struct sched_param param3;
	param1.sched_priority = sched_get_priority_max(SCHED_RR);
	param3.sched_priority = sched_get_priority_min(SCHED_RR);
	param2.sched_priority = sched_get_priority_max(SCHED_RR)/2;
	pthread_setschedparam(pot.native_handle(), SCHED_RR, &param1);
	pthread_setschedparam(luz.native_handle(), SCHED_RR, &param2);
	pthread_setschedparam(botao.native_handle(), SCHED_RR, &param3);
	cin >> c;
	if(c == 'q') {
		running = false;
	}
	pot.join();
	botao.join();
	luz.join();

}