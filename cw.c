/* 
cw - Text to audio morse utility, by Francois Wisard HB9FXW. Heavily based on 
qrq, the High speed morse trainer, by Fabian Kurz DJ1YFK.
Actually, most of the code is Fabian's; I mainly deleted stuff.

Copyright (C) 2016 Francois Wisard
Copyright (C) 2006-2013  Fabian Kurz

//$Id: qrq.c 564 2013-01-06 13:25:46Z dj1yfk $

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>				/* basename */
#include <ctype.h>
#include <time.h> 
#include <limits.h> 			/* PATH_MAX */
#include <dirent.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define PI M_PI

#define SILENCE 0		/* Waveforms for the tone generator */
#define SINE 1
#define SAWTOOTH 2
#define SQUARE 3


#ifndef VERSION
#  define VERSION "0.0.0"
#endif

#ifdef OSS
#include "oss.h"
#define write_audio(x, y, z) write(x, y, z)
#define close_audio(x) close(x)
typedef int AUDIO_HANDLE;
#endif

#ifdef PA
#include "pulseaudio.h"
typedef void *AUDIO_HANDLE;
#endif


const static char *codetable[] = {
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",".---",
"-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",
".--","-..-","-.--","--..","-----",".----","..---","...--","....-",".....",
"-....", "--...","---..","----."};


static char dspdevice[PATH_MAX]="/dev/dsp";	/* will also be read from qrqrc */
static int mincharspeed=10;				/* min. char. speed, below: farnsworth*/
static int speed=70;					/* current speed in cpm */
static int freq=350;					/* current cw sidetone freq */

long samplerate=44100;
static long long_i;
static int waveform = SINE;				/* waveform: (0 = none) */
static double edge=2.0;						/* rise/fall time in milliseconds */
static int ed;							/* risetime, normalized to samplerate */

static short buffer[882000];
static int full_buf[8820000];  /* 20 second max buffer */
static int full_bufpos = 0;

AUDIO_HANDLE dsp_fd;

static int tonegen(int freq, int length, int waveform);
static void *morse(void * arg); 
static int add_to_buf(void* data, int size);
static void help ();


int main (int argc, char *argv[]) {
	if (argc < 2) {
		help();
	}
    char text[255];
        strcpy(text, argv[1]);
    for(int i=0;text[i]!='\0';i++) {
        text[i]=toupper(text[i]);
    }


    if (argc > 2) {
        speed=atoi( argv[2]);
        speed*=5; //wpm to lpm
    }
    if (argc > 3) {
        freq=atoi(argv[3]);
    }
	
	/* buffer for audio */
	for (long_i=0;long_i<8820000;long_i++) {
		buffer[long_i]=0;
	}
	
	/* random seed from time */
	srand( (unsigned) time(NULL) ); 

    morse(text);


	return 0;
}

