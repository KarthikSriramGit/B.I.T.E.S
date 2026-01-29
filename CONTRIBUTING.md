# Contributing to B.I.T.E.S

Thank you for your interest in contributing to B.I.T.E.S! This document provides guidelines and instructions for contributing.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/B.I.T.E.S.git`
3. Create a branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test your changes thoroughly
6. Commit your changes: `git commit -m "Add feature: description"`
7. Push to your fork: `git push origin feature/your-feature-name`
8. Open a Pull Request

## Development Setup

### Prerequisites

- PlatformIO or Arduino IDE
- Teensy 4.1 development board
- Required sensors (MPU6050, piezo, IR, pressure, flex sensors)
- Teensy Audio Shield (optional but recommended)

### Building

Using PlatformIO:
```bash
pio run -e teensy41
```

Using Arduino IDE:
1. Install TeensyDuino addon
2. Select Board: Teensy 4.1
3. Upload

## Code Style

- Follow C++17 standards
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and small
- Use consistent indentation (2 spaces)
- Follow the existing code structure

## Commit Messages

- Use clear, descriptive commit messages
- Start with a verb (Add, Fix, Update, Remove)
- Reference issue numbers when applicable
- Keep the first line under 72 characters

Example:
```
Add MPU6050 sensor driver with Kalman filtering

Implements sensor fusion for gesture recognition using
Kalman filter algorithm. Includes calibration routines
and noise reduction.
```

## Pull Request Process

1. Ensure your code compiles without errors
2. Update documentation if needed
3. Add tests for new features
4. Update CHANGELOG.md if applicable
5. Request review from maintainers

## Testing

- Write unit tests for new features
- Test on actual hardware when possible
- Document any hardware-specific requirements
- Include example code demonstrating usage

## Reporting Issues

When reporting bugs or requesting features:

- Use the issue templates
- Provide clear description of the problem
- Include hardware configuration
- Add code examples if applicable
- Include error messages or logs

## Feature Requests

We welcome feature requests! Please:

- Check if the feature already exists
- Describe the use case
- Explain how it fits into the project
- Consider implementation complexity

## Questions?

Feel free to open an issue with the "question" label or reach out to maintainers.

Thank you for contributing to B.I.T.E.S!
