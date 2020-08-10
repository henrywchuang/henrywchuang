#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

#define NUM_THREADS 3

//Linux GPIO number // Aruino Shield Pin Name
#define Strobe (40) // 8
#define GP_4 (48) // A0
#define GP_5 (50) // A1
#define GP_6 (52) // A2
#define GP_7 (54) // A3
#define GPIO_DIRECTION_IN (1)
#define GPIO_DIRECTION_OUT (0)
#define ERROR (-1)
// Galileo commands
#define MSG_RESET (0x0)
#define MSG_PING (0x1)
#define MSG_GET (0x2)
#define MSG_ON (0x3)
#define MSG_OFF (0x4)
// PIC responses
#define MSG_ACK (0xE)

//WEB SERVER Libraries
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/stat.h>

//GLOBAL Variables
int adc_result; //global variable
int LDR_ctrl; //ctrl variable
int quit; //ctrl variable
int TEMP_SENSOR; //ctrl variable
int WEB;
int i;
int j=3; //iterators for loop and picture name
int k=0;
char pic_status[50];
//Temperature control definitions
double ambient_temp;
double curr_temp; // double value to display temperature
double Total; //used for computation of ambient_temp and temp_std

pthread_mutex_t mutexsum;

void HTTP_POST(const char* url, const char* image, int size){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,(long) size);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, image);
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
				curl_easy_cleanup(curl);
	}
}

int TakePicture(int num)
{
	char buffer[100];
	CvCapture *capture = 0; //pointer to object that can read from camera
	printf("Picture Capture Begun\n");
capture = cvCaptureFromCAM(CV_CAP_ANY); //0 is passed because there is only 1 camera

	if (capture == NULL){
		printf("failure\n");
	}
	else{
		printf("Camera is connected\n");
	}

	IplImage *Img; //defines Img as a IplImage structure

	Img = cvQueryFrame(capture);


	sprintf(buffer,"/media/card/CVsave_pic_%d.jpg",num);

	cvSaveImage(buffer, Img);
	cvReleaseCapture(&capture);
	WEB=1;
	j=3;
	return 0;
}

void Get_tempsensor()
{
	int r;
	int fd;
		char i2c_path[] = "/dev/i2c-0"; //path to i2c-0 the temp sensor
	int addr = 0x48; //0100 1000

	unsigned char command[2]; // vector of 2 bytes used to control temp sensor
	unsigned char temp_reg[2]; // vector of 2 bytes used to read temp sensor
		int byte0 = temp_reg[0]; //MSB of temp regisiter upper 8 bits
		int byte1 = temp_reg[1]; //LSB of temp regisiter lower 4 bits (4 unused)


		fd = open(i2c_path, O_RDWR );
	if(fd < 0){
			printf("Opening i2c device node\n");
	}
		r = ioctl(fd, I2C_SLAVE, addr);
	if(r < 0){
			printf("Selecting i2c device\n");
			}

			command[0] = 0x00;
			r = write(fd, command, 1);
			if (r < 0){
				printf("error writing command\n");
			}
			usleep(50000);

			r = read(fd,temp_reg,2);
			if (r < 0){
				printf("error reading temp\n");
			}
			byte0 = temp_reg[0]; //MSB of temp regisiter upper 8 bits
			byte1 = temp_reg[1];
			if ((byte0 & 0b10000000)==0){
				curr_temp=((byte0 << 4) + (byte1 >> 4))*0.0625;
			}
			else{
				curr_temp=(~(((byte0)<<4) + (((byte1))>>4))+1)*-0.0625; //below 0 degrees C
			}
			//printf("Temperature = %f\n\n",curr_temp);
			sleep(1);

			if(i==1){
				//printf("Sensor adjsuting to ambient temperature please wait.\n");
				Total=curr_temp; //initial temp setting
			}
			else if(i<5){
				//printf("Sensor adjsuting to ambient temperature please wait.\n");
				Total = Total + curr_temp;
			}
			else if((i % 20) == 0){
				Total=curr_temp; //20 second averaging
				i=1; //reset loop counter
			}
			else if(j!=3){
				Total=Total+ambient_temp; //don't want pressing sensor to effect average
			}
			else{
				Total = Total + curr_temp;
			}
			pthread_mutex_lock(&mutexsum);
			ambient_temp = Total/i;
			pthread_mutex_unlock(&mutexsum);
			//printf("ambient temp = %f\n",ambient_temp);

			if ((curr_temp-ambient_temp)>0.2){
				printf("Taking picture in %d seconds, iteration is : %d\n",j,i);
				sleep(0.5);

				if (j == 0){
					TakePicture(k);
						k++;
						printf("Picture Acquired\n");
						j=3;
						printf("Sensor cooling down to ambient temperature, please wait.\n");
						sleep(5);
						i=0;
					}
				j--;
			}
		}
