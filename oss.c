/* 
Copyright (C) 2006-2007  Fabian Kurz
 
$Id: oss.c 40 2008-06-22 14:08:40Z dj1yfk $

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

OSS specific functions and includes.

*/

#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <ncurses.h>
#include <stdlib.h>
#include <fcntl.h>

extern long samplerate;

int open_dsp (char * device) {
	int tmp;
	int fd;
	
	if ((fd = open(device, O_WRONLY, 0)) == -1) {
		endwin();
		perror(device);
		exit(EXIT_FAILURE);
	}

	tmp = AFMT_S16_NE; 
	if (ioctl(fd, SNDCTL_DSP_SETFMT, &tmp)==-1) {
		endwin();
		perror("SNDCTL_DSP_SETFMT");
		exit(EXIT_FAILURE);
	}

	if (tmp != AFMT_S16_NE) {
		endwin();
		fprintf(stderr, "Cannot switch to AFMT_S16_NE\n");
		exit(EXIT_FAILURE);
	}
  
	tmp = 2;	/* 2 channels, stereo */
	if (ioctl(fd, SNDCTL_DSP_CHANNELS, &tmp)==-1) {
		endwin();
		perror("SNDCTL_DSP_CHANNELS");
		exit(EXIT_FAILURE);
	}

	if (tmp != 2) {
		endwin();
		fprintf(stderr, "No stereo mode possible :(.\n");
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd, SNDCTL_DSP_SPEED, &samplerate)==-1) {
		endwin();
		perror("SNDCTL_DSP_SPEED");
		exit(EXIT_FAILURE);
	}
return fd;
}

