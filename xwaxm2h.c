#include <linux/input.h>
#include<linux/uinput.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/seq_event.h>
#include <alsa/seq_midi_event.h>
#include "config.h"


/* Setup the uinput device Function */
 int setup_uinput_device() {
  int uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
  if (uinp_fd == 0) {
     printf("Unable to open /dev/uinput\n");
     return -1;
    }

  struct uinput_user_dev uinp;
  memset(&uinp, 0, sizeof(uinp)); // Intialize the uInput device to NULL
  strncpy(uinp.name, "Virtual Keyboard Device", strlen("Virtual Keyboard Device"));
  uinp.id.version = 4;
  uinp.id.bustype = BUS_USB;
  uinp.id.product = 1;
  uinp.id.vendor = 1;

// Setup the uinput device
  ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
  ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
  int i = 0;
  for (i = 0; i < 256; i++) {
      ioctl(uinp_fd, UI_SET_KEYBIT, i);
     }

// Create input device into input sub-system 
  write(uinp_fd, &uinp, sizeof(uinp));
  if (ioctl(uinp_fd, UI_DEV_CREATE)) {
     printf("Unable to create UINPUT device.");
     return -1;
    }
  return uinp_fd; 
}

/* actual key sending function */
 void send_key_event(int fd, unsigned int keycode, int keyvalue) {
  struct input_event event;
  gettimeofday(&event.time, NULL);

  event.type = EV_KEY;
  event.code = keycode;
  event.value = keyvalue;

  if (write(fd, &event, sizeof(event)) < 0) {
     printf("simulate key error\n");
    } 

  event.type = EV_SYN;
  event.code = SYN_REPORT;
  event.value = 0;
  write(fd, &event, sizeof(event));

  if (write(fd, &event, sizeof(event)) < 0) {
     printf("simulate key error\n");
    } 
}

