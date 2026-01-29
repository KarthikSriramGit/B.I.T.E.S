# AI Features Documentation

## Gesture Recognition

Recognizes strumming, picking, and striking patterns from MPU6050 data.

**Usage:**
```cpp
AIEngine::enableGestureRecognition(true);
GestureType gesture = GestureRecognition::recognizeGesture();
```

## Chord Prediction

Predicts chords from played notes using music theory.

**Usage:**
```cpp
AIEngine::enableChordPrediction(true);
ChordPredictor::addPlayedNote(60); // C
Chord chord = ChordPredictor::getCurrentChord();
```

## Tempo Detection

Detects tempo from sensor beat patterns.

**Usage:**
```cpp
AIEngine::enableTempoDetection(true);
TempoDetector::recordBeat();
float tempo = TempoDetector::getCurrentTempo(); // BPM
```

## Pitch Correction

Real-time pitch detection and correction.

**Usage:**
```cpp
AIEngine::enablePitchCorrection(true);
float pitch = PitchCorrector::detectPitch(audioBuffer, length);
float correction = PitchCorrector::correctPitch(pitch, targetPitch);
```

## Pattern Learning

Learns and recognizes playing patterns.

**Usage:**
```cpp
AIEngine::enablePatternLearning(true);
PatternLearner::startLearning();
PatternLearner::recordNote(note, timing);
Pattern pattern = PatternLearner::recognizePattern();
```
