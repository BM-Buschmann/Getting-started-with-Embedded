# Embedded Lab 3 - Labor Microcomputertechnik 3

## German


Bisher wurden die Wartezeit im Mikrocontroller über eine Delay-Funktion realisiert. In diesem Versuch soll dies über die Verwendung eines Timers realisiert werden. Wird die Reaktion auf den Ablauf des Timers in einer Interrupt-Service-Routine untergebracht, kann das Programm noch andere Funktionen während des Wartens ausführen. Für diesen Versuch soll ein Memory-Spiel umgesetzt werden. Ähnlich dem Spiel Simon.
Dabei gibt das Spiel eine Sequenz von nacheinander blinkenden LEDs vor, welche durch den Spieler in der richtigen Reihenfolge nachgemacht werden müssen. Dazu werden in diesem Versuch die LEDs 1 – 3, sowie die Buttons PB5 – PB7 verwendet.

### Hinweise

Sofern nicht anders vermerkt, sollen Sie alle Teilversuche eines Übungsblattes immer in einem Projekt bearbeiten. Die geforderten Teilaufgaben sollen somit alle gleichzeitig ablaufen.

### Aufgabenstellung

Verbinden Sie die rote LED (JP8 LED_RT) mit **CON3:P3.0** und die grüne LED (JP8 LED_GN) mit
**CON3:P3.1**.
Verbinden sie Taster **PB5** mit **CON3:P1.3**, Taster **PB6** mit **CON3:P1.4** und Taster **PB7** mit **CON3:P1.5**.

1. Die blaue LED soll über den Pin **CON3:P3.2** angesteuert werden. Suchen Sie hierzu den erforderlichen Pin an X1 für die blaue LED aus dem Schaltplan heraus. Geben Sie außerdem in einem Kommentar im Header Ihres Programms an, welche zusätzliche Funktion dieser Pin bereitstellt und wie man diese aktivieren kann.

1. Schreiben sie ein Programm, das beim Drücken eines der drei Taster alle LEDs einmal für 2s einschaltet und anschließen für 2s aus. Für diese Zeiten darf die Delay Funktion verwendet werden.

1. Erweitern sie das Programm dahingehend, dass nach 2) fünf Sequenzen (unsere Levels) mit drei nacheinander blinkenden LEDs ausgegeben werden. Jede Sequenz soll eine pseudo-zufällige Reihenfolge ausgeben. Diese kann beispielsweise mit einem Array fest im Programm hinterlegt sein. Es kann die gleiche LED auch zwei Mal innerhalb einer Sequenz blinken. Jede Sequenz soll aber nur einmal vorkommen. Damit der Schwierigkeitsgrad mit jeder Sequenz zunimmt, sollen die LEDs wie folgt blinken:
    1. Sequenz: LED für 2s an, Duty-Cycle 50%
    1. Sequenz: LED für 1s an, Duty-Cycle 50%
    1. Sequenz: LED für 500ms an, Duty-Cycle 50%
    1. Sequenz: LED für 300ms an, Duty-Cycle 50%
    1. Sequenz: LED für 100ms an, Duty-Cycle 50%
    *Für die Realisierung der Zeiten soll hier **Timer_A** des MSP430 verwendet werden. Beachten Sie hierzu Kapitel 12 des **MSP430 Family User Guide***

1. Erweitern sie ihr Programm um die folgenden drei Funktionen:
    1. void roundwon(void) – alle LEDs an 2-mal an und 2-mal aus innerhalb einer Sekunde.
    1. void roundlost(void) – alle LEDs drei Mal innerhalb 3 Sekunden im Uhrzeigersinn aufleuchten lassen.
    1. void gamewon(void) – alle LEDs drei Mal innerhalb 3 Sekunden gegen den Uhrzeigersinn aufleuchten lassen.

    *In diesen Funktionen können sie den Timer verwenden, müssen es aber nicht.*

