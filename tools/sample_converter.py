#!/usr/bin/env python3
"""
B.I.T.E.S Audio Sample Converter
Converts WAV files to embedded C++ format for Teensy Audio library
"""

import wave
import struct
import sys
import os

def convert_wav_to_cpp(wav_path, output_path=None):
    """Convert WAV file to C++ header file for Teensy Audio library"""
    
    if not os.path.exists(wav_path):
        print(f"Error: File not found: {wav_path}")
        return False
    
    # Read WAV file
    with wave.open(wav_path, 'rb') as wav_file:
        frames = wav_file.getnframes()
        sample_rate = wav_file.getframerate()
        channels = wav_file.getnchannels()
        sample_width = wav_file.getsampwidth()
        
        # Read audio data
        audio_data = wav_file.readframes(frames)
    
    # Convert to 16-bit signed integers
    if sample_width == 1:
        # 8-bit unsigned -> 16-bit signed
        samples = struct.unpack(f'{frames * channels}B', audio_data)
        samples = [(s - 128) * 256 for s in samples]
    elif sample_width == 2:
        # 16-bit signed
        samples = struct.unpack(f'<{frames * channels}h', audio_data)
    else:
        print(f"Error: Unsupported sample width: {sample_width}")
        return False
    
    # Generate output filename
    if output_path is None:
        base_name = os.path.splitext(os.path.basename(wav_path))[0]
        output_path = f"AudioSample{base_name}.h"
    
    # Generate C++ header file
    array_name = f"AudioSample{os.path.splitext(os.path.basename(wav_path))[0]}"
    
    with open(output_path, 'w') as f:
        f.write(f"#ifndef {array_name.upper()}_H\n")
        f.write(f"#define {array_name.upper()}_H\n\n")
        f.write("#include <Arduino.h>\n\n")
        f.write(f"// Audio sample: {os.path.basename(wav_path)}\n")
        f.write(f"// Sample rate: {sample_rate} Hz\n")
        f.write(f"// Channels: {channels}\n")
        f.write(f"// Length: {frames} samples\n\n")
        f.write(f"PROGMEM const int16_t {array_name}[] = {{\n")
        
        # Write samples (limit to reasonable size)
        max_samples = min(len(samples), 100000)  # Limit to 100k samples
        for i in range(0, max_samples, 8):
            line_samples = samples[i:i+8]
            line = ", ".join(str(s) for s in line_samples)
            if i + 8 < max_samples:
                f.write(f"  {line},\n")
            else:
                f.write(f"  {line}\n")
        
        f.write("};\n\n")
        f.write(f"#endif // {array_name.upper()}_H\n")
    
    print(f"Converted {wav_path} -> {output_path}")
    print(f"  Samples: {len(samples)}")
    print(f"  Sample rate: {sample_rate} Hz")
    print(f"  Channels: {channels}")
    
    return True

def main():
    if len(sys.argv) < 2:
        print("Usage: sample_converter.py <wav_file> [output_file]")
        sys.exit(1)
    
    wav_path = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else None
    
    convert_wav_to_cpp(wav_path, output_path)

if __name__ == "__main__":
    main()