// Function called to get LDR value
void Get_LDR()
{
	int nibble1, nibble2, nibble3;
	int curr_nibble = 1;
	int msg;
	int ack_1, ack2;
	curr_nibble = 1;
		// Strobe High - tells PIC I'm ready to send data
			writeGPIO(MSG_PING); //PING the PIC microprocessor

			msg = readGPIO('c'); //wait for MSG_ACK from PING
			if (msg == MSG_ACK){ //Do stuff, the PIC has successfully read...
				pthread_mutex_lock(&mutexsum);
				sprintf(pic_status,"ping+sucessful");
				pthread_mutex_unlock(&mutexsum);
			}
			else{
				pthread_mutex_lock(&mutexsum);
				sprintf(pic_status,"ping+unsuccesful");
				pthread_mutex_unlock(&mutexsum);
				return;
			}
			writeGPIO(MSG_RESET); //Send message to PIC reset your sensor

			msg = readGPIO('c');
			if (msg == MSG_ACK){ //Do stuff, the PIC has successfully read...
			}
			else{
				return;
			}
			writeGPIO(MSG_GET); //Send command MSG_GET

			while(curr_nibble <= 3){
				if (curr_nibble == 1){ //3rd nibble with LSB and 2 extra bits
					nibble1 = readGPIO('n');
				}
				else if (curr_nibble == 2){
					nibble2 = readGPIO('n'); //2nd nibble with middle 4 bits
				}
				else if (curr_nibble == 3){
					nibble3 = readGPIO('n'); //1st nibble with MSB
				}
				curr_nibble++;
			}
			pthread_mutex_lock(&mutexsum);
			adc_result = (nibble1<<6) + (nibble2<<2) + (nibble3>>2);
			pthread_mutex_unlock(&mutexsum);

			if (adc_result>250){
				writeGPIO(MSG_ON);
			}
			else{
				writeGPIO(MSG_OFF);
			}
			LDR_ctrl=0;
}

//Takes all User interface commands and controlls all threads
void *UCI(void *threadid)
{
		int tid;
	tid = (int)threadid;

	char command; //control of main while loop
	int msg;
	int fileHandleGPIO_S;

	while(command != 'Q' && command != 'q')
{
			fileHandleGPIO_S = openGPIO(Strobe, GPIO_DIRECTION_OUT);
			printf("\nEnter Q to QUIT\n");
			printf("Enter G to get 10 bit ADC result from LDR\n");
			printf("Enter T to get ambient Temperature\n");
			scanf(" %c",&command);

			switch (command){
			case 'Q': case 'q': //Quits program
				pthread_mutex_lock(&mutexsum);
				quit=1;
				pthread_mutex_unlock(&mutexsum);
				break;
			case 'G': case 'g':
				LDR_ctrl=1;
				printf("LDR result is %d\n",adc_result);
				break;
			case 'T': case 't':
				TEMP_SENSOR = 1;
				if (i <= 5){
					printf("Ambient Temp not stable, wait a few seconds and try again.\n", ambient_temp);
				}
				else{
					printf("Ambient Temp result is %f\n", ambient_temp);
				}
				break;
			default:
				printf("Invalid command %c\n", command);
				char junk; // Used to clear line
				do {
					scanf("%c", &junk);
				} while (junk != '\n');
			}
	}
}

//Continually calls Get_tempsensor and Get_LDR
void *SSC(void *threadid)
{
	i=0; //iterators for loop and picture name
	while(1){
		i++;
		Get_tempsensor();
		Get_LDR();
		if(quit==1){
			break;
		}
	}
}

void *CSC(void *threadid)
{
	const char* hostname="10.253.75.114";
	const int port=8000;
	const int id=6;
	const char* password="password";
	const char* name="Group06";
	const char* status=pic_status;
	// const char* timestamp="20170428-11:30:44";
	const char* filename="/media/card/CVsave_pic.jpg";

	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d-%X", &tstruct);
	const char* timestamp=buf;

	while(1){
		if (WEB==1){
			char buf[1024];
				snprintf(buf, 1024,
"http://%s:%d/update?id=%d&password=%s&name=%s&data=%d&status=%s&timestamp=%s&filename=%s",
				hostname,
				port,
				id,
				password,
				name,
				adc_result,
				status,
				timestamp,
				filename);

		FILE *fp;

		struct stat num;
		stat(filename, &num);
		int size = num.st_size;
		printf("Image size: %dB\n", size);

		char *buffer = (char*)malloc(size);

		fp = fopen(filename,"rb");
		int n = fread(buffer, 1, size, fp);

		HTTP_POST(buf, buffer, size);
		fclose(fp);
		WEB=0; //update status is waiting
		}
	}
}

int main(void)
{
		pthread_t threads[NUM_THREADS];
	int ret;
		printf("main() : creating thread, 1\n");

		ret = pthread_create(&threads[0], NULL, UCI, (void *)0);
	if (ret){
			printf("Error:unable to create thread %d\n", ret);
	return -1;
	}
//Creating Thread 2 for Sensor Control Thread
	ret = pthread_create(&threads[1], NULL, SSC, (void *)1);
	if (ret){
		printf("Error:unable to create thread %d\n", ret);
		return -1;
	}
	//Creating Thread 2 for Sensor Control Thread
	ret = pthread_create(&threads[2], NULL, CSC, (void *)2);
	if (ret){
		printf("Error:unable to create thread %d\n", ret);
		return -1;
	}
	pthread_exit(NULL);
}