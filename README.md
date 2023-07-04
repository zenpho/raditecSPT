# raditecSPT
The Radikal Technologies Spectralis is a "performance-oriented music instrument with multiple sequencer-sections [and] sampling engine [...] A pattern-based 17-track sequencer [for drums and] 32 track analog-style step-sequencer which not only plays notes, but can modulate most of the soundparameters of the Spectralis."

<img src="spectralis-audiofanzine.jpg" alt="front panel of the spectralis, from audiofanzine">

OS v1.04k provides two isolated sequencers with different feature sets. The Spectralis SPT data format is not published. This is an attempt to reverse engineer the format so that I can

- exchange data between the two sequencers to freely take advantage of probability, skip, mute, roll, motif swap, creativator features, etc for any musical sequence
- import and export data to SMF MIDI file format

A typical workflow might involve
- import a rhythm from an SMF MIDI file into SPT on drum parts
- load into the spectralis and edit using the step buttons
- move drum sequence data to the step-sequencer to apply mute/skip/probability
- record MIDI output from the spectralis to an external sequencer
- export a section as an SMF MIDI file
- import this rhythm into a second SPT on drum parts
- load into the spectralis and...

We're a little way off that goal right now.

<a href="SPT_types.h">SPT_types.h</a> defines some known data structures in the SPT format.


