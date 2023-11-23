# uart2

The goal of this assignment is to further work with UART from Python.
Template for this week implements a simplified version of GSM/GPRS Modem.
The modem is connected to the computer via UART and it is controlled by AT commands.
Your task is to implement a simple Python script that will use the modem to connect to the internet, download data from web and then make a call using the downloaded data as audio.

## Modem description

### Communication

UART is used for communication with the modem.

#### Parameters
    * Baudrate: 115200
    * Data bits: 8
    * Parity: None
    * Stop bits: 1
    * Flow control: None

### Command Mode

The modem starts in command mode.
In command mode the modem accepts AT commands.

#### AT commands

AT commands are used to control the modem.
The commands are sent to the modem via UART and the modem responds with a response.
The response is in form of one or multiple lines.
Lines are terminated by `\n`.

### Data mode

In data mode the modem forwards all data from UART to the socket and vice versa.
Be careful about line endings.

#### Notes

The server will send you a list of notes.
The notes are represented in textual form as follows:
```
<tone><octave> : <duration>
```
Where `<tone>` is one of `C`, `C#`, `D`, `D#`, `E`, `F`, `F#`, `G`, `G#`, `A`, `A#`, `B`.
Where `<octave>` is one of `3`, `4`, `5`, `6`.
Where `<duration>` is one of `1`, `1/2`, `1/4`, `1/8`.

### Call Mode

In call mode the modem interprets all bytes as notes.
The modem uses the notes to play a sound to the other side of the call.

#### Notes

The notes are encoded as bytes in following manner:
```c
enum Octave : uint8_t {
    O3 = 0,
    O4 = 1,
    O5 = 2,
    O6 = 3,
};

enum Tone : uint8_t {
    C,
    C_, // C sharp
    D,
    D_, // D sharp
    E,
    F,
    F_, // F sharp
    G,
    G_, // G sharp
    A,
    A_, // A sharp
    B,
};

enum Duration : uint8_t {
    Whole = 0, // 1
    Half = 1, // 1/2
    Quarter = 2, // 1/4
    Eighth = 3, // 1/8
};

struct Note {
    Octave octave : 2;
    Tone tone : 4;
    Duration duration : 2;
};

```

Meaning the highest 2 bits represent the duration, the middle 4 bits represent the tone and the lowest 2 bits represent the octave.

## Task

1.  Establish a connection to the modem.
2.  Connect to the network.
3.  Establish a socket connection to the server.
4.  Enter data mode.
5.  Post a GET request to the server.
6.  Exit data mode.
7.  Terminate the socket connection.
8.  Start a call.
9.  Enter call mode.
10. Play the downloaded data as audio.
11. Exit call mode.
12. Terminate the call.

P.S: you can find server name/phone number and other information in `source/config.hpp`

## Notes

* Don't forget to handle possible errors (the ones not caused by you).
    * You can press SW3 before the end of a command to make the command after it fail (if it can).
* Since this is a minimimal simulation, only the required GET request to the correct ip, with correct host and path is supported.
    * Same goes for the phone number
    * The module is not really connected to the internet
* The module will send you aditional debug info over the debug console.
    * This is so you can see what is happening.
    * Don't access this in the script, you won't be able to do so in the tests.

## Commands

- `AT`
    + Test command.
    + Returns `"OK"`

- `AT+RST`
    + Resets the modem.
    + Returns `"OK"`.

- `ATI`
    + Get information about the modem.
    + Returns information about the modem, followed by `"OK"`.

- `+++`
    + Switches from data mode and call mode to command mode.
    + Returns `"OK"`.

- `ATO`
    + Switches from command mode to data mode.
    + Returns `"CONNECT"` or `"ERROR"`.

- `AT+CGATT?`
    + Connects to network and returns state of the connection.
    + Returns `+CGATT: <state>` followed by `"OK"`.

- `AT+CIICR`
    + Toggles GPRS.
    + Returns `"OK"`.

- `AT+CIPSTART="<server>","<port>"`
    + Establishes a socket connection to the server.
    + Returns `"CONNECT OK"` or `"CONNECT FAIL"` or `"CONNECT REFUSE"` or `"ALREADY CONNECT"`.

- `AT+CIPCLOSE`
    + Terminates the socket connection.
    + Returns `"OK"` or `"NO CONNECTION"`.

- `ATD<phone_number>`
    + Calls the phone number.
    + Returns `"OK"` or `"NO CARRIER"` or `"BUSY"` or `"NO ANSWER"`.

- `ATH`
    + Terminates the call.
    + Returns `"OK"` or `"NO CALL"`.

- `ATC`
    + Enters call mode.
    + Returns `"OK"` or `"NO CALL"`.
