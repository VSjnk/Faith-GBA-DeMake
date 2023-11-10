BoyScout Distribution v.1.0
------------------------------

Read the provided manual for "Terms of Usage".

Check:

Manual folder - It contains a very thourough walkthrough of the program.
Demo folder - Contains some good BoyScout demo songs.
Library folder - Files for BoyScout song playback on the GBA.

Changes v.1.0:

After a long discontinuity in BoyScout's development, I finally recieved some help
to fix the still flawed channel 3 GBA playback routines. Thanks to Dave Murphy (WinterMute)
I'm feeling pretty confident in releasing a 1.0 version of BoyScout. This version includes
a modified tracker EXE which aligns the channel 3 waveforms properly in its .BGF file output.
That together with a change in the order in which the bit flags for controlling channel 3
are set, have brought BoyScout to a final version 1.0 milestone.

Changes v.0.94-2:

GBA Channel 3 playback had some severe problems in past versions of BoyScout.
Several issues have been fixed and on all test songs I've tried, channel 3 now sounds OK.
This distro is probably the last one I will do, I'm home in Sweden from my work in the UK, and because
of getting mails about channel 3 being messed up, I dedicated a bit of time to it now and hopefully
it will be all good now. Thanks alot everyone for a great GBA demo scene, and keep on tracking!

- Fixed several GBA channel 3 playback bugs in library.
- Compiled binaries of playback of the supplied demo songs.
- The tracker hasn't changed, only the playback libraries, this is ditro 2 of BoyScout v.0.94.

Changes v.0.94:

- Fixed cpu % chewing - Runs at 3% cpu on PIII 866mhz.
- Made some minor changes to sound channel frequency calculations.
- Playback libraries no longer uses DMA.

Changes v.0.93:

- Plays even when not in focus.
- Render to wave files (.wav).
- Output of formatted C++ data files (.cpp).
- Transposing of notes (Ctrl-T).
- Copying of notes between channels.
- Preset update button (no resaving).
- Better default settings.
- A new commercial usage clause (see manual).


Changes v.0.92:

- Small bug in compression algoritm for GameBoy format fixed,
it felt important to fix it as soon as possible even though it
was a rare case scenario bug.


Changes v.0.91:

- Preset file format.
- Added sound on/off buttons to the sequencer channels.
- Editable Sound 3 wave form presets.
- Some UI fixes.


Christer Andersson - 6th of April 2005