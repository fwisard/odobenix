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


    usage: kochwords [-h] [-d] [-f FREQ] [-s SPEED] [-l LEVEL] [-p]

    Train recognition of common CW words.

    optional arguments:
      -h, --help  show this help message and exit
      -d          debug mode
      -f FREQ     frequency (300-1400) (default: 400)
      -s SPEED    speed in wpm (5-100) (default: 17)
      -l LEVEL    starting level (2-116) (default: 2)
      -p          print the words of this level and then quits
      -r          recent words have more weight
      -n          no word twice in a row

Default values may be changed by editing the configuration section at the top of the file.

##morsechef   
A game to improve your morse copying skills. Choose 4 appropriate ingredients among those proposed.

Recognized commands: qrq, qrs, rpt, quit, help.


    usage: morsechef [-h] [-d] [-f FREQ] [-s SPEED]

    A game to improve your morse receiving skills.

    optional arguments:
      -h, --help  show this help message and exit
      -d          debug mode
      -f FREQ     frequency (300-1400) (default: 400)
      -s SPEED    speed in wpm (5-100) (default: 15)

Default values may be changed by editing the configuration section at the top of the file.

##twila
Twila stands for "That's What I'd Log/Answer", and aims to train your head copying skills, via simulation of parts of a typical QSO. Type 'q' to quit.

    usage: twila [-h] [-d] [-f FREQ] [-x] [-a] [-s SPEED] [-c CAT]

    Trains your CW skills by giving you short sentences and waiting for an answer
    or a log entry with the most important info. Examples: - It says "name hr is
    bob", you should enter "bob" - It asks "pse rpt age", you should answer with
    your age. - It says "ant is mag loop on the roof", you should enter "mag
    loop".

    optional arguments:
      -h, --help  show this help message and exit
      -d          debug mode
      -f FREQ     frequency (300-1400) (default: 400)
      -x          enable strict mode
      -a          ask questions too
      -s SPEED    speed in wpm (5-100) (default: 18)
      -c CAT      focus on only one category (rst,name)

## ...
More to come...  

73 de hb9fxw.