1. Erweitern Sie das Programm so, dass nach jedem Level die eben angezeigte Sequenz per Tastendruck eingegeben werden soll.
    1. Hierbei ist folgende Zuordnung ist zu beachten:
        1. PB5 -> rote LED,
        1. PB6 -> grüne LED,
        1. PB7 -> blaue LED.
    1. Die Schalter müssen für eine korrekte Auswertung entprellt werden. Wurde eine Sequenz richtig eingegeben, soll die Funktion roundwon() aufgerufen werden. Und zum nächsten Level gewechselt werden. Dieses soll dann nach einmaligem Aufblinken aller LEDs wie in Aufgabenteil 2) direkt starten.Wurde eine Sequenz falsch eingegeben, soll die Funktion roundlost() aufgerufen werden. Danach soll das Spiel von Level 1 erneut beginnen. Es soll dann wieder über einen beliebigen Tastendruck wie in Aufgabenteil 2) gestartet werden können. Wurde ein Levels erfolgreich durchlaufen, soll die Funktion gamewon() aufgerufen werden. Ein neues Spiel soll wieder über einen beliebigen Tastendruck gestartet werden können.

## English


So far, the delay in the microcontroller has been realized using a delay function. In this experiment, this is to be implemented using a timer. If the response to the timer's expiration is placed in an interrupt service routine, the program can perform other functions while waiting. For this experiment, a memory game similar to Simon should be implemented. The game provides a sequence of sequentially flashing LEDs that the player must replicate in the correct order. In this experiment, LEDs 1–3 and buttons PB5–PB7 will be used.

### Notes

Unless otherwise specified, you should always work on all sub-tasks of an exercise sheet in one project. The required subtasks should thus all run simultaneously.

### Task Description

Connect the red LED (JP8 LED_RT) to **CON3:P3.0** and the green LED (JP8 LED_GN) to
**CON3:P3.1**.
Connect button **PB5** to **CON3:P1.3**, button **PB6** to **CON3:P1.4**, and button **PB7** to **CON3:P1.5**.

1. The blue LED should be controlled via pin **CON3:P3.2**. Identify the required pin for the blue LED from X1 in the circuit diagram. Also, indicate in a comment in the header of your program what additional function this pin provides and how to activate it.

1. Write a program that, when one of the three buttons is pressed, turns on all LEDs once for 2 seconds and then turns them off for 2 seconds. The delay function may be used for these times.

1. Expand the program so that after 2), five sequences (our levels) with three sequentially flashing LEDs are output. Each sequence should output a pseudo-random order. This can be, for example, fixed in the program with an array. The same LED can also flash twice within a sequence. However, each sequence should only occur once. To increase the difficulty level with each sequence, the LEDs should flash as follows:
    1. Sequence: LED on for 2 seconds, Duty-Cycle 50%
    1. Sequence: LED on for 1 second, Duty-Cycle 50%
    1. Sequence: LED on for 500ms, Duty-Cycle 50%
    1. Sequence: LED on for 300ms, Duty-Cycle 50%
    1. Sequence: LED on for 100ms, Duty-Cycle 50%

    For the implementation of the times, use **Timer_A** of the MSP430. Refer to Chapter 12 of the **MSP430 Family User Guide**.

1. Expand your program with the following three functions:
    1. void roundwon(void) – all LEDs on twice and off twice within one second.
    1. void roundlost(void) – all LEDs flash three times counterclockwise within 3 seconds.
    1. void gamewon(void) – all LEDs flash three times counterclockwise within 3 seconds.

    In these functions, you can use the timer, but it is not mandatory.

1. Extend the program so that after each level, the sequence just displayed should be entered by pressing a button.
    1. The following assignment is to be observed:
        1. PB5 -> red LED,
        1. PB6 -> green LED,
        1. PB7 -> blue LED.

    1. The switches must be debounced for correct evaluation. If a sequence is entered correctly, the roundwon() function should be called. And switch to the next level. This should then start after a single flash of all LEDs as in part 2). If a sequence is entered incorrectly, the roundlost() function should be called. After that, the game should start again from level 1. It should then be able to be started again with any key press, as in part 2). If a level is successfully completed, the gamewon() function should be called. A new game should be able to be started again with any key press.
