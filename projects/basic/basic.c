#include "zoho_iot_client.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#if defined(Z_SECURE_CONNECTION)
#if defined(EMBED_MODE)
#include "zclient_certificates.h"
#include </usr/include/zmq.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#endif
#endif



int random1(int range){
    int num;
     num = rand()%range;
    return num;
}



volatile int ctrl_flag = 0;

// Zoho client struct to handle the Communication with the HUB
ZohoIOTclient client;
//zclient_setMaxPayloadSize(&client,2);
#define MQTT_USER_NAME "/23762rteoy.zohoiothub.com/v1/devices/628000000187550/connect"
#define MQTT_PASSWORD "2945b99f4e3c3e1b3aedd2b6092b448c9b2034f6fe5f9a59469189c88cd76"

// Intervals in which the telemetry data needs to be dispatched to the hub
#define DISPATCH_INTERVAL 30 // Dispatch data every 30 sec
// Frequency in which the data needs to be fetched
#define POLL_FREQUENCY 5 // poll data every 5 sec

void message_command_handler(MessageData *data)
{
    char a[10]="abc";
    char payload[data->message->payloadlen];
    char topic[data->topicName->lenstring.len];
    *topic = '\0';
    *payload = '\0';
    strncpy(topic, data->topicName->lenstring.data, data->topicName->lenstring.len);
    strncpy(payload, data->message->payload, data->message->payloadlen);
    log_debug("\n\n Got new command message on '%s'\n%s \n\n", topic, payload);
    log_debug("Second level Command Ack status : %d", zclient_publishCommandAck(&client,payload, SUCCESFULLY_EXECUTED, "Command based task Executed."));
}

void interruptHandler(int signo)
{
    log_info("Closing connection...");
    ctrl_flag += 1;
    if (ctrl_flag >= 2)
    {
        log_fatal("Program is force killed");
        exit(0);
    }
}

//Function gets the current time in seconds
unsigned long long getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long seconds = (unsigned long long)(tv.tv_sec);
    return seconds;
}

int main()
{
    srand(time(NULL));
    int rc = -1;
    unsigned long long poll_time = 0;
    signal(SIGINT, interruptHandler);
    signal(SIGTERM, interruptHandler);
    int humidity=0,pressure =0;

    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, "tcp://localhost:5555");
    //if (rc != 0) {
    //    printf("Error connecting subscriber socket: %s\n", zmq_strerror(errno));
    //    return -1;
    //}

    // Subscribe to all messages
    //zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);






    char abc[6]="sample";
    //char *payload="sample";
    typedef struct bms{
	int rem;
	int soc;
	int err;
	int cur;
	int vol;
	int ran;
	}bms1;

	bms1 bms = {98, 1,1,88,878,150};


   typedef struct mcu{
	int err;
	int mtemp;
	int ctemp;
	int cur;
	int vol;
	int dis;
	int ms;
	int spd;
	}mcu1;
	

	mcu1 mcu;
	//mcu.err =random1(500); 
	//mcu1 mcu={1,80,78,77,54,23,22,25};
//     error
// motor temp
// controller temp
// current
// voltage
// distance
// speed
// max speed


 //  typedef struct pdu{

//	char dc[3];
//	char cc[3];
//	char kf[3];
//	char fs[3];
//	char ks[3];
//	int err;
//	}pdu1;

//	pdu1 pdu;



//	strncpy(pdu.dc, "off", sizeof(pdu.dc));
//	strncpy(pdu.cc, "off", sizeof(pdu.cc));
//	strncpy(pdu.kf, "off", sizeof(pdu.kf));
//	strncpy(pdu.fs, "off", sizeof(pdu.fs));
//	strncpy(pdu.ks, "off", sizeof(pdu.ks));

//	pdu1 pdu;
//	pdu.dc[3] = "off";
//	pdu.cc[3] = "off";
//	pdu.kf[3] = "off";
//	pdu.fs[3] = "off";
//	pdu.ks[3] = "off";
//	pdu.err =1;


typedef struct pdu {
    char *dc;
    char *cc;
    char *kf;
    char *fs;
    char *ks;
    int err;
} pdu1;

