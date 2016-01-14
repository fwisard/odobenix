/* 
Copyright (C) 2011  Fabian Kurz

$Id: pulseaudio.h 203 2011-03-21 22:55:21Z dj1yfk $

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

#ifndef QRQ_PA
#define QRQ_PA

void *open_dsp (); 
void write_audio (void *bla, int *in, size_t size);
void close_audio (void *s);

#endif