static void *morse(void *arg) { 
	char * text = arg;
	int i,j;
	int c, fulldotlen, dotlen, dashlen, charspeed, farnsworth, fwdotlen;
	const char *code;

#if WIN32 /* WinMM simple support by Lukasz Komsta, SP8QED */
	HWAVEOUT		h;
	WAVEFORMATEX	wf;
	WAVEHDR			wh;
	HANDLE			d;

	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 1;
	wf.wBitsPerSample = 16;
	wf.nSamplesPerSec = samplerate * 2;
	wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	wf.cbSize = 0;
	d = CreateEvent(0, FALSE, FALSE, 0);
	if(waveOutOpen(&h, 0, &wf, (DWORD) d, 0, CALLBACK_EVENT) != MMSYSERR_NOERROR);

#else
	/* opening the DSP device */
	dsp_fd = open_dsp(dspdevice);
#endif
	/* set bufpos to 0 */

	full_bufpos = 0; 

	/* Some silence; otherwise the call starts right after pressing enter */
	tonegen(0, samplerate/4, SILENCE);

	/* Farnsworth? */
	if (speed < mincharspeed) {
			charspeed = mincharspeed;
			farnsworth = 1;
			fwdotlen = (int) (samplerate * 6/speed);
	}
	else {
		charspeed = speed;
		farnsworth = 0;
	}

	/* speed is in LpM now, so we have to calculate the dot-length in
	 * milliseconds using the well-known formula  dotlength= 60/(wpm*50) 
	 * and then to samples */

	dotlen = (int) (samplerate * 6/charspeed);
	fulldotlen = dotlen;
	dashlen = 3*dotlen;

	/* edge = length of rise/fall time in ms. ed = in samples */

	ed = (int) (samplerate * (edge/1000.0));

	/* the signal needs "ed" samples to reach the full amplitude and
	 * at the end another "ed" samples to reach zero. The dots and
	 * dashes therefore are becoming longer by "ed" and the pauses
	 * after them are shortened accordingly by "ed" samples */

	for (i = 0; i < strlen(text); i++) {
		c = text[i];
		if (isalpha(c)) {
			code = codetable[c-65];
		}
		else if (isdigit(c)) {
			code = codetable[c-22];
		}
		else if (c == '/') { 
			code = "-..-.";
		}
		else if (c == '+') {
			code = ".-.-.";
		}
        else if (c == ' ') {
            code = "/";
        }
        else if (c == '=') {
            code = "-...-";
        }
		else {						/* not supposed to happen! */
			code = "..--..";
		}
		
		/* code is now available as string with - and . */

		for (j = 0; j < strlen(code) ; j++) {
			c = code[j];
			if (c == '.') {
				tonegen(freq, dotlen + ed, waveform);
				tonegen(0, fulldotlen - ed, SILENCE);
			}
            else if (c == '/') {
                tonegen(0, dotlen , SILENCE);
            }
			else {
				tonegen(freq, dashlen + ed, waveform);
				tonegen(0, fulldotlen - ed, SILENCE);
			}
		}
		if (farnsworth) {
			tonegen(0, 3*fwdotlen - fulldotlen, SILENCE);
		}
		else {
			tonegen(0, 2*fulldotlen, SILENCE);
		}
	}


#if !defined(PA) && !defined(CA)
	add_to_buf(buffer, 882000);
#endif

#if WIN32
	wh.lpData = (char*) &full_buf[0];
	wh.dwBufferLength = full_bufpos - 2;
	wh.dwFlags = 0;
	wh.dwLoops = 0;
	waveOutPrepareHeader(h, &wh, sizeof(wh));
	ResetEvent(d);
	waveOutWrite(h, &wh, sizeof(wh));
	if(WaitForSingleObject(d, INFINITE) != WAIT_OBJECT_0);
	waveOutUnprepareHeader(h, &wh, sizeof(wh));
	waveOutClose(h);
	CloseHandle(d);
#else
	write_audio(dsp_fd, &full_buf[0], full_bufpos);
	close_audio(dsp_fd);
#endif
	/* sending_complete = 1; */
	return NULL;
}

static int add_to_buf(void* data, int size)
{
	memcpy(&full_buf[full_bufpos / sizeof(int)], data, size);
	full_bufpos += size;
	return 0;
}	

/* tonegen generates a sinus tone of frequency 'freq' and length 'len' (samples)
 * based on 'samplerate', 'edge' (rise/falltime) */

static int tonegen (int freq, int len, int waveform) {
	int x=0;
	int out;
	double val=0;

	for (x=0; x < len-1; x++) {
		switch (waveform) {
			case SINE:
				val = sin(2*PI*freq*x/samplerate);
				break;
			case SAWTOOTH:
				val=((1.0*freq*x/samplerate)-floor(1.0*freq*x/samplerate))-0.5;
				break;
			case SQUARE:
				val = ceil(sin(2*PI*freq*x/samplerate))-0.5;
				break;
			case SILENCE:
				val = 0;
		}


		if (x < ed) { val *= pow(sin(PI*x/(2.0*ed)),2); }	/* rising edge */

		if (x > (len-ed)) {								/* falling edge */
				val *= pow(sin(2*PI*(x-(len-ed)+ed)/(4*ed)),2); 
		}
		
		out = (int) (val * 32500.0);
#ifndef PA
		out = out + (out<<16);	/* stereo only for OSS & CoreAudio*/
#endif
		add_to_buf(&out, sizeof(out));
	}
	return 0;
}


void help () {
		printf("Text to audio morse utility by F. Wisard, HB9FXW, based on\n"
                "qrq v0.3.1  (c) 2006-2013 Fabian Kurz, DJ1YFK.\n\n" );
		printf("This is free software, and you are welcome to" 
						" redistribute it\n");
		printf("under certain conditions (see COPYING).\n\n");
		printf("Usage: cw TEXT [wpm] [freq]\n"
                "with wpm as the word per minute speed and freq as the audio frequency.\n");
            
		exit(0);
}


/* vim: noai:ts=4:sw=4 
*/