pdu1 pdu;
pdu.dc = malloc(4); 
strcpy(pdu.dc, "off");
pdu.cc = malloc(4);
strcpy(pdu.cc,"off");
pdu.kf = malloc(4);
strcpy(pdu.kf,"off");
pdu.fs = malloc(4);
strcpy(pdu.fs,"off");
pdu.ks = malloc(4);
strcpy(pdu.ks,"off");


// discharge contactor (on/off)
// charge contactor (on/off)
// keyfob (on/off)
// footswitch (on/off)
// killsiwtch (on/off)
// Error







  typedef struct trip{
	int range;
	int tdist;
	int en;
	int ef;
	}trip1;

	trip1 trip = {78,189,78,89};




    char *pRootCACert = "", *pDeviceCert = "", *pDevicePrivateKey = "", *pDeviceCertParsword = "";

#if defined(Z_SECURE_CONNECTION)
    pRootCACert = CA_CRT;
#if defined(Z_USE_CLIENT_CERTS)
    pDeviceCert = CLIENT_CRT;
    pDevicePrivateKey = CLIENT_KEY;
#endif
#endif
    //
    ZlogConfig *logConfig = getZlogger();
    logConfig->enableFileLog = 1;
    logConfig->logPrefix = "basic";
    logConfig->logPath = "./";
    logConfig->maxLogFileSize = 100000; // File size in bytes
    logConfig->maxRollingLogFile = 2;

    //Update your DEVICE_ID AND AUTH_TOKEN below:
    // IF the application is used in non-TLS mode then replace all the following variables pRootCACert, pDeviceCert, pDevicePrivateKey, pDeviceCertParsword as emtpy string ;
    rc = zclient_init(&client, MQTT_USER_NAME, MQTT_PASSWORD, CRT_PARSE_MODE, pRootCACert, pDeviceCert, pDevicePrivateKey, pDeviceCertParsword, logConfig);


    /*
    * If you want to configure the SDK logger use the ZlogConfig object and 
    * set the required configuration and pass it to the zclient_init function as follows 
    * 
    * ZlogConfig *logConfig = getZlogger();
    * logConfig->enableFileLog = 1;
    * logConfig->setQuiet = 1;
    * logConfig ->logPath = "./"
    * logConfig->logPrefix = "logFile_name";
    * logConfig->maxLogFileSize = 100000;
    * logConfig->maxRollingLogFile = 1;
    * 
    * zclient_init(&client, "/domain_name/v1/devices/client_id/connect", "mqtt_password", CRT_PARSE_MODE, pRootCACert, pDeviceCert, pDevicePrivateKey, pDeviceCertParsword,logConfig);
    *                                                                           (or)
    * 
    * enableFileLog     -> enables the file logging for the application
    * setQuiet          -> disables the console print 
    * logPath           -> location where the log to be created
    * logPrefix         -> log file name
    * maxLogFileSize    -> size of the log file in bytes
    * maxRollingLogFile -> No of rolling log file to be created in addition to the main log file
    * 
    * The SDK default log configuration would be used when the Z_ENABLE_LOGGING option is set to ON and the init call is made without the log config as
zclient    *  
    * zclient_init(&client, "/domain_name/v1/devices/client_id/connect", "mqtt_password", CRT_PARSE_MODE, pRootCACert, pDeviceCert, pDevicePrivateKey, pDeviceCertParsword);
    * 
    * The SDK logging default configuration is as follows 
    * Path : "./"
    * File name : "zoho_SDK_logs"
    * Max log file size : 100 Kb
    * No of rolling files : 2
    * 
    */

    if (rc != ZSUCCESS)
    {
        return 0;
    }
    rc = zclient_connect(&client);
    while (rc != ZSUCCESS && ctrl_flag == 0)
    {
        //Endless reconnection on start. No data collection happens during connection error.
        rc = zclient_reconnect(&client);
    }

    rc = zclient_command_subscribe(&client, message_command_handler);

    while (ctrl_flag == 0)
    {

	
    mcu1 mcu = {
        .err = random1(100),
        .mtemp = random1(100),
        .ctemp = random1(100),
        .cur = random1(100),
        .vol = random1(1000),
        .dis = random1(1000),
        .ms = random1(100),
        .spd = random1(50)
    };



   
    trip1 trip = {
        .range = random1(100), 
        .tdist = random1(500), 
        .en = random1(150),    
        .ef = random1(100)    


    };

	

	
    bms1 bms = {
        .rem = random1(101), // Random value between 0 and 100
        .soc = random1(101), // Random value between 0 and 100
        .err = random1(2),   // Random value 0 or 1
        .cur = random1(101), // Random value between 0 and 100
        .vol = random1(1001), // Random value between 0 and 1000
        .ran = random1(501)  // Random value between 0 and 500
    };






        // If Connection is already available, then retryring to connect inside function is ignored
        rc = zclient_reconnect(&client);
        if (getTime() - poll_time > POLL_FREQUENCY) //check if it is time for polling the data from the sensor
        {
            log_debug("Polling datapoints");
            //temperature += 2;
            //humidity *= 2;
            //pressure -= 5;
            if (client.current_state == CONNECTED)
            {
	//	rc=zclient_setMaxPayloadSize(&client,2);
                //rc = zclient_addNumber(&client, "remaining", bms.rem);
                rc = zclient_addNumber(&client, "remaining capacity", bms.rem, "BMS");
		rc = zclient_addNumber(&client,"error",bms.err,"BMS");
		rc = zclient_addNumber(&client,"current",bms.cur,"BMS");
		rc = zclient_addNumber(&client,"state of charge",bms.soc,"BMS");
		rc = zclient_addNumber(&client,"volt",bms.vol,"BMS");
		rc = zclient_addNumber(&client,"range",bms.ran,"BMS");
        
        rc = zclient_addNumber(&client,"error",mcu.err,"MCU");
		rc = zclient_addNumber(&client,"motor temperature",mcu.mtemp,"MCU");
		rc = zclient_addNumber(&client,"controller temperature",mcu.ctemp,"MCU");
		rc = zclient_addNumber(&client,"current",mcu.cur,"MCU");
		rc = zclient_addNumber(&client,"voltage",mcu.vol,"MCU");
        rc = zclient_addNumber(&client,"distance",mcu.dis,"MCU");
		rc = zclient_addNumber(&client,"maximum speed",mcu.ms,"MCU");
		rc = zclient_addNumber(&client,"speed",mcu.spd,"MCU");

        rc = zclient_addString(&client, "Discharge contactor", pdu.cc,"PDU");
        rc = zclient_addString(&client,"Charge contactor",pdu.dc,"PDU");
        rc = zclient_addString(&client, "keyfob", pdu.kf,"PDU");
        rc = zclient_addString(&client,"Kill Switch",pdu.ks,"PDU");     
        rc = zclient_addNumber(&client, "Error", pdu.err,"PDU"); 



        rc = zclient_addNumber(&client,"Range",trip.range,"Trip Details");
        rc = zclient_addNumber(&client,"Trip Distance",trip.tdist,"Trip Details");
        rc = zclient_addNumber(&client,"Energy Consumption",trip.en,"Trip Details");
        rc = zclient_addNumber(&client,"Efficiency",trip.ef,"Trip Details");
      


		//rc = zcleint_addNumbe(&client,"error",bms.
                rc = zclient_addNumber(&client, "pressure", pressure, "MCU");
		//rc = zclient_addNumber(&client,
               // rc = zclient_markDataPointAsError(&client, "mositure", "room2");
               // rc = zclient_markDataPointAsError(&client, "air_quality");
                rc = zclient_addString(&client, "status", abc,"PDU");
		//rc = zclient_dispatch(&client);
               //payload = zclient_getpayload();
               // rc = zclient_publish(&client, payload);   

                if(rc = zclient_dispatch(&client)){
			printf("%d\n",rc);
			}

            }
            poll_time = getTime();
        }
        if (client.current_state == CONNECTED)
        {
            // Yielding only on available connection
            rc = zclient_yield(&client, 300);
        }
    }

    zclient_disconnect(&client);
    return 0;
}
