#include "simulation.h"
#include <iostream>

Head *TeaRoom, *WaitingLine;
double ThroughTime, SimPeriod, P; 
long NoOfCustomers, MaxLength, N, U;

class Car : public Process {
    void Actions() {
        double EntryTime = Time();
        Into(WaitingLine);
        long QLength = WaitingLine->Cardinal();
        if (MaxLength < QLength) 
            MaxLength = QLength;
        if (!TeaRoom->Empty()) 
            Activate((Process*) TeaRoom->First());
        Passivate(); 
        NoOfCustomers++;
        ThroughTime += Time() - EntryTime;
    }			
};

class CarWasher : public Process {
    void Actions() {
        for(;;) { 
            Out(); 
            while (!WaitingLine->Empty()) {
                Car *Served = (Car*) WaitingLine->First();
                Served->Out();
                Hold(10);
                Activate(Served);
                delete Served;
           }
           Wait(TeaRoom);
        }
    }
};

class CarGen : public Process {
     void Actions() {
         while (Time() <= SimPeriod) {
             Activate(new Car);
             Hold(Negexp(1/P,U));
        }
    }
};

void Report() {
    std::cout << N << " Car washer simulation\n";
    std::cout << "No.of cars through the system = " << NoOfCustomers << std::endl;
    std::cout << "Av.elapsed time = " << ThroughTime/NoOfCustomers << std::endl;
    std::cout << "Maximum queue length = " << MaxLength << std::endl;
}

void CarWash() {
    P = 11; N = 2; SimPeriod = 200; U = 5;
    TeaRoom = new Head;
    WaitingLine = new Head;
    for (int i = 1; i <= N; i++)
        (new CarWasher)->Into(TeaRoom);
    Activate(new CarGen); 
    Hold(SimPeriod+10000000);
    Report();
}

int main() Simulation(CarWash())