/* Main program */
int main(int argc, char *argv[]) {
   
    FILE* midi;
    unsigned int notenumber[128];
    unsigned int key_code[128];
    int temp, temp_v, temp_ch, i;
	int FL = 0;
	int BYPASS = 0;
    snd_midi_event_t* parser;
    snd_seq_event_t event;

/*usage*/
 if (argc < 2) {
    fprintf(stderr,"Usage: %s device \n",*argv);
    return 1;
   }


/* initialize array*/
 for ( i = 0; i < 128; i++) {
     key_code[i] = 0;
     notenumber[i] = 0;
    }

/*midinote to keycode array assign*/
    key_code[up] = 103;
    notenumber[up] = up;
    
    key_code[down] = 108;
    notenumber[down] = down;
	
    key_code[page_up] = 104;
    notenumber[page_up] = page_up;
    
    key_code[page_down] = 109;
    notenumber[page_down] = page_down;
  
    key_code[left] = 105;
    notenumber[left] = left;
 
    key_code[right] = 106;
    notenumber[right] = right;

    key_code[f1] = 59;
    notenumber[f1] = f1;
  
    key_code[f2] = 60;
    notenumber[f2] = f2;

    int key_bypass = 61;
    int key_ctrl = 29;
    int key_shift = 42;
    int key_tab = 15;
	   
    //notenumber[f3] = f3;
   
 //   key_code[control] = 29;
 //   notenumber[control] = control;

    //key_code[toggle] = 42;
    //notenumber[toggle] = toggle;

//    key_code[tab] = 15;
//    notenumber[tab] = tab; 
 
/* initialize virtual keyboard*/
 int uinp_fd;
 if ((uinp_fd = setup_uinput_device()) < 0) {
     printf("Unable to find uinput device\n");
     return -1;
    }
sleep(1);

/*midi*/
if (!(midi = fopen(argv[1],"rb"))) {
   fprintf(stderr,"Unable to open MIDI device '%s'\n",argv[1]);
   return 3;
  }
    
 snd_midi_event_new(32,&parser); /*feed implied note on message*/
 snd_midi_event_encode_byte(parser,0x9A,&event);

/*main loop*/
fputs("Ready for midi input\n",stderr);
while ((temp = fgetc(midi)) != EOF) { 
	if (snd_midi_event_encode_byte(parser, temp, &event) == 1) {
        if (event.type == SND_SEQ_EVENT_NOTEON) {
			if ((event.data.note.note <= 127) && (event.data.note.velocity)) {
				temp = event.data.note.note;
				if (temp == notenumber[event.data.note.note]) {
					send_key_event(uinp_fd, key_code[temp], 1);
					send_key_event(uinp_fd, key_code[temp], 0);
					printf("Note: %i -> KeyCode: %i\n",event.data.note.note, (int)key_code[event.data.note.note]);
				} else if (event.data.note.note == 64){
					if (FL == 0) {
						FL = 1;
						printf("Note: %i FOLDER Scroll ON\n", event.data.note.note);
					} else {
						FL = 0;
						printf("Note: %i FOLDER Scroll OFF\n", event.data.note.note);
					}
				} else if (event.data.note.note == 29){
					if (BYPASS==0) {
						system("amixer sset 'Output Mixer HiFi' off");	
						system("amixer cset numid=4,iface=MIXER,name='Line Capture Switch' off"); 
						system("amixer sset 'Output Mixer Line Bypass' on");
						system("sleep 0.1s");
						send_key_event(uinp_fd, key_bypass, 1);
						send_key_event(uinp_fd, key_bypass, 0);
						BYPASS = 1;
						printf("BYPASS ON\n");
					}
					else if (BYPASS==1) {
						system("amixer sset 'Output Mixer Line Bypass' off");
						system("amixer cset numid=4,iface=MIXER,name='Line Capture Switch' on"); 
						system("amixer sset 'Output Mixer HiFi' on");
						send_key_event(uinp_fd, key_bypass, 1);
						send_key_event(uinp_fd, key_bypass, 0);
						BYPASS = 0;
						printf("BYPASS OFF\n");
					}
				} else if (event.data.note.note == 30){
						send_key_event(uinp_fd, key_ctrl, 1);
						send_key_event(uinp_fd, key_shift, 1);
						send_key_event(uinp_fd, key_tab, 1);
						
						send_key_event(uinp_fd, key_ctrl, 0);
						send_key_event(uinp_fd, key_shift, 0);
						send_key_event(uinp_fd, key_tab, 0);
						printf("RESCAN\n");
				}
			} 
		} else {
			if (event.type == SND_SEQ_EVENT_CONTROLLER) {
				//printf("Channel: %d\n", event.data.control.channel);
				//printf("Param: %d\n", event.data.control.param);
				//printf("Value: %d\n", event.data.control.value);
				printf("Channel: %i, Control: %i -> KeyCode: %i\n", event.data.control.channel, event.data.control.value, (int)key_code[event.data.control.value]);
				temp_v = event.data.control.value;
				temp_ch = event.data.control.channel;
				
				
				if (FL == 1) {
					temp_v = temp_v + 20;
					// folder scroll
				}
				else if ((FL == 0) && (temp_ch == 1)) {
					// scroll library
				}
				else if ((FL == 0) && (temp_ch == 0)) {
					temp_v = temp_v + 10;
					// page scroll library
				}
					
				
				if (temp_v == notenumber[temp_v]) {
					send_key_event(uinp_fd, key_code[temp_v], 1);
					send_key_event(uinp_fd, key_code[temp_v], 0);                    
				}
				printf("\nControl: %i -> KeyCode: %i\n",temp_v, (int)key_code[temp_v]);
				
			}
		}
	}
}



/* Destroy the input device */
 ioctl(uinp_fd, UI_DEV_DESTROY);
/* Close the UINPUT device */
 close(uinp_fd);
/*close midi */
 snd_midi_event_free(parser);
 fclose(midi);
/*exit*/
 return 0;
} 
