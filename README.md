#Odobenix in a nutshell
Utilities and games to improve morse skills. Written in C and Python, aimed at linux console users who want to raise their CW speed or head copy. GPL.

#Programs
This suite consists of the following software:

##cw
An audio morse generator, for PulseAudio or OSS. You need to compile it first (just type 'make').  

    Text to audio morse utility by F. Wisard, HB9FXW, based on qrq v0.3.1  (c) 2006-2013 Fabian Kurz, DJ1YFK.

    This is free software, and you are welcome to redistribute it under certain conditions (see COPYING).

    Usage: cw TEXT [wpm] [freq]
    with wpm as the word per minute speed and freq as the audio frequency.

##kochwords  
Common CW words trainer. Enter the words you hear, as fast as possible. If you're quick enough and thus prove your skill, the level will raise and there will be more words.

The scoring system is adjusted for an average touch typer. 'Hunt & peck' typers or professional touch typers may want to tweak the appropriate variable in the source code.


    usage: kochwords [-h] [-d] [-f F] [-s S] [-l L] [-p]

    Train recognition of common CW words.

    optional arguments:
      -h, --help  show this help message and exit
      -d          debug mode
      -f F        frequency (300-1400) (default: 400)
      -s S        speed in wpm (5-100) (default: 17)
      -l L        starting level (2-116) (default: 2)
      -p          print the words of this level and then quits

##morsechef   
A game to improve your morse copying skills. Choose 4 appropriate ingredients among those proposed.

Recognized commands: qrq, qrs, rpt, quit, help.


    usage: morsechef [-h] [-d] [-f F] [-s S]

    A game to improve your morse receiving skills.

    optional arguments:
      -h, --help  show this help message and exit
      -d          debug mode
      -f F        frequency (300-1400) (default: 400)
      -s S        speed in wpm (5-100) (default: 15)


More to come...

73 de hb9fxw.
