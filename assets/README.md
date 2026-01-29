# Audio Assets

This directory contains audio samples for B.I.T.E.S instruments.

## Directory Structure

- `bass/` - Bass guitar samples
- `guitar/` - Guitar samples  
- `keyboard/` - Keyboard samples
- `drums/` - Drum samples

## Sample Format

- **Format:** WAV
- **Sample Rate:** 44.1 kHz
- **Bit Depth:** 16-bit
- **Channels:** Mono or Stereo

## Converting Samples

Use the `tools/sample_converter.py` script to convert WAV files to embedded C++ format:

```bash
python tools/sample_converter.py assets/bass/E0.wav src/audio/samples/AudioSampleE0.h
```

## Sample Naming Convention

- Bass: `E0.wav`, `A0.wav`, `D1.wav`, `G1.wav`
- Guitar: `E2.wav`, `A2.wav`, `D3.wav`, `G3.wav`, `B3.wav`, `E4.wav`
- Keyboard: `C4.wav`, `D4.wav`, `E4.wav`, etc.
- Drums: `kick.wav`, `snare.wav`, `hihat.wav`, etc.
