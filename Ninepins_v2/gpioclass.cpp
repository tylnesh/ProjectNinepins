/*
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "gpioclass.h"

using namespace std;

GPIOClass::GPIOClass()
{
    this->gpionum = "4"; //GPIO4 is default
}

GPIOClass::GPIOClass(string gnum)
{
    this->gpionum = gnum;  //Instatiate GPIOClass object for GPIO pin number "gnum"
}

int GPIOClass::export_gpio()
{
    string export_str = "/sys/class/gpio/export";
    ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    if (exportgpio < 0){
        cout << " OPERATION FAILED: Unable to export GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }

    exportgpio << this->gpionum ; //write GPIO number to export
    exportgpio.close(); //close export file
    return 0;
}

int GPIOClass::unexport_gpio()
{
    string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    if (unexportgpio < 0){
        cout << " OPERATION FAILED: Unable to unexport GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }

    unexportgpio << this->gpionum ; //write GPIO number to unexport
    unexportgpio.close(); //close unexport file
    return 0;
}

int GPIOClass::setdir_gpio(string dir)
{

    string setdir_str ="/sys/class/gpio/gpio" + this->gpionum + "/direction";
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
        if (setdirgpio < 0){
            cout << " OPERATION FAILED: Unable to set direction of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }

        setdirgpio << dir; //write direction to direction file
        setdirgpio.close(); // close direction file
        return 0;
}

int GPIOClass::setval_gpio(string val)
{

    string setval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
        if (setvalgpio < 0){
            cout << " OPERATION FAILED: Unable to set the value of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }

        setvalgpio << val ;//write value to value file
        setvalgpio.close();// close value file
        return 0;
}

int GPIOClass::getval_gpio(string& val){

    string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ifstream getvalgpio(getval_str.c_str());// open value file for gpio
    if (getvalgpio < 0){
        cout << " OPERATION FAILED: Unable to get value of GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
            }

    getvalgpio >> val ;  //read gpio value

    if(val != "0")
        val = "1";
    else
        val = "0";

    getvalgpio.close(); //close the value file
    return 0;
}

string GPIOClass::get_gpionum(){

return this->gpionum;

}
